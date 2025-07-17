#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/io.h>
#include <sys/types.h>

#include "ite.h"


//=============================================================================
//  variables
//=============================================================================
ite_t	ite_ent;
ite_t	*ite;

//=============================================================================
//  ite_wait_ibe
//=============================================================================
uint8_t ite_wait_ibe(void)
{
	ite_port_t	*port = &ite->port;
	uint32_t	retries = ITE_IBF_RETRIES;

	// wait for the input buffer empty
	while (inb(port->sts) & ITE_STS_IBF)
	{
		if (--retries == 0)
			return ITE_ERR_IBF;
	}

	return ITE_OK;
}

//=============================================================================
//  ite_wait_obf
//=============================================================================
uint8_t ite_wait_obf(void)
{
	ite_port_t	*port = &ite->port;
	uint32_t	retries = ITE_OBF_RETRIES;

	// wait for the output buffer full
	while ((inb(port->sts) & ITE_STS_OBF) == 0)
	{
		if (--retries == 0)
			return ITE_ERR_OBF;
	}

	return ITE_OK;
}

//=============================================================================
//  ite_err_msg
//=============================================================================
char *ite_err_msg[0x24] =
{
	"OK",	// 0
	"OBF",
	"OBE",
	"IBF",
	"CHIP_ID",
	"VENDOR_ID",
	"FW_CHIP_ID",
	"CH",
	"NO_DEV",
	"MBX_TIMEOUT",	// 9
	// 10~15
	"", "", "", "", "", "",
	// 16~31
	"", "", "", "","", "", "", "","", "", "", "","", "", "", "",
	// 32~35
	"MBX_FAIL",
	"MBX_UNDEF_ITEM",
	"MBX_UNDEF_DID",
	"MBX_HWPIN_TYPE"
};

//=============================================================================
//  ite_err_msg_str
//=============================================================================
char *ite_err_msg_str(uint8_t errcode)
{
	// error code
	if (errcode >= 0x24)
		return "?";

	return ite_err_msg[errcode];
}

//=============================================================================
//  ite_fw_info
//=============================================================================
static uint8_t ite_fw_info(void)
{
	uint8_t		res;
	uint8_t		wb[32];
	uint8_t		rb[32];

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen, dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 0;				// wblen = 0

	rb[0] = MBX_VER_LEN;	// rlen=17: project name len = 8, version len = 9
	rb[1] = 0;				// roff=0 : dat0~

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_VER, wb, rb);

	if (res != ITE_OK)
	{
		// error reading
		memset(&ite->fw, 0xFF, sizeof(ite_fw_t));
		return res;
	}

	//----------------------------------------------------------------------------------------------------------------------------
	// rb   <0>   <1>   <2>   <3>   <4>   <5>        <11>   <12>   <13>   <14>   <15>   <16>    <17>    <18>    <19>     <20>
	// rb : rlen, roff, slen, soff, dat0, dat1....., dat7,  dat8   dat9   dat10  dat11  dat12   dat13   dat14   dat15    dat16
	//                              (Project Name       )   table  kverm  kverl  chipv  chipid  prjid   prjty   prjverm  prjverl
	//----------------------------------------------------------------------------------------------------------------------------
	
	// kernel version
	ite->fw.kver = rb[13];
	ite->fw.kver <<= 8;
	ite->fw.kver += rb[14];
	
	// project name (max len = 8)
	memcpy(ite->fw.prjname, &rb[4], 8);
	ite->fw.prjname[8] = 0;
	
	// table code
	ite->fw.tbc = rb[12];	// table code
	ite->fw.ccv = rb[15];	// chip vendor
	ite->fw.ccd = rb[16];	// chip id
	ite->fw.pid = rb[17];	// project id
	ite->fw.pty = rb[18];	// project type
	
	// project version
	ite->fw.pver = rb[19];
	ite->fw.pver <<= 8;
	ite->fw.pver += rb[20];

	return ITE_OK;
}

//=============================================================================
//  ite_exit
//=============================================================================
void ite_exit(void)
{
	if (ioperm(0x290, 16, 0))
		perror("ioperm port 290h");
	
	if (ioperm(0x62, 1, 0))
		perror("ioperm port 62h");

	if (ioperm(0x66, 1, 0))
		perror("ioperm port 66h");
}

