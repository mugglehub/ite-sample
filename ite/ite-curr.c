#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"

//=============================================================================
//  ite_curr_read
//=============================================================================
uint8_t ite_curr_read(uint8_t ch, double *curr)
{
	uint8_t		res;
	uint8_t		did;
	uint8_t		buf[8];
	uint16_t	val;

	if (ite->curr.num == 0)
		return ITE_ERR_NODEV;
	
	if (ch >= ite->curr.num)
		return ITE_ERR_CH;
	
	did = ite->curr.dev[ch];
	
	res = ite_dt_read_hwpin(did, buf);

	if (res != ITE_OK)
	{
		*curr = 0.0;
		return res;
	}

	val = *(uint16_t *)buf;
	val = (int)swap16((uint16_t)val);
	
	*curr = (double)val;
	*curr = *curr * 3 / 1023;
			

	// val as current
	// buf : [1] [2] [3] [4]
	val = *(uint16_t *)&buf[2];
	val = (int)swap16((uint16_t)val);

	*curr *= (double)val;
			
	// buf[4] as 10's exponential
	if (buf[4] & 0x80)
	{
		// 2's complment
		val = 0xFF - buf[4];
		val++;
				
		while (val--)
			*curr /= 10.0;	// 10^-1,-2,-3...
	}
	else
	{
		if (buf[4] & 0x7F)
		{
			val &= 0x7F;
			while (val--)
				*curr *= 10.0; 	// 10^1,2,3,4....
		}
	}

	return ITE_OK;
}

//=============================================================================
//  ite_curr_init
//=============================================================================
void ite_curr_init(void)
{
	int		i;
	uint8_t	did;

	ite->curr.num = 0;
	
	for (i=0; i<ite->dt.num; i++)
	{
		if (i >= ITE_MAX_NUM_DEV)
			break;

		did = ite->dt.dev[i].did;

		// current
		if (did == DEV_ID_ADC_CURR)
		{
			if (ite->curr.num < ITE_MAX_NUM_CURR)
			{
				ite->curr.dev[ ite->curr.num ] = did;
				ite->curr.num++;
			}
		}
	}
}
