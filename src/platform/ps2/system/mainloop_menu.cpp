#include <stdio.h>

#include "types.h"
#include "uiMenu.h"
#include "mainloop_install.h"
#include "mainloop_menu.h"

extern CMenuScreen *_MainLoop_pMenuScreen;
extern char *_MainLoop_pInstallFiles[];

int InstallFiles(char *pDestPath, char *pSrcPath, char **ppInstallFiles, int (*pCallBack)(char *pDestName, char *pSrcName, int Position, int Total));
int CopyFile(char *pDest, char *pSrc, int (*pCallBack)(char *pDestName, char *pSrcName, int Position, int Total));
extern "C" int list_title_db(char *pPath);

int _MainLoopMenuEvent(Uint32 Type, Uint32 Parm1, void *Parm2)
{
        switch (Type)
        {
                case 1:
                        {
                                char mc0[1024];
                                char mc1[1024];
                                char exploit_dir[256];
                                char **ppInstallFiles = _MainLoop_pInstallFiles;

                                _GetExploitDir(exploit_dir);

                                snprintf(mc0, sizeof(mc0), "mc0:/%s", exploit_dir);
                                snprintf(mc1, sizeof(mc1), "mc1:/%s", exploit_dir);

                                // hack in default destination name for elf
                                ppInstallFiles[0] = (char *)"BOOT.ELF"; // dest
                                switch (Parm1)
                                {
#if 0
                                        case 0:
                                                // cdrom->mc0
                                                ppInstallFiles[1] = VersionGetElfName(); // src
                                                InstallFiles(mc0, "cdrom0:\\", ppInstallFiles, _MainLoopInstallCallback);
                                                break;
                                        case 1:
                                                ppInstallFiles[1] = VersionGetElfName(); // src
                                                InstallFiles(mc1, "cdrom0:\\", ppInstallFiles, _MainLoopInstallCallback);
                                                break;
#endif
                                        case 2:
                                                ppInstallFiles[1] = (char *)"SNESTICLE.ELF"; // src
                                                InstallFiles(mc0, (char *)"host:", ppInstallFiles, _MainLoopInstallCallback);
                                                break;
                                        case 3:
                                                ppInstallFiles[1] = (char *)"BOOT.ELF"; // src
                                                InstallFiles(mc1, mc0, ppInstallFiles, _MainLoopInstallCallback);
                                                break;
                                        case 4:
                                                ppInstallFiles[1] = (char *)"BOOT.ELF"; // src
                                                InstallFiles(mc0, mc1, ppInstallFiles, _MainLoopInstallCallback);
                                                break;
                                        case 5:
                                                ppInstallFiles[0] = (char *)"SNESTICLE.ELF"; // dest
                                                ppInstallFiles[1] = (char *)"BOOT.ELF"; // src
                                                InstallFiles((char *)"host:", mc0, ppInstallFiles, _MainLoopInstallCallback);
                                                break;
                                        case 6:
                                                _DumpMemory();
                                                break;
                                        case 7:
                                                snprintf(mc0, sizeof(mc0), "mc0:/%s/TITLE.DB", exploit_dir);
                                                _AddTitleDB(mc0);
                                                break;
                                        case 8:
                                                snprintf(mc0, sizeof(mc0), "mc0:/%s/TITLE.DB", exploit_dir);
                                                list_title_db(mc0);
                                                break;
                                        case 9: // copy rom0:libsd -> host
                                                CopyFile((char *)"host:LIBSD.IRX", (char *)"rom0:LIBSD", NULL);
                                                break;
                                        default:
                                                return 0;
                                }
                                _MainLoop_pMenuScreen->SetText(0, "");
                                _MainLoop_pMenuScreen->SetText(1, "");
                                _MainLoop_pMenuScreen->SetText(2, "");
                        }
                        break;
        }

        return 0;
}
