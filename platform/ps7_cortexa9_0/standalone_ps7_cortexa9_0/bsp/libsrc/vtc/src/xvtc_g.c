#include "xvtc.h"

XVtc_Config XVtc_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,v-tc-6.2", /* compatible */
		0x43c00000, /* reg */
		0x401f, /* interrupts */
		0xf8f01000 /* interrupt-parent */
	},
	{
		"xlnx,v-tc-6.2", /* compatible */
		0x43c10000, /* reg */
		0x4021, /* interrupts */
		0xf8f01000 /* interrupt-parent */
	},
	 {
		 NULL
	}
};