//=============================================================================
//  ite_init
//=============================================================================
uint8_t ite_init(void)
{
	uint8_t		res;
	uint8_t		lsb;
	uint8_t		msb;

	// 290h~29Fh
	if (ioperm(0x290, 16, 1))
	{
		perror("ioperm port 290h");
		return 0xFF;
	}

	if (ioperm(0x62, 1, 1))
	{
		perror("ioperm port 62h");
		return 0xFD;
	}

	if (ioperm(0x66, 1, 1))
	{
		perror("ioperm port 66h");
		return 0xFC;
	}

	memset(&ite_ent, 0, sizeof(ite_t));
	ite = &ite_ent;

	// enter cfg
	outb(0x87, ITE_CFG_IDX);
	outb(0x87, ITE_CFG_IDX);

	// chip id
	outb(0x20, ITE_CFG_IDX);
	msb = inb(ITE_CFG_DAT);

	outb(0x21, ITE_CFG_IDX);
	lsb = inb(ITE_CFG_DAT);

	ite->hw.cid = (uint16_t)msb;
	ite->hw.cid <<= 8;
	ite->hw.cid |= (uint16_t)lsb;

	// exit cfg
	outb(0xAA, ITE_CFG_IDX);

	// vendor id
	res = acpi_ec_read(0xFA, &ite->hw.vid);
	if (res != ACPI_OK || ite->hw.vid != 'I') 
	{
		ite_exit();
		return ITE_ERR_VENDORID;
	}

	// fw chip code
	res = acpi_ec_read(0xFB, &ite->hw.fcc);
	if (res != ACPI_OK)
	{
		ite_exit();
		return ITE_ERR_FWCHIPID;
	}

	switch (ite->hw.cid)
	{
		//-----------------------------------------------------
		//  IT8528
		//-----------------------------------------------------
		case ITE_CHIPID_8528:
			if (ite->hw.fcc != 0x28)
			{
				ite_exit();
				return ITE_ERR_FWCHIPID;
			}
			else
			{
				ite->port.cmd = ITE_8528_CMD_PORT;
				ite->port.sts = ITE_8528_STS_PORT;
				ite->port.dat = ITE_8528_DAT_PORT;
			
				ite->mbx.read = ite_mbx_read;
				ite->mbx.write = ite_mbx_write;
				ite->mbx.wait = ite_mbx_wait;
			
				ite->mbx.brsz = 256;	// IT8528, IT8518
			}
			break;

		//-----------------------------------------------------
		//  IT5121
		//-----------------------------------------------------
		case ITE_CHIPID_5121:
			if (ite->hw.fcc != 0x21)
			{
				ite_exit();
				return ITE_ERR_FWCHIPID;
			}
			else
			{
				ite->port.cmd = ITE_5121_CMD_PORT;
				ite->port.sts = ITE_5121_STS_PORT;
				ite->port.dat = ITE_5121_DAT_PORT;

				ite->mbx.read = ite_hmbx_read;
				ite->mbx.write = ite_hmbx_write;
				ite->mbx.wait = ite_hmbx_wait;
			
				ite->mbx.brsz = 320;	// IT5121, IT5782
			}
			break;

		//-----------------------------------------------------
		//  IT5782
		//-----------------------------------------------------
		case ITE_CHIPID_5782:
			if (ite->hw.fcc != 0x82)
			{
				ite_exit();
				return ITE_ERR_FWCHIPID;
			}
			else
			{
				ite->port.cmd = ITE_5782_CMD_PORT;
				ite->port.sts = ITE_5782_STS_PORT;
				ite->port.dat = ITE_5782_DAT_PORT;

				ite->mbx.read = ite_hmbx_read;
				ite->mbx.write = ite_hmbx_write;
				ite->mbx.wait = ite_hmbx_wait;
			
				ite->mbx.brsz = 320;	// IT5121, IT5782
			}
			break;
			
		//-----------------------------------------------------
		//  IT8518
		//-----------------------------------------------------
		case ITE_CHIPID_8518:
			if (ite->hw.fcc != 0x18)
			{
				ite_exit();
				return ITE_ERR_FWCHIPID;
			}
			else
			{
				ite->port.cmd = ITE_8518_CMD_PORT;
				ite->port.sts = ITE_8518_STS_PORT;
				ite->port.dat = ITE_8518_DAT_PORT;

				ite->mbx.read = ite_mbx_read;
				ite->mbx.write = ite_mbx_write;
				ite->mbx.wait = ite_mbx_wait;
			
				ite->mbx.brsz = 256;	// IT8528, IT8518
			}
			break;

		default:
			ite_exit();
			return ITE_ERR_CHIPID;
	}

	// getting fw info
	res = ite_fw_info();
	ITE_ERR_CHECK(res);

	// device table
	res = ite_dt_init();
	ITE_ERR_CHECK(res);

	return ITE_OK;
}

//===========================================================================
//  swap16
//===========================================================================
uint16_t swap16(uint16_t data)
{
	uint16_t    lsb = (data >> 8) & 0x00FF;
	uint16_t    msb = (data << 8) & 0xFF00;

	return (msb | lsb);
}
