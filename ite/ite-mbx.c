#include <stdio.h>
#include <stdint.h>
#include <sys/io.h>

#include "ite.h"
#include "ite-mbx.h"

//=============================================================================
//  ite_mbx_write
//=============================================================================
uint8_t ite_mbx_write(uint8_t off, uint8_t dat)
{
	uint8_t		res;
	ite_port_t	*port = &ite->port;

	// ibf=0
	res = ite_wait_ibe();
	ITE_ERR_CHECK(res);

	// clear OBF
	inb(port->dat);
		
	// cmd
	outb(ITE_MBX_WR_BASE + off, port->cmd);

	// ibf=0
	res = ite_wait_ibe();
	ITE_ERR_CHECK(res);
	
	// dat
	outb(dat, port->dat);

	return ITE_OK;
}

//=============================================================================
//  ite_hmbx_write
//=============================================================================
uint8_t ite_hmbx_write(uint8_t off, uint8_t dat)
{
	ite_port_t	*port = &ite->port;
	
	// clear obf
	inb(port->dat);

	// cmd
	outb(ITE_MBX_ITE + off, port->cmd);

	// dat
	outb(dat, port->dat);

	return ITE_OK;
}

//=============================================================================
//  ite_hmbx_write0
//=============================================================================
uint8_t ite_hmbx_write0(uint8_t off, uint8_t dat)
{
	ite_port_t	*port = &ite->port;
	
	// clear obf
	inb(port->dat);

	// cmd
	outb(ITE_MBX_ITE_0 + off, port->cmd);

	// dat
	outb(dat, port->dat);

	return ITE_OK;
}

//=============================================================================
//  ite_mbx_read
//=============================================================================
uint8_t ite_mbx_read(uint8_t off, uint8_t *dat)
{
	uint8_t		res;
	ite_port_t	*port = &ite->port;

	// ibf=0
	res = ite_wait_ibe();
	ITE_ERR_CHECK(res);

	// clear OBF
	inb(port->dat);
		
	// cmd
	outb(ITE_MBX_RD_BASE + off, port->cmd);

	// obf=1
	res = ite_wait_obf();
	ITE_ERR_CHECK(res);

	// dat
	*dat = inb(port->dat);

	return ITE_OK;
}

//=============================================================================
//  ite_hmbx_read
//=============================================================================
uint8_t ite_hmbx_read(uint8_t off, uint8_t *dat)
{
	ite_port_t	*port = &ite->port;
	
	// clear OBF
	inb(port->dat);
		
	// cmd
	outb(ITE_MBX_ITE + off, port->cmd);

	// dat
	*dat = inb(port->dat);

	return ITE_OK;
}

//=============================================================================
//  ite_mbx_wait
//=============================================================================
uint8_t ite_mbx_wait(void)
{
	int			i;
	uint8_t		res;
	ite_port_t	*port = &ite->port;

	for (i=0; i<ITE_MBX_BUSY_RETRIES; i++)
	{
		// ibf=0
		res = ite_wait_ibe();
		ITE_ERR_CHECK(res);

		// clear OBF
		inb(port->dat);
		
		// cmd
		outb(ITE_MBX_RD_CMD, port->cmd);

		// obf=1
		res = ite_wait_obf();
		ITE_ERR_CHECK(res);

		// dat
		if (inb(port->dat) == 0)
			return ITE_OK;
	}
		
	return ITE_ERR_MBX_TMOUT;
}

//=============================================================================
//  ite_hmbx_wait
//=============================================================================
uint8_t ite_hmbx_wait(void)
{
	int			i;
	ite_port_t	*port = &ite->port;

	for (i=0; i<ITE_MBX_BUSY_RETRIES; i++)
	{
		// clear OBF
		inb(port->dat);
		
		// cmd : hw mbox
		outb(ITE_MBX_ITE_CMD, port->cmd);
		
		// dat
		if (inb(port->dat) == 0)
			return ITE_OK;

		//delay(1);
	}
		
	return ITE_ERR_TMOUT_MBX;
}

