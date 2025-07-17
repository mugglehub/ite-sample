#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"
#include "ite-mbx.h"
#include "ite-dt.h"

//=============================================================================
//  ite_dev_type_name_str
//=============================================================================
char *ite_dev_type_name_str[ DEV_TYPE_TAB_NUM ] =
{
	"None",
	"GPIO",
	"OEM_GPIO",
	"ADC",
	"DAC",
	"TAC",
	"PWM",
	"SMBus",
	"Battery",
	"IRQ",
	"Function",
	"Unknown"
};

//=============================================================================
//  ite_dev_table: dynamic table
//=============================================================================
ite_dev_nm_t	ite_dev_table[ DEV_NAME_TAB_NUM ] =
{// <did>					<typ>			<name>
  {	DEV_ID_NONE,			DEV_TYPE_NONE,	"None" },				// 0x00
  {	DEV_ID_GPIO0,			DEV_TYPE_GPIO,	"GPIO0" },				// 0x10
  {	DEV_ID_GPIO1,			DEV_TYPE_GPIO,	"GPIO1" },				// 0x11
  {	DEV_ID_GPIO2,			DEV_TYPE_GPIO,	"GPIO2" },				// 0x12
  {	DEV_ID_GPIO3,			DEV_TYPE_GPIO,	"GPIO3" },				// 0x13
  {	DEV_ID_GPIO4,			DEV_TYPE_GPIO,	"GPIO4" },				// 0x14
  {	DEV_ID_GPIO5,			DEV_TYPE_GPIO,	"GPIO5" },				// 0x15
  {	DEV_ID_GPIO6,			DEV_TYPE_GPIO,	"GPIO6" },				// 0x16
  {	DEV_ID_GPIO7,			DEV_TYPE_GPIO,	"GPIO7" },				// 0x17
  {	DEV_ID_BTN0,			DEV_TYPE_GPIO,	"BTN0" },				// 0x18
  {	DEV_ID_BTN1,			DEV_TYPE_GPIO,	"BTN1" },				// 0x19
  {	DEV_ID_BTN2,			DEV_TYPE_GPIO,	"BTN2" },				// 0x1A
  {	DEV_ID_BTN3,			DEV_TYPE_GPIO,	"BTN3" },				// 0x1B
  {	DEV_ID_BTN4,			DEV_TYPE_GPIO,	"BTN4" },				// 0x1C
  {	DEV_ID_BTN5,			DEV_TYPE_GPIO,	"BTN5" },				// 0x1D
  {	DEV_ID_BTN6,			DEV_TYPE_GPIO,	"BTN6" },				// 0x1E
  {	DEV_ID_BTN7,			DEV_TYPE_GPIO,	"BTN7" },				// 0x1F
  {	DEV_ID_FAN_CPU2P,		DEV_TYPE_PWM,	"CPU_FAN_2P" },			// 0x20
  {	DEV_ID_FAN_CPU4P,		DEV_TYPE_PWM,	"CPU_FAN_4P" },			// 0x21
  {	DEV_ID_FAN_SYS02P,		DEV_TYPE_PWM,	"SYS0_FAN_2P" },		// 0x22
  {	DEV_ID_FAN_SYS04P,		DEV_TYPE_PWM,	"SYS0_FAN_4P" },		// 0x23
  {	DEV_ID_FAN_SYS12P,		DEV_TYPE_PWM,	"SYS1_FAN_2P" },		// 0x24
  {	DEV_ID_FAN_SYS14P,		DEV_TYPE_PWM,	"SYS1_FAN_4P" },		// 0x25
  {	DEV_ID_PWM_BRT0,		DEV_TYPE_PWM,	"LVDS0_BRT" },			// 0x26
  {	DEV_ID_PWM_BEEP,		DEV_TYPE_PWM,	"BEEP" },				// 0x27
  {	DEV_ID_SMB_OEM0,		DEV_TYPE_SMB,	"SMB_OEM0" },			// 0x28
  {	DEV_ID_SMB_OEM1,		DEV_TYPE_SMB,	"SMB_OEM1" },			// 0x29
  {	DEV_ID_SMB_OEM2,		DEV_TYPE_SMB,	"SMB_OEM2" },			// 0x2A
  {	DEV_ID_SMB_EE,			DEV_TYPE_SMB,	"SMB_EE" },				// 0x2B
  {	DEV_ID_SMB_THERM0,		DEV_TYPE_SMB,	"SMB_THERM0" },			// 0x2C
  {	DEV_ID_SMB_THERM1,		DEV_TYPE_SMB,	"SMB_THERM1" },			// 0x2D
  {	DEV_ID_SMB_EESEC,		DEV_TYPE_SMB,	"SMB_EE_SEC" },			// 0x2E
  {	DEV_ID_I2C_OEM,			DEV_TYPE_SMB,	"I2C_OEM" },			// 0x2F
  {	DEV_ID_DAC_SPKR,		DEV_TYPE_DAC,	"SPKR" },				// 0x30
  {	DEV_ID_SMB_EE2K,		DEV_TYPE_SMB,	"SMB_EE_2KB" },			// 0x38
  {	DEV_ID_OEM_EE,			DEV_TYPE_SMB,	"OEM_EE" },				// 0x39
  {	DEV_ID_OEM_EE2K,		DEV_TYPE_SMB,	"OEM_EE_2KB" },			// 0x3A
  {	DEV_ID_PECI,			DEV_TYPE_SMB,	"PECI" },				// 0x3B
  {	DEV_ID_SMB_OEM3,		DEV_TYPE_SMB,	"SMB_OEM3" },			// 0x3C
  {	DEV_ID_SMB_SML,			DEV_TYPE_SMB,	"SMLINK" },				// 0x3D
  {	DEV_ID_SMB_SLA,			DEV_TYPE_SMB,	"SMB_SLA_MODE" },		// 0x3E
  {	DEV_ID_SMB_EE64K,		DEV_TYPE_SMB,	"SMB_EE_64KB" },		// 0x3F
  {	DEV_ID_LED_PWR,			DEV_TYPE_GPIO,	"PWR_LED" },			// 0x40
  {	DEV_ID_LED_BATG,		DEV_TYPE_GPIO,	"BAT_LEDG" },			// 0x41
  {	DEV_ID_LED_OEM0,		DEV_TYPE_GPIO,	"OEM_LED0" },			// 0x42
  {	DEV_ID_LED_OEM1,		DEV_TYPE_GPIO,	"OEM_LED1" },			// 0x43
  {	DEV_ID_LED_OEM2,		DEV_TYPE_GPIO,	"OEM_LED2" },			// 0x44
  {	DEV_ID_LED_BATR,		DEV_TYPE_GPIO,	"BAT_LEDR" },			// 0x45
  {	DEV_ID_LED_PWM_PWR,		DEV_TYPE_GPIO,	"BAT_LED_PWM_PWR" },	// 0x46
  {	DEV_ID_LED_PWM_BAT,		DEV_TYPE_GPIO,	"BAT_LED_PWM_BAT" },	// 0x47
  {	DEV_ID_BAT0,			DEV_TYPE_BAT,	"BAT0" },				// 0x48
  {	DEV_ID_BAT1,			DEV_TYPE_BAT,	"BAT1" },				// 0x49
  {	DEV_ID_ADC_BAT_CMOS,	DEV_TYPE_ADC, 	"CMOS_BAT_V" },			// 0x50
  {	DEV_ID_ADC_BAT_CMOSX2,	DEV_TYPE_ADC, 	"CMOS_BAT_VX2" },		// 0x51
  {	DEV_ID_ADC_BAT_CMOSX10,	DEV_TYPE_ADC, 	"CMOS_BAT_VX10" },		// 0x52
  {	DEV_ID_ADC_BAT_LION,	DEV_TYPE_ADC, 	"LION_BAT_V" },			// 0x53
  {	DEV_ID_ADC_BAT_LIONX2,	DEV_TYPE_ADC, 	"LION_BAT_VX2" },		// 0x54
  {	DEV_ID_ADC_BAT_LIONX10,	DEV_TYPE_ADC, 	"LION_BAT_VX10" },		// 0x55
  {	DEV_ID_ADC_V5S0,		DEV_TYPE_ADC, 	"V5_S0_V" },			// 0x56
  {	DEV_ID_ADC_V5S0X2,		DEV_TYPE_ADC, 	"V5_S0_VX2" },			// 0x57
  {	DEV_ID_ADC_V5S0X10,		DEV_TYPE_ADC, 	"V5_S0_VX10" },			// 0x58
  {	DEV_ID_ADC_V5S5,		DEV_TYPE_ADC, 	"V5_S5_V" },			// 0x59
  {	DEV_ID_ADC_V5S5X2,		DEV_TYPE_ADC, 	"V5_S5_VX2" },			// 0x5A
  {	DEV_ID_ADC_V5S5X10,		DEV_TYPE_ADC, 	"V5_S5_VX10" },			// 0x5B
  {	DEV_ID_ADC_V33S0,		DEV_TYPE_ADC, 	"V33_S0_V" },			// 0x5C
  {	DEV_ID_ADC_V33S0X2,		DEV_TYPE_ADC, 	"V33_S0_VX2" },			// 0x5D
  {	DEV_ID_ADC_V33S0X10,	DEV_TYPE_ADC, 	"V33_S0_VX10" },		// 0x5E
  {	DEV_ID_ADC_V33S5,		DEV_TYPE_ADC, 	"V33_S5_V" },			// 0x5F
  {	DEV_ID_ADC_V33S5X2,		DEV_TYPE_ADC, 	"V33_S5_VX2" },			// 0x60
  {	DEV_ID_ADC_V33S5X10,	DEV_TYPE_ADC, 	"V33_S5_VX10" },		// 0x61
  {	DEV_ID_ADC_V12S0,		DEV_TYPE_ADC, 	"V12_S0_V" },			// 0x62
  {	DEV_ID_ADC_V12S0X2,		DEV_TYPE_ADC, 	"V12_S0_VX2" },			// 0x63
  {	DEV_ID_ADC_V12S0X10,	DEV_TYPE_ADC, 	"V12_S0_VX10" },		// 0x64
  {	DEV_ID_ADC_VCOREA,		DEV_TYPE_ADC, 	"VCORE_A_V" },			// 0x65
  {	DEV_ID_ADC_VCOREAX2,	DEV_TYPE_ADC, 	"VCORE_A_VX2" },		// 0x66
  {	DEV_ID_ADC_VCOREAX10,	DEV_TYPE_ADC, 	"VCORE_A_VX10" },		// 0x67
  {	DEV_ID_ADC_VCOREB,		DEV_TYPE_ADC, 	"VCORE_B_V" },			// 0x68
  {	DEV_ID_ADC_VCOREBX2,	DEV_TYPE_ADC, 	"VCORE_B_VX2" },		// 0x69
  {	DEV_ID_ADC_VCOREBX10,	DEV_TYPE_ADC, 	"VCORE_B_VX10" },		// 0x6A
  {	DEV_ID_ADC_DC,			DEV_TYPE_ADC, 	"DC_V" },				// 0x6B
  {	DEV_ID_ADC_DCX2,		DEV_TYPE_ADC, 	"DC_V_X2" },			// 0x6C
  {	DEV_ID_ADC_DCX10,		DEV_TYPE_ADC, 	"DC_V_X10" },			// 0x6D
  {	DEV_ID_ADC_DCSB,		DEV_TYPE_ADC, 	"DC_SB_V" },			// 0x6E
  {	DEV_ID_ADC_DCSBX2,		DEV_TYPE_ADC, 	"DC_SB_VX2" },			// 0x6F
  {	DEV_ID_ADC_DCSBX10,		DEV_TYPE_ADC, 	"DC_SB_VX10" },			// 0x70
  {	DEV_ID_ADC_DCOEM,		DEV_TYPE_ADC, 	"DC_OEM_V" },			// 0x71
  {	DEV_ID_ADC_DCOEMX2,		DEV_TYPE_ADC, 	"DC_OEM_VX2" },			// 0x72
  {	DEV_ID_ADC_DCOEMX10,	DEV_TYPE_ADC, 	"DC_OEM_VX10" },		// 0x73
  {	DEV_ID_ADC_CURR,		DEV_TYPE_ADC, 	"CURR_A" },				// 0x74
  {	DEV_ID_WDT_IRQ_TYP,		DEV_TYPE_IRQ, 	"IRQ_TYP" },			// 0x78
  {	DEV_ID_WDT_NMI_TYP,		DEV_TYPE_IRQ, 	"NMI_TYP" },			// 0x79
  {	DEV_ID_TAC_FAN0_SPEED,	DEV_TYPE_TAC, 	"FAN_TAC0" },			// 0x80
  {	DEV_ID_TAC_FAN1_SPEED,	DEV_TYPE_TAC, 	"FAN_TAC1" },			// 0x81
  {	DEV_ID_TAC_FAN2_SPEED,	DEV_TYPE_TAC, 	"FAN_TAC2" },			// 0x82
  {	DEV_ID_TAC_FAN3_SPEED,	DEV_TYPE_TAC, 	"FAN_TAC3" },			// 0x83
  {	DEV_ID_TAC_FAN4_SPEED,	DEV_TYPE_TAC, 	"FAN_TAC4" },			// 0x84
  {	DEV_ID_TAC_FAN5_SPEED,	DEV_TYPE_TAC, 	"FAN_TAC5" },			// 0x85
  {	DEV_ID_TAC_FAN6_SPEED,	DEV_TYPE_TAC, 	"FAN_TAC6" },			// 0x86
  {	DEV_ID_TAC_FAN7_SPEED,	DEV_TYPE_TAC, 	"FAN_TAC7" },			// 0x87
  {	DEV_ID_PWM_BRT1,		DEV_TYPE_PWM, 	"LVDS1_BRT" },			// 0x88
  {	DEV_ID_BL_CTL0,			DEV_TYPE_GPIO,	"BL_CTL0" },			// 0x89
  {	DEV_ID_BL_CTL1,			DEV_TYPE_GPIO,	"BL_CTL1" },			// 0x8A
  {	DEV_ID_TMP_CPU0,		DEV_TYPE_FUNC,	"CPU0_TEMP" },			// 0x90
  {	DEV_ID_TMP_CPU1,		DEV_TYPE_FUNC,	"CPU1_TEMP" },			// 0x91
  {	DEV_ID_TMP_SYS0,		DEV_TYPE_FUNC,	"SYS0_TEMP" },			// 0x92
  {	DEV_ID_TMP_SYS1,		DEV_TYPE_FUNC,	"SYS1_TEMP" },			// 0x93
  {	DEV_ID_TP_SHT,			DEV_TYPE_FUNC,	"TP_SHT" },				// 0x94
  {	DEV_ID_TP_PWR,			DEV_TYPE_FUNC,	"TP_PWR" },				// 0x95
  {	DEV_ID_TP_THR,			DEV_TYPE_FUNC,	"TP_THR" },				// 0x96
  {	DEV_ID_WDT_WRST,		DEV_TYPE_FUNC,	"WDT_WARM_RST" },		// 0x97
  {	DEV_ID_WDT_CRST,		DEV_TYPE_FUNC,	"WDT_COLD_RST" },		// 0x98
  {	DEV_ID_WDT_SCI,			DEV_TYPE_FUNC,	"WDT_SCI" },			// 0x99
  {	DEV_ID_WDT_IRQ,			DEV_TYPE_FUNC,	"WDT_IRQ" },			// 0x9A
  {	DEV_ID_WDT_PIN,			DEV_TYPE_FUNC,	"WDT_PIN" },			// 0x9B
  {	DEV_ID_EE_WP,			DEV_TYPE_FUNC,	"EE_WP" },				// 0x9C
  {	DEV_ID_PW_ON_SEC,		DEV_TYPE_FUNC,	"PWR_ON_SEC" },			// 0x9D*
  {	DEV_ID_GPIO8,			DEV_TYPE_GPIO,	"GPIO8" },				// 0xA0
  {	DEV_ID_GPIO9,			DEV_TYPE_GPIO,	"GPIO9" },				// 0xA1
  {	DEV_ID_GPIO10,			DEV_TYPE_GPIO,	"GPIO10" },				// 0xA2
  {	DEV_ID_GPIO11,			DEV_TYPE_GPIO,	"GPIO11" },				// 0xA3
  {	DEV_ID_GPIO12,			DEV_TYPE_GPIO,	"GPIO12" },				// 0xA4
  {	DEV_ID_GPIO13,			DEV_TYPE_GPIO,	"GPIO13" },				// 0xA5
  {	DEV_ID_GPIO14,			DEV_TYPE_GPIO,	"GPIO14" },				// 0xA6
  {	DEV_ID_GPIO15,			DEV_TYPE_GPIO,	"GPIO15" },				// 0xA7
  {	DEV_ID_SMB_OEM4,		DEV_TYPE_SMB,	"SMB_OEM4" },			// 0xA8
  {	DEV_ID_SMB_OEM5,		DEV_TYPE_SMB,	"SMB_OEM5" },			// 0xA9
  {	DEV_ID_FAN3,			DEV_TYPE_PWM,	"FAN3" },				// 0xAA
  {	DEV_ID_FAN4,			DEV_TYPE_PWM,	"FAN4" },				// 0xAB
  {	DEV_ID_FAN5,			DEV_TYPE_PWM,	"FAN5" },				// 0xAC
  {	DEV_ID_FAN6,			DEV_TYPE_PWM,	"FAN6" },				// 0xAD
  {	DEV_ID_FAN7,			DEV_TYPE_PWM,	"FAN7" },				// 0xAE
  {	DEV_ID_FAN8,			DEV_TYPE_PWM,	"FAN8" },				// 0xAF
  { DEV_ID_GPIO_GROUP0,		DEV_TYPE_FUNC,	"GPIO_GROUP0" },		// 0xB0
  { DEV_ID_GPIO_GROUP1,		DEV_TYPE_FUNC,	"GPIO_GROUP1" },		// 0xB1
  { DEV_ID_TP_BEEP,			DEV_TYPE_FUNC,	"TP_BEEP" },			// 0xC0*
  { DEV_ID_CASEOPEN,		DEV_TYPE_FUNC,	"CASEOPEN" }			// 0xCA*
};

