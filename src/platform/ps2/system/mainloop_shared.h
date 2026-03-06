#pragma once

#include "types.h"
#include "snes.h"
#include "snstate.h"
#include "emusys.h"

extern Char _RomName[256];
extern Char _SramPath[256];

extern Emu::System *_pSystem;
extern SnesSystem *_pSnes;
extern SnesStateT _SnesState;

extern Uint32 _MainLoop_SRAMChecksum;
extern Uint32 _MainLoop_SaveCounter;
extern Uint32 _MainLoop_AutoSaveTime;
extern Bool _MainLoop_SRAMUpdated;
extern Bool _bStateSaved;
