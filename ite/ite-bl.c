#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"

//=============================================================================
//  ite_bl_sw_read
//=============================================================================
uint8_t ite_bl_sw_read(uint8_t ch, uint8_t *sw)
{
	uint8_t		val;
	uint8_t		addr;

	if (ch >= ITE_MAX_NUM_BL)
		return ITE_ERR_NODEV;

	if ((ite->bl.mask & (1<<ch)) == 0)
		return ITE_ERR_NODEV;

	if (ch == 0)
		addr = 0x50;	// BRT0
	else
		addr = 0x52;	// BRT1
	
	ite_ram_read(ITE_ECRAM_TYP_ACPI, addr, 1, &val);

	// bit7 : bl en/dis
	if (val & 0x80)
		*sw = 1;	// enable
	else
		*sw = 0;	// disable
	
	return ITE_OK;
}

//=============================================================================
//  ite_bl_sw_write
//=============================================================================
uint8_t ite_bl_sw_write(uint8_t ch, uint8_t sw)
{
	uint8_t		val;
	uint8_t		addr;

	if (ch >= ITE_MAX_NUM_BL)
		return ITE_ERR_NODEV;

	if ((ite->bl.mask & (1<<ch)) == 0)
		return ITE_ERR_NODEV;

	if (ch == 0)
		addr = 0x50;	// BRT0
	else
		addr = 0x52;	// BRT1
	
	ite_ram_read(ITE_ECRAM_TYP_ACPI, addr, 1, &val);

	// bit7: 0=off, 1=on
	if (sw)
		val |= 0x80;
	else
		val &= ~0x80;
	
	ite_ram_write(ITE_ECRAM_TYP_ACPI, addr, 1, &val);
	
	return ITE_OK;
}

//=============================================================================
//  ite_bl_pwm_read
//=============================================================================
uint8_t ite_bl_pwm_read(uint8_t ch, uint8_t *pwm)
{
	uint8_t		val;
	uint8_t		addr;

	if (ch >= ITE_MAX_NUM_BL)
		return ITE_ERR_NODEV;

	if ((ite->bl.mask & (1<<ch)) == 0)
		return ITE_ERR_NODEV;

	if (ch == 0)
		addr = 0x50;	// BRT0
	else
		addr = 0x52;	// BRT1
	
	ite_ram_read(ITE_ECRAM_TYP_ACPI, addr, 1, &val);

	// bit[6:0] pwm duty
	*pwm = val & 0x7F;
	
	return ITE_OK;
}

//=============================================================================
//  ite_bl_pwm_write
//=============================================================================
uint8_t ite_bl_pwm_write(uint8_t ch, uint8_t pwm)
{
	uint8_t		val;
	uint8_t		addr;

	if (ch >= ITE_MAX_NUM_BL)
		return ITE_ERR_NODEV;

	if ((ite->bl.mask & (1<<ch)) == 0)
		return ITE_ERR_NODEV;
	
	if (pwm > 100)
		return ITE_ERR;

	if (ch == 0)
		addr = 0x50;	// BRT0
	else
		addr = 0x52;	// BRT1
	
	ite_ram_read(ITE_ECRAM_TYP_ACPI, addr, 1, &val);

	// bit7: 0=off, 1=on
	val &= 0x80;

	// bit[6:0] pwm duty
	val |= pwm;
	
	ite_ram_write(ITE_ECRAM_TYP_ACPI, addr, 1, &val);
	
	return ITE_OK;
}
	
//=============================================================================
//  ite_bl_init
//=============================================================================
uint8_t ite_bl_init(void)
{
	int			i;
	uint8_t		did;

	if (ite->dt.num == 0)
		return ITE_ERR_NODEV;
	
	ite->bl.num = 0;
	ite->bl.mask = 0;

	for (i=0; i<ite->dt.num; i++)
	{
		did = ite->dt.dev[i].did;

		if (did == DEV_ID_PWM_BRT0)
		{
			ite->bl.num++;
			ite->bl.mask |= (1<<0);	// BRT0
		}
		else if (did == DEV_ID_PWM_BRT1)
		{
			ite->bl.num++;
			ite->bl.mask |= (1<<1);	// BRT1
		}
	}
	
	if (ite->bl.num == 0)
		return ITE_ERR_NODEV;
	
	return ITE_OK;
}