#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"

//=============================================================================
//  ite_gpio_read_conifg
//=============================================================================
static uint8_t ite_gpio_read_conifg(uint8_t did, uint8_t *cfg)
{
	uint8_t		res;
	uint8_t		wb[50];
	uint8_t		rb[50];


	memset(wb, 0, sizeof(wb));
	memset(rb, 0, sizeof(rb));

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 2;		// wblen=2
	wb[1] = did;	// par  : device id

	rb[0] = 1;		// rlen : size to read by caller (rb[1]=pin state)
	rb[1] = 0;		// roff : dat0~

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_GPIOCFG_RD, wb, rb);

	if (res != ITE_OK)
	{
		*cfg = 0xFF;
		return res;
	}

	//---------------------------------------------------------------------
	// rb   <0>   <1>   <2>   <3>
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//---------------------------------------------------------------------
	*cfg = rb[4];	// dat0~

	return ITE_OK;
}

//=============================================================================
//  ite_gpio_write_conifg
//=============================================================================
static uint8_t ite_gpio_write_conifg(uint8_t did, uint8_t cfg)
{
	uint8_t		res;
	uint8_t		wb[50];
	uint8_t		rb[50];


	memset(wb, 0, sizeof(wb));
	memset(rb, 0, sizeof(rb));

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 6;		// wblen=6
	wb[1] = did;	// par  : device id
	wb[2] = 0;		// dlen : useless
	wb[3] = 0;		// woff : dat0~
	wb[4] = 1;		// wlen : dat0
	wb[5] = cfg;	// dat0 : gpio_cfg

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_GPIOCFG_WR, wb, rb);

	return res;
}

//=============================================================================
//  ite_gpio_get_dir
//=============================================================================
uint8_t ite_gpio_get_dir(uint8_t ch, uint8_t *dir)
{
	uint8_t		res;
	uint8_t		dr;
	uint8_t		dat;

	if (ite->gpio.num == 0)
		return ITE_ERR_NODEV;

	if (ch >= ITE_MAX_NUM_GPIO)
		return ITE_ERR_CH;
	
	// n/a
	if ((ite->gpio.mask & (1<<ch)) == 0)
		return ITE_ERR_NODEV;

	// dr:
	// bit[7:6] : 00=alt, 01=output, 10=input, 11=n/a
	// bit[5:3] : reserved
	// bit[2:1] : 00=pull_none, 01=pull_down, 10=pull_up, 11=pull_??
	// bit[0]   : 0=low, 1=high
	if(ch < 8)
		res = ite_gpio_read_conifg(DEV_ID_GPIO0 + ch, &dr);
	else
		res = ite_gpio_read_conifg(DEV_ID_GPIO8 + (ch - 8), &dr);

	if (res != ITE_OK)
	{
		*dir = 0xFF;
		return res;
	}

	dat = (dr >> 6) & 0x3;

	if (dat == 0x01)
	{
		*dir = 1;	// output
	}
	else if (dat == 0x02)
	{
		*dir = 0;	// input
	}
	else
	{
		*dir = 0xFF;
		return ITE_ERR_NODEV;
	}
	
	return ITE_OK;
}

//=============================================================================
//  ite_gpio_set_dir
//=============================================================================
uint8_t ite_gpio_set_dir(uint8_t ch, uint8_t dir)
{
	uint8_t		res;
	uint8_t		dr = 0;

	if (ite->gpio.num == 0)
		return ITE_ERR_NODEV;

	if (ch >= ITE_MAX_NUM_GPIO)
		return ITE_ERR_CH;
	
	// n/a
	if ((ite->gpio.mask & (1<<ch)) == 0)
		return ITE_ERR_NODEV;

	// dir: 0=input, 1=output, !0=output
	if (dir)
		dr = 0x40;	// output
	else
		dr = 0x80;	// input
	
	if (ch < 8)
		res = ite_gpio_write_conifg(DEV_ID_GPIO0 + ch, dr);
	else
		res = ite_gpio_write_conifg(DEV_ID_GPIO8 + (ch - 8), dr);

	ITE_ERR_CHECK(res);
		
	return ITE_OK;
}

//=============================================================================
//  ite_gpio_get_dat
//=============================================================================
uint8_t ite_gpio_get_dat(uint8_t ch, uint8_t *dat)
{
	uint8_t		res;
	uint8_t		level;

	if (ite->gpio.num == 0)
		return ITE_ERR_NODEV;
	
	// ch : 0 ~ 15
	if (ch >= ITE_MAX_NUM_GPIO)
		return ITE_ERR_CH;
	
	// n/a
	if ((ite->gpio.mask & (1<<ch)) == 0)
		return ITE_ERR_NODEV;

	// level:
	// bit[7:6] : 00=alt, 01=output, 10=input, 11=n/a
	// bit[5:3] : reserved
	// bit[2:1] : 00=pull_none, 01=pull_down, 10=pull_up, 11=pull_??
	// bit[0]   : 0=low, 1=high
	if(ch < 8)
		res = ite_dt_read_hwpin(DEV_ID_GPIO0 + ch, &level);
	else
		res = ite_dt_read_hwpin(DEV_ID_GPIO8 + (ch - 8), &level);

	if (res != ITE_OK)
	{
		*dat = 0xFF;
		return res;
	}
	
	*dat = !!(level & 0x1);	// 1=high, 0=low

	return ITE_OK;
}

//=============================================================================
//  ite_gpio_set_dat
//=============================================================================
uint8_t ite_gpio_set_dat(uint8_t ch, uint8_t dat)
{
	uint8_t		res;
	uint8_t		level;

	if (ite->gpio.num == 0)
		return ITE_ERR_NODEV;
		
	// ch : 0 ~ 15
	if (ch >= ITE_MAX_NUM_GPIO)
		return ITE_ERR_CH;
	
	// n/a
	if ((ite->gpio.mask & (1<<ch)) == 0)
		return ITE_ERR_NODEV;

	//0:Low	1:High
	level = !!dat;
		
	if(ch < 8)
		res = ite_dt_write_hwpin(DEV_ID_GPIO0 + ch, level);
	else
		res = ite_dt_write_hwpin(DEV_ID_GPIO8 + (ch - 8), level);

	ITE_ERR_CHECK(res);
		
	return ITE_OK;
}

//=============================================================================
//  ite_gpio_init
//=============================================================================
void ite_gpio_init(void)
{
	int		i;
	uint8_t	did;

	ite->gpio.num = 0;
	
	for (i=0; i<ite->dt.num; i++)
	{
		if (i >= ITE_MAX_NUM_DEV)
			break;

		did = ite->dt.dev[i].did;

		// gpio
		if ((did >= DEV_ID_GPIO0 && did <= DEV_ID_GPIO7) ||
		    (did >= DEV_ID_GPIO8 && did <= DEV_ID_GPIO15))
		{
			if (ite->gpio.num < ITE_MAX_NUM_GPIO)
			{
				ite->gpio.dev[ ite->gpio.num ] = did;
				ite->gpio.num++;

				// gpio mask
				if (did >= DEV_ID_GPIO0 && did <= DEV_ID_GPIO7)
				{
					ite->gpio.mask |= (1 << (did - DEV_ID_GPIO0));
				}
				else if (did >= DEV_ID_GPIO8 && did <= DEV_ID_GPIO15)
				{
					ite->gpio.mask |= (1 << (did - DEV_ID_GPIO8 + 8));
				}
			}
		}
	}
}