//=============================================================================
//  ite_mbx_wait_cmd_done
//=============================================================================
uint8_t ite_mbx_wait_cmd_done(void)
{
	uint8_t		res;
	uint8_t		done;
	uint32_t	retries = ITE_MBX_CMD_DONE_RETRIES;

	while (retries--)
	{
		// read : cmd byte
		res = ite->mbx.read(ITE_MBX_OFF_CMD, &done);
		
		if (res == ITE_OK)
		{
			if (done == 0)
				return ITE_OK;
		}
	}

	return ITE_ERR_MBX_TMOUT;
}

//=============================================================================
//  ite_mbx_clear
//=============================================================================
uint8_t ite_mbx_clear(void)
{
	uint8_t		res;

	// wait cmd done
	res = ite_mbx_wait_cmd_done();
	ITE_ERR_CHECK(res);

	// cmd issued : clear mbx
	res = ite->mbx.write(ITE_MBX_OFF_CMD, ITE_MBX_CMD_CLEAR);
	
	ITE_ERR_CHECK(res);

	// cmd done : clear mbx
	res = ite_mbx_wait_cmd_done();
	ITE_ERR_CHECK(res);

	return ITE_OK;
}

//=============================================================================
//  ite_mbx_read_buf_ram
//=============================================================================
uint8_t ite_mbx_read_buf_ram(uint8_t *rb, int len)
{
	int			i, j, k;
	int			nbank;
	int			bcnt;
	uint8_t		res;
	uint8_t		sts;

	// clear mbx
	res = ite_mbx_clear();
	ITE_ERR_CHECK(res);

	if (len > 255)
	{
		len = 256;	// max size of buffer ram = 256 bytes
	}
	else if (len == 0)
	{
		return ITE_ERR_SIZE;
	}

	// num of bank
	nbank = (len + BUFRAM_PAGE_SZ - 1) / BUFRAM_PAGE_SZ;

	for (j=0, k=0; j<nbank; j++)
	{
		// par
		res = ite->mbx.write(ITE_MBX_OFF_PAR, (uint8_t)j);	// par : bank no
		ITE_ERR_CHECK(res);
	
		// cmd : issue buffer ram read command (C1h)
		res = ite->mbx.write(ITE_MBX_OFF_CMD, ITE_MBX_CMD_BUFRAM_RD);
		ITE_ERR_CHECK(res);

		// cmd done : cmd
		res = ite_mbx_wait_cmd_done();
		ITE_ERR_CHECK(res);

		// sts : read
		res = ite->mbx.read(ITE_MBX_OFF_STS, &sts);
		ITE_ERR_CHECK(res);

		if (sts != 0x01)
		{
			rb[1] = sts;
			return ITE_ERR_MBX;
		}

		if (len >= 32)
			bcnt = 32;
		else
			bcnt = len;	// rest of buffer

		for (i=0; i<bcnt; i++, k++)
		{
			res = ite->mbx.read(ITE_MBX_OFF_DAT + (uint8_t)i, &rb[k]);
			ITE_ERR_CHECK(res);
		}

		len -= bcnt;
	}

	return ITE_OK;
}

//=============================================================================
//  ite_mbx_write_buf_ram
//=============================================================================
uint8_t ite_mbx_write_buf_ram(uint8_t *wb, int len)
{
	int			i, b;
	int			nbank;
	int			bcnt;
	int			boff;
	uint8_t		res;
	uint8_t		sts;



	// brsz : buffer ram size
	if (len > ite->mbx.brsz)
		len = ite->mbx.brsz;
	else if (len == 0)
		return ITE_ERR_SIZE;

	// nbank : num of bank, bank size = 32 due to mailbox buffer size is 44-bytes
	nbank = (len + 32 - 1) / 32;

	// go through all the bank
	for (b=0; b<nbank; b++)
	{
		// clear mbx
		res = ite_mbx_clear();
		ITE_ERR_CHECK(res);

		// bank offset
		boff = b * 32;

		if (len >= 32)
			bcnt = 32;
		else
			bcnt = len;	// rest of buffer

		// par
		res = ite->mbx.write(ITE_MBX_OFF_PAR, (uint8_t)b);	// par : bank no
		ITE_ERR_CHECK(res);
	
		
		for (i=0; i<bcnt; i++)
		{
			res = ite->mbx.write(ITE_MBX_OFF_DAT + (uint8_t)i, wb[boff + i]);
			ITE_ERR_CHECK(res);
		}

		// cmd : issue buffer ram write command (C2h)
		res = ite->mbx.write(ITE_MBX_OFF_CMD, ITE_MBX_CMD_BUFRAM_WR);
		ITE_ERR_CHECK(res);

		// cmd done : cmd
		res = ite_mbx_wait_cmd_done();
		ITE_ERR_CHECK(res);

		// sts : read
		res = ite->mbx.read(ITE_MBX_OFF_STS, &sts);
		ITE_ERR_CHECK(res);

		if (sts != 0x01)
		{
			//sts  res[7:6]  desc
			//-----------------------------------------
			// 0 :  00       fail
			// 1 :  -        success
			// 2 :  01       undefined item
			// 3 :  10       device id is not defined
			// 4 :  11       hw pin type error
			//-----------------------------------------
			return ITE_ERR_MBX;	// fail
			
		}

		len -= bcnt;
	}

	return ITE_OK;
}

