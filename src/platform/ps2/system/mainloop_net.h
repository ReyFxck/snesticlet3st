#pragma once

#include "types.h"

extern "C" {
#include "netplay_ee.h"
}

extern char *_MainLoop_NetConfigPaths[];

Bool _MainLoopConfigureNetwork(char **ppSearchPaths, char *pConfigFileName);
Bool _MainLoopInitNetwork(Char **ppSearchPaths);
void *_MainLoopNetCallback(NetPlayCallbackE eCallback, char *data, int size);
