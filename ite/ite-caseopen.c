#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"

//=============================================================================
//  ite_caseopen_read
//=============================================================================
uint8_t ite_caseopen_read(uint8_t *det)
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
	wb[1] = ITE_CASEOPEN_GET_SWITCH;	// par

	rb[0] = 1;		// rlen : 2-bytes
	rb[1] = 0;		// roff : dat0=smbus_id, dat1=fan_id

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_CASEOPEN, wb, rb);

	if (res == ITE_OK)
		*det = rb[4];
	
	return res;
}

//=============================================================================
//  ite_caseopen_write
//=============================================================================
uint8_t ite_caseopen_write(uint8_t *det)
{
	uint8_t		wb[50];
	uint8_t		rb[50];


	memset(wb, 0, sizeof(wb));
	memset(rb, 0, sizeof(rb));

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 6;							// wblen
	wb[1] = ITE_CASEOPEN_SET_SWITCH;	// par
	wb[2] = 0;							// dlen : useless
	wb[3] = 0;							// woff : dat0~
	wb[4] = 1;							// wlen : 1
	wb[5] = *det;						// dat0  : caseopen data, 0x00:disable, 0x01:enable
	
	// cmd issued
	return ite_mbx_trans(ITE_MBX_CMD_CASEOPEN, wb, rb);
}

//=============================================================================
//  ite_caseopen_read_status
//=============================================================================
uint8_t ite_caseopen_read_status(uint8_t *sts)
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
	wb[1] = ITE_CASEOPEN_GET_STATUS;	// par

	rb[0] = 1;		// rlen : 2-bytes
	rb[1] = 0;		// roff : dat0=smbus_id, dat1=fan_id

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_CASEOPEN, wb, rb);

	if (res == ITE_OK)
		*sts = rb[4] & 0x1;
	
	return res;
}

//=============================================================================
//  ite_caseopen_clear_status
//=============================================================================
uint8_t ite_caseopen_clear_status(void)
{
	uint8_t		wb[50];
	uint8_t		rb[50];


	memset(wb, 0, sizeof(wb));
	memset(rb, 0, sizeof(rb));

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 2;							// wblen
	wb[1] = ITE_CASEOPEN_CLEAN_STATUS;	// par
	
	// cmd issued
	return ite_mbx_trans(ITE_MBX_CMD_CASEOPEN, wb, rb);
}
	
//=============================================================================
//  ite_caseopen_init
//=============================================================================
void ite_caseopen_init(void)
{
	int			i;
	uint8_t		did;

	ite->caseopen.num = 0;

	if (ite->dt.num == 0)
		return;
	
	for (i=0; i<ite->dt.num; i++)
	{
		if (i >= ITE_MAX_NUM_DEV)
			break;
		
		did = ite->dt.dev[i].did;

		if (ite->caseopen.num < ITE_MAX_NUM_CASEOPEN)
		{
			if (did == DEV_ID_CASEOPEN)
			{
				ite->caseopen.dev[ ite->caseopen.num ] = did;
				ite->caseopen.num++;

				// volt mask
				ite->caseopen.mask |= (1 << (did - DEV_ID_CASEOPEN));
			}
		}
	}
}
