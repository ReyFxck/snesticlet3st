#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "types.h"
#include "console.h"
#include "file.h"
#include "dataio.h"
#include "pathext.h"
#include "snppucolor.h"
#include "emurom.h"
#include "mainloop_load.h"
#include "zlib.h"

extern "C" {
#include "unzip.h"
}

void _MainLoopGetName(Char *pName, const Char *pPath)
{
        const Char *pFileName;

        pFileName = strrchr(pPath, '/');
        if (pFileName==NULL)
        {
                pFileName = pPath;
        } else
        {
                // skip /
                pFileName = pFileName + 1;
        }
        strcpy(pName, pFileName);
}

int _MainLoopReadBinaryData(Uint8 *pBuffer, Int32 nBufferBytes, const char *pRomFile)
{
        int nBytes = 0;
        int hFile;

        hFile = open(pRomFile, O_RDONLY);
        if (hFile < 0)
        {
                return -1;
        }

        nBytes = read(hFile, pBuffer, nBufferBytes);
        close(hFile);

        return nBytes;
}

int _MainLoopReadGZData(Uint8 *pBuffer, Int32 nBufferBytes, const char *pRomFile)
{
        int nBytes = 0;
        gzFile pFile;

        pFile = gzopen(pRomFile, "rb");
        if (!pFile)
        {
                return -1;
        }
        nBytes = gzread(pFile, pBuffer, nBufferBytes);
        gzclose(pFile);

        return nBytes;
}

int _MainLoopReadZipData(Uint8 *pBuffer, Int32 nBufferBytes, const char *pZipFile, char *pFileName)
{
        unzFile hFile;
        unz_file_info file_info;
        char filename[256];
        int nBytes = 0;

        hFile = unzOpen(pZipFile);
        if (!hFile)
        {
                return -1;
        }
        printf("ZIP: file opened\n");

        do
        {
                if (unzGetCurrentFileInfo(hFile, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0) != UNZ_OK)
                        break;

                printf("ZIP: file %s (%d)\n", filename, (int)file_info.uncompressed_size);

                if (file_info.uncompressed_size <= (unsigned)nBufferBytes)
                {
                        PathExtTypeE eType;
                        // do we recognize this file type?
                        if (PathExtResolve(filename, &eType, FALSE))
                        {
                                printf("ZIP: read %s (%d)\n", filename, (int)file_info.uncompressed_size);

                                // if so, read it
                                if (unzOpenCurrentFile(hFile) == UNZ_OK)
                                {
                                        if (unzReadCurrentFile(hFile, pBuffer, file_info.uncompressed_size) > 0)
                                        {
                                                if (pFileName)
                                                        strcpy(pFileName, filename);
                                                nBytes = (int)file_info.uncompressed_size;
                                        }
                                        unzCloseCurrentFile(hFile);
                                }
                        }
                }

        } while (nBytes == 0 && unzGoToNextFile(hFile) == UNZ_OK);

        printf("ZIP: file closed (%d)\n", nBytes);

        unzClose(hFile);

        return nBytes;
}

Bool _MainLoopLoadRomData(Emu::Rom *pRom, Uint8 *pRomData, Int32 nRomBytes)
{
        CMemFileIO romfile;
        Emu::Rom::LoadErrorE eError;

        // open memoryfile for rom data
        romfile.Open(pRomData, nRomBytes);

        // load rom
        eError = pRom->LoadRom(&romfile);
        romfile.Close();

        if (eError!=Emu::Rom::LoadErrorE::LOADERROR_NONE)
        {
                ConPrint("ERROR: loading rom %d\n", eError);
                return FALSE;
        }
        return TRUE;
}

Bool _MainLoopLoadBios(Emu::Rom *pRom, const Char *pFilePath)
{
        CFileIO romfile;
        Emu::Rom::LoadErrorE eError;

        // open memoryfile for rom data
        if (!romfile.Open(pFilePath, "rb"))
        {
                ConPrint("ERROR: loading fds bios!\n");
                return FALSE;
        }

        // load rom
        eError = pRom->LoadRom(&romfile);
        romfile.Close();

        if (eError!=Emu::Rom::LoadErrorE::LOADERROR_NONE)
        {
                ConPrint("ERROR: loading rom %d\n", eError);
                return FALSE;
        }
        return TRUE;
}

Bool _MainLoopLoadSnesPalette(const char *pFileName)
{
        Uint32 *pPalData;
        pPalData = SNPPUColorGetPalette();

        return _MainLoopReadBinaryData((Uint8 *)pPalData, SNPPUCOLOR_NUM * sizeof(Uint32), pFileName) > 0;
}
