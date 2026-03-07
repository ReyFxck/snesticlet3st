#include "types.h"
#include <libpad.h>
#include "snes.h"
#include "mainloop_input.h"

static Uint16 _MainLoopSnesInput(Uint32 cond)
{
	Uint32 pad = 0;

	if (cond & PAD_LEFT)    pad|= (SNESIO_JOY_LEFT);
	if (cond & PAD_RIGHT)   pad|= (SNESIO_JOY_RIGHT);
	if (cond & PAD_UP)      pad|= (SNESIO_JOY_UP);
	if (cond & PAD_DOWN)    pad|= (SNESIO_JOY_DOWN);

	if (cond & PAD_SQUARE)   pad|= (SNESIO_JOY_Y);
	if (cond & PAD_TRIANGLE) pad|= (SNESIO_JOY_X);
	if (cond & PAD_CROSS)    pad|= (SNESIO_JOY_B);
	if (cond & PAD_CIRCLE)   pad|= (SNESIO_JOY_A);

	if (cond & PAD_L1)   pad|= (SNESIO_JOY_L);
	if (cond & PAD_R1)   pad|= (SNESIO_JOY_R);

	if (cond & PAD_SELECT)  pad|= (SNESIO_JOY_SELECT);
	if (cond & PAD_START)   pad|= (SNESIO_JOY_START);
	return pad;
}

Uint16 _MainLoopInput(Uint32 pad)
{
	if (pad & (PAD_R2|PAD_L2))
    {
        return 0;
    }
#if 0
    if (_pSystem==_pSnes)
    {
        return _MainLoopSnesInput(pad);
    } else
    if (_pSystem==_pNes)
    {
        return _MainLoopNesInput(pad);
    }
	   return 0;
#else
 	return _MainLoopSnesInput(pad);
#endif
 
}
