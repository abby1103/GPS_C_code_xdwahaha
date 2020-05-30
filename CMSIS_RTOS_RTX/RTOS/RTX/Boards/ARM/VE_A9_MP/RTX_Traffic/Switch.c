/*
** Copyright (C) ARM Limited, 2011-2013. All rights reserved.
*/

/* Simple GPIO driver for Cortex-A9x4 VX FVP model */

#include "Switch.h"

/*switch*/
unsigned int Get_Switch(void)
{
    return SWITCH->SWITCH_SOFT & 0x0f;
}


void Set_Switch(unsigned int val)
{
    SWITCH->SWITCH_SOFT = val;
}