//=============================================================================
//  ite_mbx_trans
//=============================================================================
uint8_t ite_mbx_trans(uint8_t cmd, uint8_t *wb, uint8_t *rb)
{
	int			i;
	uint8_t		sts;
	uint8_t		res;
	uint8_t		did, dlen, doff;
	uint8_t		len;
	uint8_t		roff, rlen;
	uint8_t		woff, wlen;
	uint8_t		soff, slen;
	uint8_t		*dp;

	// clear mbx
	res = ite_mbx_clear();
	ITE_ERR_CHECK(res);

	//-------------------------------------------------------------------------
	//  write buffer
	//-------------------------------------------------------------------------
	//      <0>   <1>  <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par, dlen, woff, wlen  dat0, dat1...........dat43, dat44
	//-------------------------------------------------------------------------
	// len   : size of wbuf
	// par   : parameter (did or others)
	// dlen  : data length (@ dat44 or dat2C) for some mbx cmd
	// woff  : offset address of datx for writing
	// wlen  : length of datx for writing

	len  = wb[0];
	did  = wb[1];
	dlen = wb[2];
	woff = wb[3];
	wlen = wb[4];

	//-----------------------------------------------------
	//  par
	//-----------------------------------------------------
	if (len > 1)
	{
		res = ite->mbx.write(ITE_MBX_OFF_PAR, did);
		ITE_ERR_CHECK(res);
	}

	//-----------------------------------------------------
	//  dat0~ : before dlen
	//-----------------------------------------------------
	if ((len > 5) && (woff > 0))
	{
		//-----------------------------------------------------
		//  dat(0) ~ dat(woff)
		//-----------------------------------------------------
		dp	 = &wb[5];	// dat0
		doff = ITE_MBX_OFF_DAT0;

		for (i=0; i<(int)woff; i++)
		{
			res = ite->mbx.write(doff + (uint8_t)i, *dp++);
			ITE_ERR_CHECK(res);
		}
	}

	//-----------------------------------------------------
	//  dlen: dat44 or dat2C
	//-----------------------------------------------------
	if ((len > 2) && (dlen > 0))
	{
		res = ite->mbx.write(ITE_MBX_OFF_LEN, dlen);
		ITE_ERR_CHECK(res);
	}
	
	//-----------------------------------------------------
	//  datx: ...
	//-----------------------------------------------------
	if ((len == (5 + woff + wlen)) && (wlen > 0))
	{
		// point to datx (where x = woff)
		dp   = &wb[(5 + woff)];	
		doff = ITE_MBX_OFF_DAT0 + woff;	

		for (i=0; i<(int)wlen; i++)
		{
			res = ite->mbx.write(doff + (uint8_t)i, *dp++);
			ITE_ERR_CHECK(res);
		}
	}

	// cmd
	res = ite->mbx.write(ITE_MBX_OFF_CMD, cmd);
	ITE_ERR_CHECK(res);

	// cmd done : cmd
	res = ite_mbx_wait_cmd_done();
	ITE_ERR_CHECK(res);

	// sts : read
	res = ite->mbx.read(ITE_MBX_OFF_STS, &sts);
	ITE_ERR_CHECK(res);

	if (sts != 0x01)
	{
		rb[1] = sts;

		// rb[1] = status code

		//sts  res[7:6]  desc
		//-----------------------------------------
		// 0 :  00       fail
		// 1 :  -        success
		// 2 :  01       undefined item
		// 3 :  10       device id is not defined
		// 4 :  11       hw pin type error
		//-----------------------------------------
		if (sts == 0)
		{
			return ITE_ERR_MBX;	// fail
		}
		else
		{
			// sts=1 : skip status of success 
			sts--;
			sts &= 0x03;
			
			return (ITE_ERR_MBX | (sts<<6));
		}
	}

	//-------------------------------------------------------------------------
	//  read buffer
	//-------------------------------------------------------------------------
	// rb   <0>   <1>   <2>   <3>   <4>   <5> ..... <48>
	// rb : rlen, roff  slen, soff, dat0, dat1......dat44
	//-------------------------------------------------------------------------


	// offset address
	// 0 = read from dat0, dat1, dat2....
	// 1 = read from dat1, dat2...
	// 2 = read from dat2, dat3...
	rlen = rb[0];
	roff = rb[1];
	slen = rb[2];
	soff = rb[3];

	if (rlen)
	{
		doff = ITE_MBX_OFF_DAT0 + roff;

		for (i=0; i<rlen; i++)
		{
			//-------------------------------------------------------------------------
			// rb   <0>   <1>   <2>   <3>   <4>   <5> ..... <48>
			// rb : rlen, roff  slen, soff, dat0, dat1......dat44
			//-------------------------------------------------------------------------
			res = ite->mbx.read(doff + (uint8_t)i, &rb[i+4+roff]);	// 4:dat0
			ITE_ERR_CHECK(res);
		}
	}

	// structure len : dat2c or dat44
	if (slen)
	{
		doff = ITE_MBX_OFF_DAT0 + soff;

		// slen : overwrite it!
		res = ite->mbx.read(ITE_MBX_OFF_LEN, &slen);
		ITE_ERR_CHECK(res);
			
		rb[2] = slen;	// overwrite

		for (i=0; i<slen; i++)
		{
			res = ite->mbx.read(doff + (uint8_t)i, &rb[i+4+soff]);	// 4:dat0
			ITE_ERR_CHECK(res);
		}
	}

	return ITE_OK;
}


