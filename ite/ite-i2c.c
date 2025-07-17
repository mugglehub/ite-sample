#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"

//=============================================================================
//  ite_i2c_set_clock
//=============================================================================
uint8_t ite_i2c_set_clock(uint8_t ch, uint16_t clk)
{
	uint8_t		wb[50];
	uint8_t		rb[50];


	if (ch >= ITE_MAX_NUM_I2C)
		return ITE_ERR_CH;
	
	if (ite->i2c.num == 0)
		return ITE_ERR_CH;
	
	if ((ite->i2c.mask & (1<<ch)) == 0)
		return ITE_ERR_CH;

	if (!(clk == 50 || clk == 100 || clk == 400))
		return ITE_ERR_CH;
	
	memset(wb, 0, sizeof(wb));
	memset(rb, 0, sizeof(rb));

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 7;					// len: wblen=7
	wb[1] = ite->i2c.dev[ch];	// par: dev id
	wb[2] = 0;					// dlen : useless
	wb[3] = 0;					// woff : dat0~
	wb[4] = 2;					// wlen : 2-bytes
	
	if (clk == 50)
		wb[5] = 0x01;			// dat0 : 50 kHz
	else if (clk == 100)
		wb[5] = 0x02;			// dat0 : 100 kHz
	else if (clk == 400)
		wb[5] = 0x03;			// dat0 : 400 kHz
	
	wb[6] = 0;					// dat1 : 

	// cmd issued
	return ite_mbx_trans(ITE_MBX_CMD_SMB_FREQ_WR, wb, rb);

}

//=============================================================================
//  ite_i2c_probe
//=============================================================================
uint8_t ite_i2c_probe(uint8_t ch, uint8_t sla)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	rlen;


	if (!(ite->i2c.mask & (1 << ch)))
		return ITE_ERR_CH;
	
	did = ite->i2c.dev[ ch ];
	
	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_WR_QUICK, did, sla, 0x00, NULL, 0, NULL, &rlen);
	
	if ((res & ITE_SMB_ERR_MASK) != ITE_SMB_ERR_ADDR_NOT_ACK && (res & ITE_SMB_ERR_MASK) < ITE_SMB_ERR_ALM)
		return ITE_OK;

	return ITE_ERR_NODEV;
}

//=============================================================================
//  ite_i2c_trans
//=============================================================================
uint8_t ite_i2c_trans(uint8_t ch, uint8_t sla, uint8_t *wdat, uint32_t wlen, uint8_t *rbuf, uint32_t *rlen)
{
	uint8_t		res;
	uint8_t		did;
	uint8_t		mbxcmd;
	

	if (!(ite->i2c.mask & (1 << ch)))
		return ITE_ERR_CH;
	
	did = ite->i2c.dev[ ch ];

	// buffer ram size
	// IT8528 : 256 bytes
	// IT5121 : 320 bytes
	// IT5782 : 320 bytes

	// there are 3 mbx cmd:
	// 0Fh: wlen<=41, rlen<=41
	// 01h: wlen<=41, rlen<=256
	// 0Dh: wlen<=256, rlen<=256 (IT8528)
	// 0Dh: wlen<=320, rlen<=320 (IT5121)
	// 0Dh: wlen<=320, rlen<=320 (IT5782)

	//-------------------------------------------------------------------------
	//  0Fh: ITE_MBX_CMD_I2C_WR_RD
	//-------------------------------------------------------------------------
	// Due to the limitation of mailbox size should be under 42 bytes when using the ITE_MBX_CMD_I2C_WR_RD (0Fh) command.
	// It can be used the ITE_MBX_CMD_I2C_BUFRAM (01h) command when the reading size is larger thane
	if ((wlen <= 41) && (*rlen <= 41))
	{
		mbxcmd = ITE_MBX_CMD_I2C_WR_RD;
	}
	//-------------------------------------------------------------------------
	//  01h: ITE_MBX_CMD_I2C_BUFRAM
	//-------------------------------------------------------------------------
	else if ((wlen <= 41) && (*rlen <= 256))
	{
		mbxcmd = ITE_MBX_CMD_I2C_BUFRAM;
	}
	//-------------------------------------------------------------------------
	//  0Dh: ITE_MBX_CMD_I2C_MASS_WR_RD
	//-------------------------------------------------------------------------
	else
	{
		mbxcmd = ITE_MBX_CMD_I2C_MASS_WR_RD;
	}

	res = ite_mbx_smb_trans(mbxcmd, did, sla, 0x00, wdat, wlen, rbuf, rlen);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;

	return res;
}
	
//=============================================================================
//  ite_i2c_init
//=============================================================================
void ite_i2c_init(void)
{
	int		i;
	uint8_t	did;
	uint8_t	pin;

	ite->i2c.num = 0;
	
	for (i=0; i<ite->dt.num; i++)
	{
		if (i >= ITE_MAX_NUM_DEV)
			break;

		did = ite->dt.dev[i].did;
		pin = ite->dt.dev[i].pin;
		
		// mask: 0~7
		pin &= (ITE_MAX_NUM_I2C - 1);

		// i2c
		if ((did >= DEV_ID_SMB_OEM0 && did <= DEV_ID_I2C_OEM) ||
			(did >= DEV_ID_SMB_OEM3 && did <= DEV_ID_SMB_SML) ||
			(did >= DEV_ID_SMB_OEM4 && did <= DEV_ID_SMB_OEM5))
		{
			if (ite->i2c.num < ITE_MAX_NUM_I2C)
			{
				// idx: pin no.
				ite->i2c.dev[ pin ] = did;

				// i2c mask
				ite->i2c.mask |= (1 << pin);
				
				ite->i2c.num++;

			}
		}
	}
}
