#pragma once

#include "types.h"

Int32 IOPLoadModule(const Char *pModuleName, Char **ppSearchPaths, int arglen, const char *pArgs);
extern Char _MainLoop_BootDir[256];
extern Char *_MainLoop_IOPModulePaths[];
void _MainLoopLoadModules(Char **ppSearchPaths);
