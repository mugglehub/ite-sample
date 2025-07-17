#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"

//=============================================================================
//  ite_ram_read
//=============================================================================
uint8_t ite_ram_read(uint8_t rt, uint8_t off, uint8_t sz, uint8_t *dat)
{
	uint8_t		res;
	uint8_t		roff;
	uint8_t		wb[50];
	uint8_t		rb[50];


	//-------------------------------------------------------------------------
	//  parameters
	//-------------------------------------------------------------------------
	// rt   : ram type (1=acpi, 2=hw, 3=ext, 6=buffer)
	// off  : offset address of ram type
	// sz   : data length for reading
	// *dat : data buffer

	// max size = 32 bytes for each time due to the size of mbx buffer is only 48-bytes (44-bytes)
	if (sz > 32 || sz == 0)
		return ITE_ERR_RAM_SIZE;

	memset(wb, 0, sizeof(wb));
	memset(rb, 0, sizeof(rb));
	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 6;		// wblen=6
	wb[1] = rt;		// par  : ec ram type
	wb[2] = sz;		// dlen : size to read to f/w
	wb[3] = 1;		// woff : dat0~
	wb[4] = 0;		// wlen : length of datx
	wb[5] = off;	// dat0 : offset address of ec ram

	rb[0] = sz;		// rlen : size to read
	rb[1] = 1;		// roff : dat1~
	roff  = rb[1];	// roff

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_ECRAM_RD, wb, rb);

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	if (res == ITE_OK)
		memcpy(dat, &rb[4+roff], sz);	// dat0~

	return res;
}

//=============================================================================
//  ite_ram_write
//=============================================================================
uint8_t ite_ram_write(uint8_t rt, uint8_t off, uint8_t sz, uint8_t *dat)
{
	int			i;
	uint8_t		res;
	uint8_t		wb[50];
	uint8_t		rb[50];


	//-------------------------------------------------------------------------
	//  parameters
	//-------------------------------------------------------------------------
	// rt   : ram type (1=acpi, 2=hw, 3=ext, 6=buffer)
	// off  : offset of ec ram
	// sz   : size of data buffer *dat
	// *dat : data buffer 

	// max size = 32 bytes for each time due to the size of mbx buffer is only 48-bytes (44-bytes)
	if (sz > 32 || sz == 0)
		return ITE_ERR_RAM_SIZE;

	memset(wb, 0, sizeof(wb));
	memset(rb, 0, sizeof(rb));

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	//-------------------------------------------------------------------------
	wb[0] = 6;		// wblen=6
	wb[1] = rt;		// par  : ec ram type
	wb[2] = sz;		// dlen : size to write for *dat
	wb[3] = 1;		// woff : dat1~
	wb[4] = sz;		// wlen : length of dat
	wb[5] = off;	// dat0 : offset of ec ram

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	//-------------------------------------------------------------------------
	// dat1 ~ : data buffer to write
	if (sz > 0)
	{
		// from dat1~  : moving data dat[] to dat1~
		for (i=0; (uint8_t)i<sz; i++)
			wb[(6 + i)] = dat[i];	// 6=dat1~ (dat0=offset of ec ram)
	}
	wb[0] += sz;	// len  += sz, add size of *dat

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_ECRAM_WR, wb, rb);

	return res;
}

