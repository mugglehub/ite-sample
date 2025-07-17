#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"

//=============================================================================
//  ite_smb_set_clock
//=============================================================================
uint8_t ite_smb_set_clock(uint8_t ch, uint16_t clk)
{
	uint8_t		wb[50];
	uint8_t		rb[50];


	if (ch >= ITE_MAX_NUM_SMB)
		return ITE_ERR_CH;
	
	if (ite->smb.num == 0)
		return ITE_ERR_CH;
	
	if ((ite->smb.mask & (1<<ch)) == 0)
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
	wb[1] = ite->smb.dev[ch];	// par: dev id
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
//  ite_smb_probe
//=============================================================================
uint8_t ite_smb_probe(uint8_t ch, uint8_t sla)
{
	uint8_t		res;
	uint8_t		did;
	uint8_t		rbuf[4];
	uint32_t	rlen;

	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;
	
	did =  ite->smb.dev[ ch ];
	
	// follow linux i2c tool : i2cdetect.c 
	// read byte:   60h~6Fh, A0h~BFh
	// write quick: else
	if (((sla >= 0x60) && (sla <= 0x6F)) || ((sla >= 0xA0) && (sla <= 0xBF)))		//avoid memory SPD write protect affect, and mistake to write SPD
	{
		rlen = 1;
		res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_RECV_BYTE, did, sla, 0x00, NULL, 0, rbuf, &rlen);
	}
	else
	{
		rlen = 0;
		res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_WR_QUICK, did, sla, 0x00, NULL, 0, NULL, &rlen);
	}
	
	if ((res & ITE_SMB_ERR_MASK) == 0)
		return ITE_OK;

	return ITE_ERR_NODEV;
}

//=============================================================================
//  ite_smb_read_quick
//=============================================================================
uint8_t ite_smb_read_quick(uint8_t ch, uint8_t sla)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	len;

	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];
	
	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_RD_QUICK, did, sla, 0x00, NULL, 0, NULL, &len);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;

	return ITE_ERR;
}

//=============================================================================
//  ite_smb_write_quick
//=============================================================================
uint8_t ite_smb_write_quick(uint8_t ch, uint8_t sla)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	len;

	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];
	
	fprintf(stderr, "did = %02Xh", did);
	
	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_WR_QUICK, did, sla, 0x00, NULL, 0, NULL, &len);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;

	return ITE_ERR;
}

//=============================================================================
//  ite_smb_receive_byte
//=============================================================================
uint8_t ite_smb_receive_byte(uint8_t ch, uint8_t sla, uint8_t *dat)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	len;
	
	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];
	
	len = 1;
	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_RECV_BYTE, did, sla, 0x00, NULL, 0, dat, &len);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;
	
	return ITE_ERR;
}

//=============================================================================
//  ite_smb_send_byte
//=============================================================================
uint8_t ite_smb_send_byte(uint8_t ch, uint8_t sla, uint8_t *dat)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	len;
	
	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];
	
	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_SEND_BYTE, did, sla, 0x00, dat, 1, NULL, &len);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;
	
	return ITE_ERR;
}

//=============================================================================
//  ite_smb_read_byte
//=============================================================================
uint8_t ite_smb_read_byte(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	len;
	
	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];

	len = 1;
	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_RD_BYTE, did, sla, cmd, NULL, 0, dat, &len);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;
	
	return ITE_ERR;
}

//=============================================================================
//  IteSmbWriteByte
//=============================================================================
uint8_t ite_smb_write_byte(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	len;
	
	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];

	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_WR_BYTE, did, sla, cmd, dat, 1, NULL, &len);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;
	
	return ITE_ERR;
}

//=============================================================================
//  ite_smb_read_word
//=============================================================================
uint8_t ite_smb_read_word(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	len;
	
	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];

	len = 2;
	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_RD_WORD, did, sla, cmd, NULL, 0, dat, &len);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;
	
	return ITE_ERR;
}

//=============================================================================
//  ite_smb_write_word
//=============================================================================
uint8_t ite_smb_write_word(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	len;

	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];

	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_WR_WORD, did, sla, cmd, dat, 2, NULL, &len);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;
	
	return ITE_ERR;
}

//=============================================================================
//  ite_smb_block_read
//=============================================================================
uint8_t ite_smb_block_read(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat, uint8_t *blen)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	rlen;

	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];

	rlen = *blen;
	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_RD_BLOCK, did, sla, cmd, NULL, 0, dat, &rlen);
	*blen = (uint8_t)rlen;
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;
	
	return ITE_ERR;
}

//=============================================================================
//  ite_smb_block_write
//=============================================================================
uint8_t ite_smb_block_write(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat, uint8_t blen)
{
	uint8_t		res;
	uint8_t		did;
	uint32_t	rlen;

	if (!(ite->smb.mask & (1 << ch)))
		return ITE_ERR_CH;

	did =  ite->smb.dev[ ch ];

	rlen = blen;
	res = ite_mbx_smb_trans(ITE_MBX_CMD_SMB_WR_BLOCK, did, sla, cmd, dat, blen, NULL, &rlen);
	
	if (res == ITE_SMB_DONE)
		return ITE_OK;
	
	return ITE_ERR;
}

//=============================================================================
//  ite_smb_init
//=============================================================================
void ite_smb_init(void)
{
	int		i;
	uint8_t	did;
	uint8_t	pin;

	ite->smb.num = 0;
	
	for (i=0; i<ite->dt.num; i++)
	{
		if (i >= ITE_MAX_NUM_DEV)
			break;

		did = ite->dt.dev[i].did;
		pin = ite->dt.dev[i].pin;
		
		// mask: 0~7
		pin &= (ITE_MAX_NUM_SMB - 1);

		// smb
		if ((did >= DEV_ID_SMB_OEM0 && did <= DEV_ID_I2C_OEM) ||
			(did >= DEV_ID_SMB_OEM3 && did <= DEV_ID_SMB_SML) ||
			(did >= DEV_ID_SMB_OEM4 && did <= DEV_ID_SMB_OEM5))
		{
			if (ite->smb.num < ITE_MAX_NUM_SMB)
			{
				// idx: pin no.
				ite->smb.dev[ pin ] = did;

				// smb mask
				ite->smb.mask |= (1 << pin);
				
				ite->smb.num++;

			}
		}
	}
}