//=============================================================================
//  ite_dev_find_type
//=============================================================================
uint8_t ite_dev_find_type(uint8_t did)
{
	int			i;
	uint8_t		found;

	for (i=0, found=0; i<DEV_NAME_TAB_NUM; i++)
	{
		if (did == ite_dev_table[i].did)
		{
			found = 1;
			break;
		}
	}

	if (found)
		return ite_dev_table[i].typ;
	else
		return DEV_TYPE_NONE;
}

//=============================================================================
//  ite_dev_name
//=============================================================================
char *ite_dev_name(uint8_t did)
{
	int			i;
	uint8_t		found;

	for (i=0, found=0; i<DEV_NAME_TAB_NUM; i++)
	{
		if (did == ite_dev_table[i].did)
		{
			found = 1;
			break;
		}
	}

	if (found)
		return ite_dev_table[i].name;
	else
		return DEV_TYPE_NONE;
}

//=============================================================================
//  ite_dev_type_name
//=============================================================================
char *ite_dev_type_name(uint8_t typ)
{
	if (typ > 10)
		return ite_dev_type_name_str[11];
	else
		return ite_dev_type_name_str[typ];
}

//=============================================================================
//  ite_dt_read_hwpin
//=============================================================================
uint8_t ite_dt_read_hwpin(uint8_t did, uint8_t *dat)
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
	wb[0] = 2;		// len  : wblen=2
	wb[1] = did;	// par  : device id

	rb[0] = 0;		// rlen : default read length, it depends on different type of device
	rb[1] = 0;		// roff : dat0~

	// variable length of different device type
	switch(ite_dev_find_type(did))
	{
		case DEV_TYPE_GPIO:
		case DEV_TYPE_OEMGPIO:
		case DEV_TYPE_PWM:
		case DEV_TYPE_DAC:
		case DEV_TYPE_FUNC:
			rb[0] = 1;	// dat0 : one byte to read
			break;

		case DEV_TYPE_ADC:
		case DEV_TYPE_TAC:
			rb[0] = 2;	// dat0~dat1 : two bytes to read
			
			// except adc current device
			if (did ==DEV_ID_ADC_CURR)
				rb[0] = 5;	// dat0~dat4 : five bytes to read

			break;
	}

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_HWPIN_RD, wb, rb);

	if (res != ITE_OK)
	{
		*dat = 0xFF;
		return res;
	}

	//-------------------------------------------------------------------------
	// rb   <0>   <1>   <2>   <3>   <4>   <5>.........
	// rb : rlen, roff, slen, soff, dat0, dat1........
	//-------------------------------------------------------------------------
	memcpy(dat, &rb[4], rb[0]);		// dat0~, rb[0]=rlen

	return ITE_OK;
}

