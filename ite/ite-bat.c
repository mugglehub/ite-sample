#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"


//=============================================================================
//  ite_bat_ac_status
//=============================================================================
uint8_t ite_bat_ac_status(void)
{
	uint8_t		buf[2];
	uint8_t		off = 0x90;
	uint8_t		sts;

	if (ite->bat.num == 0)
		return 0xFF;

	// read ecram
	ite_ram_read(ITE_ECRAM_TYP_ACPI, off, 1, buf);
	
	// AC0: bit0 0=removal, 1=plugin
	sts = buf[0] & 0x01;
	return !!sts;
}

//=============================================================================
//  ite_bat_status
//=============================================================================
uint8_t ite_bat_status(uint8_t bat)
{
	uint8_t		buf[2];
	uint8_t		off = 0x90;
	uint8_t		sts;

	if (ite->bat.num == 0)
		return 0xFF;

	// read ecram
	ite_ram_read(ITE_ECRAM_TYP_ACPI, off, 1, buf);
	
	if (bat == 0)
	{
		// BAT0: bit1 0=removal, 1=plugin
		sts = buf[0] & 0x02;
		return !!sts;
	}
	else if (bat == 1)
	{
		// BAT1: bit2 0=removal, 1=plugin
		sts = buf[0] & 0x04;
		return !!sts;
	}

	return 0xFF;
}
	
//=============================================================================
//  ite_bat_init
//=============================================================================
void ite_bat_init(void)
{
	int		i;
	uint8_t	did;

	ite->bat.num = 0;
	
	for (i=0; i<ite->dt.num; i++)
	{
		if (i >= ITE_MAX_NUM_DEV)
			break;

		did = ite->dt.dev[i].did;

		// battery
		if (did == DEV_ID_BAT0 || did == DEV_ID_BAT1)
		{
			if (ite->bat.num < ITE_MAX_NUM_BAT)
			{
				ite->bat.dev[ ite->bat.num ] = did;
				ite->bat.num++;

				// volt mask
				ite->bat.mask |= (1 << (did - DEV_ID_BAT0));
			}
		}
	}
}
