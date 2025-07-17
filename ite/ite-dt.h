#ifndef __ITE_DT_H
#define __ITE_DT_H

// numbers
#define DEV_NAME_TAB_NUM		(136)
#define DEV_TYPE_TAB_NUM		(12)

//=============================================================================
//  Dynamic Table : Device ID
//=============================================================================
#define DEV_ID_NONE				0x00
// Reserved : 01h ~ 0Fh 
// GPIO
#define DEV_ID_GPIO0			0x10
#define DEV_ID_GPIO1			0x11
#define DEV_ID_GPIO2			0x12
#define DEV_ID_GPIO3			0x13
#define DEV_ID_GPIO4			0x14
#define DEV_ID_GPIO5			0x15
#define DEV_ID_GPIO6			0x16
#define DEV_ID_GPIO7			0x17
// Button
#define DEV_ID_BTN0				0x18
#define DEV_ID_BTN1				0x19
#define DEV_ID_BTN2				0x1A
#define DEV_ID_BTN3				0x1B
#define DEV_ID_BTN4				0x1C
#define DEV_ID_BTN5				0x1D
#define DEV_ID_BTN6				0x1E
#define DEV_ID_BTN7				0x1F
// PWM
#define DEV_ID_FAN_CPU2P		0x20
#define DEV_ID_FAN_CPU4P		0x21
#define DEV_ID_FAN_SYS02P		0x22
#define DEV_ID_FAN_SYS04P		0x23
#define DEV_ID_FAN_SYS12P		0x24
#define DEV_ID_FAN_SYS14P		0x25
#define DEV_ID_FAN_START		DEV_ID_FAN_CPU2P
#define DEV_ID_FAN_END			DEV_ID_FAN_SYS14P
// Brightness
#define DEV_ID_PWM_BRT0			0x26
#define DEV_ID_PWM_BEEP			0x27
// SMB_OEM
#define DEV_ID_SMB_OEM0			0x28
#define DEV_ID_SMB_OEM1			0x29
#define DEV_ID_SMB_OEM2			0x2A
#define DEV_ID_SMB_EE			0x2B
#define DEV_ID_SMB_THERM0		0x2C
#define DEV_ID_SMB_THERM1		0x2D
#define DEV_ID_SMB_EESEC		0x2E	// Security EEPROM
#define DEV_ID_I2C_OEM			0x2F
// DAC
#define DEV_ID_DAC_SPKR			0x30
// Reserved : 31h ~ 37h 
// SMB_EE
#define DEV_ID_SMB_EE2K			0x38
#define DEV_ID_OEM_EE			0x39
#define DEV_ID_OEM_EE2K			0x3A	// EEPROM size > 2KB (>16Kb)
#define DEV_ID_PECI				0x3B
#define DEV_ID_SMB_OEM3			0x3C
#define DEV_ID_SMB_SML			0x3D
#define DEV_ID_SMB_SLA			0x3E	// slave mode
#define DEV_ID_SMB_EE64K		0x3F	// EEPROM size > 64K (>512Kb) : 1Mb = 128KB EEPROM
// LED
#define DEV_ID_LED_PWR			0x40
#define DEV_ID_LED_BATG			0x41
#define DEV_ID_LED_OEM0			0x42
#define DEV_ID_LED_OEM1			0x43
#define DEV_ID_LED_OEM2			0x44
#define DEV_ID_LED_BATR			0x45
#define DEV_ID_LED_PWM_PWR		0x46
#define DEV_ID_LED_PWM_BAT		0x47
// Smart Battery
#define DEV_ID_BAT0				0x48
#define DEV_ID_BAT1				0x49
// Reserved : 4Ah ~ 4Fh 
// Voltage, Current
#define DEV_ID_ADC_BAT_CMOS		0x50
#define DEV_ID_ADC_BAT_CMOSX2	0x51
#define DEV_ID_ADC_BAT_CMOSX10	0x52
#define DEV_ID_ADC_BAT_LION		0x53
#define DEV_ID_ADC_BAT_LIONX2	0x54
#define DEV_ID_ADC_BAT_LIONX10	0x55
#define DEV_ID_ADC_V5S0			0x56
#define DEV_ID_ADC_V5S0X2		0x57
#define DEV_ID_ADC_V5S0X10		0x58
#define DEV_ID_ADC_V5S5			0x59
#define DEV_ID_ADC_V5S5X2		0x5A
#define DEV_ID_ADC_V5S5X10		0x5B
#define DEV_ID_ADC_V33S0		0x5C
#define DEV_ID_ADC_V33S0X2		0x5D
#define DEV_ID_ADC_V33S0X10		0x5E
#define DEV_ID_ADC_V33S5		0x5F
#define DEV_ID_ADC_V33S5X2		0x60
#define DEV_ID_ADC_V33S5X10		0x61
#define DEV_ID_ADC_V12S0		0x62
#define DEV_ID_ADC_V12S0X2		0x63
#define DEV_ID_ADC_V12S0X10		0x64
#define DEV_ID_ADC_VCOREA		0x65
#define DEV_ID_ADC_VCOREAX2		0x66
#define DEV_ID_ADC_VCOREAX10	0x67
#define DEV_ID_ADC_VCOREB		0x68
#define DEV_ID_ADC_VCOREBX2		0x69
#define DEV_ID_ADC_VCOREBX10	0x6A
#define DEV_ID_ADC_DC			0x6B
#define DEV_ID_ADC_DCX2			0x6C
#define DEV_ID_ADC_DCX10		0x6D
#define DEV_ID_ADC_DCSB			0x6E	// standby
#define DEV_ID_ADC_DCSBX2		0x6F
#define DEV_ID_ADC_DCSBX10		0x70
#define DEV_ID_ADC_DCOEM		0x71
#define DEV_ID_ADC_DCOEMX2		0x72
#define DEV_ID_ADC_DCOEMX10		0x73
#define DEV_ID_ADC_CURR			0x74
#define DEV_ID_VOLT_START		DEV_ID_ADC_BAT_CMOS
#define DEV_ID_VOLT_END			DEV_ID_ADC_DCOEMX10
// Reserved : 75h ~ 77h 
// Watchdog IRQ/NMI
#define DEV_ID_WDT_IRQ_TYP		0x78
#define DEV_ID_WDT_NMI_TYP		0x79
// Reserved : 7Ah ~ 7Fh
// Tachometer
#define DEV_ID_TAC_FAN0_SPEED	0x80
#define DEV_ID_TAC_FAN1_SPEED	0x81
#define DEV_ID_TAC_FAN2_SPEED	0x82
#define DEV_ID_TAC_FAN3_SPEED	0x83
#define DEV_ID_TAC_FAN4_SPEED	0x84
#define DEV_ID_TAC_FAN5_SPEED	0x85
#define DEV_ID_TAC_FAN6_SPEED	0x86
#define DEV_ID_TAC_FAN7_SPEED	0x87
#define DEV_ID_TAC_START		DEV_ID_TAC_FAN0_SPEED
#define DEV_ID_TAC_END			DEV_ID_TAC_FAN7_SPEED
// Brightness (2nd)
#define DEV_ID_PWM_BRT1			0x88
#define DEV_ID_BL_CTL0			0x89
#define DEV_ID_BL_CTL1			0x8A
// Reserved : 8Bh ~ 8Fh
// Temperature
#define DEV_ID_TMP_CPU0			0x90
#define DEV_ID_TMP_CPU1			0x91
#define DEV_ID_TMP_SYS0			0x92
#define DEV_ID_TMP_SYS1			0x93
// Thermal Protection
#define DEV_ID_TP_SHT			0x94
#define DEV_ID_TP_PWR			0x95
#define DEV_ID_TP_THR			0x96
// Watchdog
#define DEV_ID_WDT_WRST			0x97
#define DEV_ID_WDT_CRST			0x98
#define DEV_ID_WDT_SCI			0x99
#define DEV_ID_WDT_IRQ			0x9A
#define DEV_ID_WDT_PIN			0x9B
// EEPROM write-protect function ?
#define DEV_ID_EE_WP			0x9C	
// EAPI power on  second
#define DEV_ID_PW_ON_SEC		0x9D
// Reserved : 9Eh ~ 9Fh
#define DEV_ID_GPIO8			0xA0	// User control GPIO
#define DEV_ID_GPIO9			0xA1
#define DEV_ID_GPIO10			0xA2
#define DEV_ID_GPIO11			0xA3
#define DEV_ID_GPIO12			0xA4
#define DEV_ID_GPIO13			0xA5
#define DEV_ID_GPIO14			0xA6
#define DEV_ID_GPIO15			0xA7
#define DEV_ID_SMB_OEM4			0xA8
#define DEV_ID_SMB_OEM5			0xA9
// Fan (extend) : FAN3~FAN8 reserved for old fan
#define DEV_ID_FAN3				0xAA
#define DEV_ID_FAN4				0xAB
#define DEV_ID_FAN5				0xAC
#define DEV_ID_FAN6				0xAD
#define DEV_ID_FAN7				0xAE
#define DEV_ID_FAN8				0xAF
// GPIO Group
#define DEV_ID_GPIO_GROUP0		0xB0	// GPIO0~GPIO7
#define DEV_ID_GPIO_GROUP1		0xB1	// GPIO8~GPIO15

#define DEV_ID_TP_BEEP			0xC0
#define DEV_ID_CASEOPEN			0xCA

// Reserved : B2h ~ BFh
// Reserved : C0h ~ EFh
// Reserved : F0h ~ FFh for OEM usage

//=============================================================================
//  dynamic table : device type
//=============================================================================
#define DEV_TYPE_NONE			0
#define DEV_TYPE_GPIO			1
#define DEV_TYPE_OEMGPIO		2
#define DEV_TYPE_ADC			3
#define DEV_TYPE_DAC			4
#define DEV_TYPE_TAC			5
#define DEV_TYPE_PWM			6
#define DEV_TYPE_SMB			7
#define DEV_TYPE_BAT			8
#define DEV_TYPE_IRQ			9
#define DEV_TYPE_FUNC			10


//=============================================================================
//  functions
//=============================================================================
uint8_t ite_dev_find_type(uint8_t did);
char*	ite_dev_name(uint8_t did);
char*	ite_dev_type_name(uint8_t typ);

uint8_t ite_dt_read_hwpin(uint8_t did, uint8_t *dat);
uint8_t ite_dt_write_hwpin(uint8_t did, uint8_t dat);

uint8_t ite_dt_init(void);


#endif