//=============================================================================
//  ite_dt_write_hwpin
//=============================================================================
uint8_t ite_dt_write_hwpin(uint8_t did, uint8_t dat)
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
	wb[0] = 5;		// len  : wblen=5
	wb[1] = did;	// par  : device id
	wb[2] = 0;		// dlen : useless
	wb[3] = 0;		// woff : dat0~
	wb[4] = 0;		// wlen : default write length

	// variable length of different device type
	switch(ite_dev_find_type(did))
	{
		case DEV_TYPE_GPIO:
		case DEV_TYPE_OEMGPIO:
		case DEV_TYPE_PWM:
		case DEV_TYPE_DAC:
		case DEV_TYPE_FUNC:
			wb[4] = 1;	// wlen: dat0
			break;
	}

	// wb[4]=wlen > 0
	if (wb[4])
	{
		wb[5] = dat;	// dat0~
		wb[0] += wb[4];	// += wlen
	}

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_HWPIN_WR, wb, rb);

	return res;
}

//=============================================================================
//  ite_dt_init
//=============================================================================
uint8_t ite_dt_init(void)
{
	int			i;
	uint8_t		res;
	uint8_t		wb[50];
	uint8_t		rb[50];
	uint8_t		did[ITE_MAX_NUM_DEV];
	uint8_t		pin[ITE_MAX_NUM_DEV];
	uint8_t		pol[ITE_MAX_NUM_DEV];
	uint8_t		nd;


	memset(wb, 0, sizeof(wb));
	memset(rb, 0, sizeof(rb));

	//=========================================================================
	//  DID : device id
	//=========================================================================
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 2;						// wblen=2
	wb[1] = MBX_DYNATBL_DID;		// par  : did of dynamic table
	
	rb[0] = ITE_MAX_NUM_DEV;	// rlen : no way to know how many devices, so assigned max number
	rb[1] = 0;						// roff : dat0~

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_DYNATBL, wb, rb);
	ITE_ERR_CHECK(res);

	//-------------------------------------------------------------------------
	//  ndev : parsing did list to determine how many device ID
	//-------------------------------------------------------------------------		
	for (i=0, nd=0; i<ITE_MAX_NUM_DEV; i++)
	{
		if (rb[4+i] == DEV_ID_NONE)	// 4:dat0
			break;

		nd++;	// count num of device
	}

	// nd : got num of device here!

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>
	// rb : rlen, roff, slen, soff, dat0, dat1.....
	//-------------------------------------------------------------------------
	// reading did
	memcpy(did, &rb[4], nd);	// dat0~

	//=========================================================================
	//  PIN : hardware pin
	//=========================================================================
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 2;					// wblen=2
	wb[1] = MBX_DYNATBL_HWPIN;	// par  : did of dynamic table

	rb[0] = nd;					// rlen : no way to know how many devices, so assigned max number
	rb[1] = 0;					// roff : dat0~

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_DYNATBL, wb, rb);
	ITE_ERR_CHECK(res);

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>
	// rb : rlen, roff, slen, soff, dat0, dat1.....
	//-------------------------------------------------------------------------
	// reading pin
	memcpy(pin, &rb[4], nd);	// dat0~

	//=========================================================================
	//  POL : polarity
	//=========================================================================
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 2;					// wblen=2
	wb[1] = MBX_DYNATBL_POL;	// par  : did of dynamic table

	rb[0] = nd;					// rlen : no way to know how many devices, so assigned max number
	rb[1] = 0;					// roff : dat0~

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_DYNATBL, wb, rb);
	ITE_ERR_CHECK(res);

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>
	// rb : rlen, roff, slen, soff, dat0, dat1.....
	//-------------------------------------------------------------------------
	// reading pol
	memcpy(pol, &rb[4], nd);	// dat0~

	//-------------------------------------------------------------------------
	//  create dynamic table
	//-------------------------------------------------------------------------
	for (i=0; i<nd; i++)
	{
		if (i >= ITE_MAX_NUM_DEV)
			break;

		ite->dt.dev[i].did = did[i];
		ite->dt.dev[i].pin = pin[i];
		ite->dt.dev[i].pol = pol[i];
		ite->dt.dev[i].typ = ite_dev_find_type(did[i]);
	}

	ite->dt.num = nd;		// num of dev

	return ITE_OK;
}
