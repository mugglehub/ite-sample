#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"

//=============================================================================
//  ite_volt_read
//=============================================================================
uint8_t ite_volt_read(uint8_t ch, double *volt)
{
	uint8_t		res;
	uint8_t		did;
	uint8_t		buf[2];
	uint16_t	val;

	if (ite->volt.num == 0)
		return ITE_ERR_NODEV;
	
	if (ch >= ite->volt.num)
		return ITE_ERR_CH;
	
	did = ite->volt.dev[ch];
	
	res = ite_dt_read_hwpin(did, buf);

	if (res != ITE_OK)
	{
		*volt = 0.0;
		return res;
	}

	val = *(uint16_t *)buf;
	val = (int)swap16((uint16_t)val);
	
	*volt = (double)val;
	*volt = *volt * 3 / 1023;
			
	switch ((did - DEV_ID_ADC_BAT_CMOS) % 3)
	{
		case 0:					break;		// X1
		case 1: *volt *= 2.0;	break;		// X2
		case 2: *volt *= 10.0;	break;		// X10
	}
	
	return ITE_OK;
}

//=============================================================================
//  ite_volt_init
//=============================================================================
void ite_volt_init(void)
{
	int		i;
	uint8_t	did;

	ite->volt.num = 0;
	
	for (i=0; i<ite->dt.num; i++)
	{
		if (i >= ITE_MAX_NUM_DEV)
			break;

		did = ite->dt.dev[i].did;

		// volt
		if (did >= DEV_ID_ADC_BAT_CMOS && did <= DEV_ID_ADC_DCOEMX10)
		{
			if (ite->volt.num < ITE_MAX_NUM_VOLT)
			{
				ite->volt.dev[ ite->volt.num ] = did;
				ite->volt.num++;

				// volt mask
				ite->volt.mask |= (1 << ((did - DEV_ID_ADC_BAT_CMOS)/3));
			}
		}
	}
}
