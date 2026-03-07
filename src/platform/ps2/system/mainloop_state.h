#pragma once

#include "types.h"

void PathTruncFileName(Char *pOut, Char *pStr, Int32 nMaxChars);
int PathGetMaxFileNameLength(const char *pPath);

Bool _MainLoopHasSRAM();
Bool _MainLoopSaveSRAM(Bool bSync);
void _MainLoopLoadSRAM();
Bool _MainLoopCheckSRAM();
void _MainLoopLoadState();
void _MainLoopSaveState();
Bool _MainLoopSaveSRAM(Bool bForce);
#if MAINLOOP_HISTORY
void _MainLoopResetHistory();
#endif
void _MainLoopResetInputChecksums();