//=============================================================================
//  ite_mbx_smb_trans
//=============================================================================
uint8_t ite_mbx_smb_trans(uint8_t cmd, uint8_t did, uint8_t sla, uint8_t smbcmd, uint8_t *wb, uint32_t wlen, uint8_t *rb, uint32_t *rlen)
{
	uint32_t	i;
	uint32_t	len;
	uint8_t		sts;
	uint8_t		res;

	//-------------------------------------------------------------------------
	//  check mbx command : support PEC protocol
	//-------------------------------------------------------------------------
	// 01h : ITE_MBX_CMD_BUF_I2C_RD
	// 02h : ITE_MBX_CMD_SMB_WR_QUICK
	// 03h : ITE_MBX_CMD_SMB_RD_QUICK
	// 04h : ITE_MBX_CMD_SMB_SEND_BYTE
	// 05h : ITE_MBX_CMD_SMB_RECV_BYTE
	// 06h : ITE_MBX_CMD_SMB_WR_BYTE
	// 07h : ITE_MBX_CMD_SMB_RD_BYTE
	// 08h : ITE_MBX_CMD_SMB_WR_WORD
	// 09h : ITE_MBX_CMD_SMB_RD_WORD
	// 0Ah : ITE_MBX_CMD_SMB_WR_BLOCK
	// 0Bh : ITE_MBX_CMD_SMB_RD_BLOCK
	// 0Eh : ITE_MBX_CMD_I2C_RD_WR
	// 0Fh : ITE_MBX_CMD_I2C_WR_RD
	//
	// 84h : ITE_MBX_CMD_SMB_SEND_BYTE_PEC
	// 85h : ITE_MBX_CMD_SMB_RECV_BYTE_PEC
	// 86h : ITE_MBX_CMD_SMB_WR_BYTE_PEC
	// 87h : ITE_MBX_CMD_SMB_RD_BYTE_PEC
	// 88h : ITE_MBX_CMD_SMB_WR_WORD_PEC
	// 89h : ITE_MBX_CMD_SMB_RD_WORD_PEC
	// 8Ah : ITE_MBX_CMD_SMB_WR_BLOCK_PEC
	// 8Bh : ITE_MBX_CMD_SMB_RD_BLOCK_PEC
	//
	// C4h : ITE_MBX_CMD_I2C_BUFRAM_W
	//-------------------------------------------------------------------------
	if ((((cmd & 0x7F) < 0x01) || ((cmd & 0x7F) > 0x0F) || (cmd == 0x0C)) && (cmd != 0xC4))
	{
		return ITE_SMB_ERR_MBX_CMD;
	}

	//-------------------------------------------------------------------------
	//  0Dh: ITE_MBX_CMD_I2C_MASS_WR_RD
	//-------------------------------------------------------------------------
	//  buffer ram handler : it must be done before i2c transaction
	if (cmd == ITE_MBX_CMD_I2C_MASS_WR_RD)
	{
		// buffer ram writing
		if (wlen > 0)
		{
			res = ite_mbx_write_buf_ram(wb, wlen);
			ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);
		}
	}

	//-------------------------------------------------------------------------
	//  clear mbx
	//-------------------------------------------------------------------------
	res = ite_mbx_clear();
	ITE_ERR_CHECK(res);


	//-------------------------------------------------------------------------
	//  mbx buffer layout
	//-------------------------------------------------------------------------
	//  0 : cmd  : mbxcmd   mailbox command
	//  1 : par  : did      device id, e.g. 0x2B
	//  2 : dat0 : sla      slave address, e.g. 0xA0
	//  3 : dat1 : smbcmd   SMBus command code
	//  4 : dat2 : rlen		read data length
	//  5 : dat3 : wlen     write data length
	//  6 : dat4 : data 0   start of data buffer
	//  7 : dat5 : data 1
	//  8 : dat6 : data 2
	// ..   ................................. data buffer size = 41
	// 46 : dat44: data 40  end of data buffer
	// 47 : dat45: size of structure (for specific device)
	//-------------------------------------------------------------------------

	res = ite->mbx.write(ITE_MBX_OFF_PAR, did);				// par : did
	ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_PAR);

	res = ite->mbx.write(ITE_MBX_OFF_DAT0, sla);			// dat0 : sla
	ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_SLA);
	
	res = ite->mbx.write(ITE_MBX_OFF_DAT1, smbcmd);			// dat1 : smbcmd
	ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_SMBCMD);

	// TBD : marked ITE_SMB_ERR_CHECK, follow ECTEST way, but it's dirty.
	// weird ? : sometimes checking return value, sometimes not checking ? why ?


	//=========================================================================
	//  writing data before start condition
	//=========================================================================
	switch (cmd)
	{
	//-------------------------------------------------------------------------
	//  write word
	//-------------------------------------------------------------------------
	//  08h: ITE_MBX_CMD_SMB_WR_WORD
	//  88h: ITE_MBX_CMD_SMB_WR_WORD_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_WR_WORD:
	case ITE_MBX_CMD_SMB_WR_WORD_PEC:

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 5, wb[1]);		// dat5 : MSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 4, wb[0]);		// dat4 : LSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		break;

	//-------------------------------------------------------------------------
	//  send byte
	//-------------------------------------------------------------------------
	//  04h: ITE_MBX_CMD_SMB_SEND_BYTE
	//  84h: ITE_MBX_CMD_SMB_SEND_BYTE_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_SEND_BYTE:
	case ITE_MBX_CMD_SMB_SEND_BYTE_PEC:
	
		// do the same with "write byte"
	
	//-------------------------------------------------------------------------
	//  write byte
	//-------------------------------------------------------------------------
	//  06h: ITE_MBX_CMD_SMB_WR_BYTE
	//  86h: ITE_MBX_CMD_SMB_WR_BYTE_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_WR_BYTE:
	case ITE_MBX_CMD_SMB_WR_BYTE_PEC:

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 4, wb[0]);		// dat4 : LSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		break;

	//-------------------------------------------------------------------------
	//  write quick
	//-------------------------------------------------------------------------
	//  02h: ITE_MBX_CMD_SMB_WR_QUICK
	//  82h: ITE_MBX_CMD_SMB_WR_QUICK_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_WR_QUICK:
	case ITE_MBX_CMD_SMB_WR_QUICK_PEC:
		break;
		
	//-------------------------------------------------------------------------
	//  read quick
	//-------------------------------------------------------------------------
	//  03h: ITE_MBX_CMD_SMB_RD_QUICK
	//  83h: ITE_MBX_CMD_SMB_RD_QUICK_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_RD_QUICK:
	case ITE_MBX_CMD_SMB_RD_QUICK_PEC:
		break;

	//-------------------------------------------------------------------------
	//  receive byte
	//-------------------------------------------------------------------------
	//  05h: ITE_MBX_CMD_SMB_RECV_BYTE
	//  85h: ITE_MBX_CMD_SMB_RECV_BYTE_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_RECV_BYTE:
	case ITE_MBX_CMD_SMB_RECV_BYTE_PEC:
		break;

	//-------------------------------------------------------------------------
	//  read byte
	//-------------------------------------------------------------------------
	//  07h: ITE_MBX_CMD_SMB_RD_BYTE
	//  87h: ITE_MBX_CMD_SMB_RD_BYTE_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_RD_BYTE:
	case ITE_MBX_CMD_SMB_RD_BYTE_PEC:
		break;

	//-------------------------------------------------------------------------
	//  read word
	//-------------------------------------------------------------------------
	//  09h: ITE_MBX_CMD_SMB_RD_WORD
	//  89h: ITE_MBX_CMD_SMB_RD_WORD_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_RD_WORD:
	case ITE_MBX_CMD_SMB_RD_WORD_PEC:
		break;

	//-------------------------------------------------------------------------
	//  block read
	//-------------------------------------------------------------------------
	//  0Bh: ITE_MBX_CMD_SMB_WR_BLOCK_PEC
	//  8Bh: ITE_MBX_CMD_SMB_RD_BLOCK_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_RD_BLOCK:
	case ITE_MBX_CMD_SMB_RD_BLOCK_PEC:
		break;

	//-------------------------------------------------------------------------
	//  01h: ITE_MBX_CMD_I2C_BUFRAM
	//       I2C Write-Read : write buffer (41-bytes) read buffer (256-bytes)
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_I2C_BUFRAM:

		//---------------------------------------------------------------------
		//  issue clear buffer ram command (C0h)
		//---------------------------------------------------------------------
		res = ite->mbx.write(ITE_MBX_OFF_CMD, ITE_MBX_CMD_BUFRAM_CLR);
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_BUF_CLEAR);

		// cmd done : cmd
		res = ite_mbx_wait_cmd_done();
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_CMD_DONE);

		// TBD : does it need to check status (ITE_MBX_OFF_STS) ?
		
		// rlen
		len = *rlen;
		// buffer ram size = 256, but...why > 255 ?
		// because the size of dat2 = uint8_t, only 00h~FFh, it's mismatch of data size.
		
		if (len > 255)
			len = 0;	// 0x00 = 256 bytes

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 2, (uint8_t)len);	// rlen
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		// wlen
		len = wlen;
		if (len > 41)	// mbox buffer of data size = 41 : dat04h~dat2Ch
			len = 41;

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 3, (uint8_t)len);	// rlen
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		// wb
		if (len > 0)
		{
			for (i=0; i<len; i++)
			{
				res = ite->mbx.write(ITE_MBX_OFF_DAT + 4 + (uint8_t)i, wb[i]);
				ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);
			}
		}

		break;

	//-------------------------------------------------------------------------
	//  C4h: ITE_MBX_CMD_I2C_BUFRAM_W
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_I2C_BUFRAM_W:
		
		// wlen
		len = wlen;
		if (len > 41)	// mbox buffer of data size = 41 : dat04h~dat2Ch
			len = 41;
		
		res = ite->mbx.write(ITE_MBX_OFF_DAT + 3,  wb[0]);	// wlen
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 4,  wb[1]);	// dat4 :MSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);
		
		res = ite->mbx.write(ITE_MBX_OFF_DAT + 5,  wb[0]);	// dat5 : LSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		break;

	//-------------------------------------------------------------------------
	//  0Dh: ITE_MBX_CMD_I2C_MASS_WR_RD
	//       I2C Write-Read : write buffer (256-bytes) read buffer (256-bytes) in IT8528
	//       I2C Write-Read : write buffer (320-bytes) read buffer (320-bytes) in IT5121
	//       I2C Write-Read : write buffer (320-bytes) read buffer (320-bytes) in IT5782
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_I2C_MASS_WR_RD:

		// buffer ram writing is done!

		// cmd performed must be after buffer ram writing,
		// because the buffer ram writing will destroy the parameters.

		// rlen
		len = *rlen;

		// brsz : buffer ram size
		if (len > ite->mbx.brsz)
			len = ite->mbx.brsz;

		// wlen
		res = ite->mbx.write(ITE_MBX_OFF_DAT + 2, (uint8_t)(len & 0xFF));		//dat2 : rlen LSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);
		
		res = ite->mbx.write(ITE_MBX_OFF_DAT + 3, (uint8_t)(wlen&0xFF));		//dat3 : wlen LSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 4, (uint8_t)((len>>8) & 0xFF));	//dat4 : rlen MSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);
		
		res = ite->mbx.write(ITE_MBX_OFF_DAT + 5, (uint8_t)((wlen>>8)&0xFF));		//dat5 : wlen MSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		break;

	//-------------------------------------------------------------------------
	//  I2C : read-write, write-read
	//-------------------------------------------------------------------------
	//  0Eh: ITE_MBX_CMD_I2C_RD_WR
	//
	//  0Fh: ITE_MBX_CMD_I2C_WR_RD
	//       I2C Write-Read : write buffer (41-bytes) read buffer (41-bytes)
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_I2C_RD_WR:
	case ITE_MBX_CMD_I2C_WR_RD:

		// rlen
		len = *rlen;
		if (len > 41)
			len = 41;

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 2, (uint8_t)len);
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		// wlen
		len = wlen;
		if (len > 41)
			len = 41;

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 3, (uint8_t)len);
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		// wb
		for (i=0; i<len; i++)
		{
			res = ite->mbx.write(ITE_MBX_OFF_DAT + 4 + (uint8_t)i, wb[i]);
			ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);
		}

		break;

	//-------------------------------------------------------------------------
	//  block write
	//-------------------------------------------------------------------------
	//  0Ah: ITE_MBX_CMD_SMB_WR_BLOCK_PEC
	//  8Ah: ITE_MBX_CMD_SMB_RD_BLOCK_PEC
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_WR_BLOCK:
	case ITE_MBX_CMD_SMB_WR_BLOCK_PEC:

		// wlen
		len = wlen;
		if (len > 41)
			len = 41;

		res = ite->mbx.write(ITE_MBX_OFF_DAT + 3, (uint8_t)len);
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);

		// wb
		for (i=0; i<len; i++)
		{
			res = ite->mbx.write(ITE_MBX_OFF_DAT + 4 + (uint8_t)i, wb[i]);
			ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_WRITE);
		}

		break;
	}

	//=========================================================================
	//  start condition
	//=========================================================================
	res = ite->mbx.write(ITE_MBX_OFF_CMD, cmd);
	ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_CMD);

	// cmd done : cmd
	res = ite_mbx_wait_cmd_done();
	ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_CMD_DONE);
		
	// sts : read
	res = ite->mbx.read(ITE_MBX_OFF_STS, &sts);
	ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_STS);

	if (sts != ITE_SMB_DONE)
	{
		return sts;
	}


	//=========================================================================
	//  reading data after stop condition
	//=========================================================================
	switch (cmd)
	{
	//-------------------------------------------------------------------------
	//  02h: ITE_MBX_CMD_SMB_WR_QUICK		write quick
	//  03h: ITE_MBX_CMD_SMB_RD_QUICK		read quick
	//  04h: ITE_MBX_CMD_SMB_SEND_BYTE		send byte
	//  06h: ITE_MBX_CMD_SMB_WR_BYTE		write byte
	//  08h: ITE_MBX_CMD_SMB_WR_WORD		write word
	//  0Ah: ITE_MBX_CMD_SMB_WR_BLOCK		block write
	//  84h: ITE_MBX_CMD_SMB_SEND_BYTE_PEC	PEC: send byte 
	//  86h: ITE_MBX_CMD_SMB_WR_BYTE_PEC	PEC: write byte
	//  88h: ITE_MBX_CMD_SMB_WR_WORD_PEC	PEC: write word
	//  8Ah: ITE_MBX_CMD_SMB_WR_BLOCK_PEC	PEC: block write
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_WR_QUICK:
	case ITE_MBX_CMD_SMB_RD_QUICK:
	case ITE_MBX_CMD_SMB_SEND_BYTE:
	case ITE_MBX_CMD_SMB_WR_BYTE:
	case ITE_MBX_CMD_SMB_WR_WORD:
	case ITE_MBX_CMD_SMB_WR_BLOCK:
	case ITE_MBX_CMD_SMB_SEND_BYTE_PEC:
	case ITE_MBX_CMD_SMB_WR_BYTE_PEC:
	case ITE_MBX_CMD_SMB_WR_WORD_PEC:
	case ITE_MBX_CMD_SMB_WR_BLOCK_PEC:
		break;

	//-------------------------------------------------------------------------
	//	09h: ITE_MBX_CMD_SMB_RD_WORD		read word
	//	89h: ITE_MBX_CMD_SMB_RD_WORD_PEC	PEC: read word
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_RD_WORD:
	case ITE_MBX_CMD_SMB_RD_WORD_PEC:

		res = ite->mbx.read(ITE_MBX_OFF_DAT + 5, &rb[1]);	// MSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_READ);

		res = ite->mbx.read(ITE_MBX_OFF_DAT + 4, &rb[0]);	// LSB
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_READ);

		*rlen = 2;	// word
		break;

	//-------------------------------------------------------------------------
	//  05h: ITE_MBX_CMD_SMB_RECV_BYTE			receive byte
	//	85h: ITE_MBX_CMD_SMB_RECV_BYTE_PEC		PEC: receive byte
	//  07h: ITE_MBX_CMD_SMB_RD_BYTE			read byte
	//	87h: ITE_MBX_CMD_SMB_RD_BYTE_PEC		PEC: read byte
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_RECV_BYTE:
	case ITE_MBX_CMD_SMB_RECV_BYTE_PEC:
	case ITE_MBX_CMD_SMB_RD_BYTE:
	case ITE_MBX_CMD_SMB_RD_BYTE_PEC:

		res = ite->mbx.read(ITE_MBX_OFF_DAT + 4, &rb[0]);
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_READ);

		*rlen = 1;	// byte
		break;

	//-------------------------------------------------------------------------
	//  01h: ITE_MBX_CMD_I2C_BUFRAM		buffer ram read-write
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_I2C_BUFRAM:

		len = *rlen;
		if (len > 255)
			len = 256;	// max size of buffer ram = 256 bytes

		res = ite_mbx_read_buf_ram(rb, len);
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_BUF_READ);

		break;

	//-------------------------------------------------------------------------
	//  0Eh: ITE_MBX_CMD_I2C_RD_WR		I2C: read-write
	//	0Fh: ITE_MBX_CMD_I2C_WR_RD		I2C: write-read
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_I2C_RD_WR:
	case ITE_MBX_CMD_I2C_WR_RD:

		len = *rlen;
		if (len > 41)
			len = 41;

		for (i=0; i<len; i++)
		{
			res = ite->mbx.read(ITE_MBX_OFF_DAT + 4 + (uint8_t)i, &rb[i]);
			ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_READ);
		}
		
		*rlen = len;
		break;

	//-------------------------------------------------------------------------
	//	0Bh: ITE_MBX_CMD_SMB_RD_BLOCK  			block read
	//	8Bh: ITE_MBX_CMD_SMB_RD_BLOCK_PEC		PEC: block read
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_SMB_RD_BLOCK:
	case ITE_MBX_CMD_SMB_RD_BLOCK_PEC:

		// block count
		res = ite->mbx.read(ITE_MBX_OFF_DAT + 2, &rb[0]);
		ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_READ);

		len = rb[0];
		for (i=0; i<len; i++)
		{
			res = ite->mbx.read(ITE_MBX_OFF_DAT + 4 + (uint8_t)i, &rb[i]);
			ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_DAT_READ);
		}
		*rlen = len;
		break;

	//-------------------------------------------------------------------------
	//  0Dh: ITE_MBX_CMD_I2C_MASS_WR_RD
	//-------------------------------------------------------------------------
	case ITE_MBX_CMD_I2C_MASS_WR_RD:

		len = *rlen;

		if (len > 0)
		{
			if (len > ite->mbx.brsz)
				len = ite->mbx.brsz;	// max size of buffer ram = 320 bytes(IT5121/IT5782), 256 bytes(IT8528)

			res = ite_mbx_read_buf_ram(rb, len);
			ITE_SMB_ERR_CHECK(res, ITE_SMB_ERR_MBX_BUF_READ);
		
			*rlen = (uint32_t)len;
		}
		
		break;
	}

	return ITE_OK;
}
