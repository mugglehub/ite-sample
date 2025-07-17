#include <stdio.h>
#include <stdint.h>
#include <sys/io.h>

#include "acpi.h"

//=============================================================================
//  acpi_wait_ibe
//=============================================================================
static uint8_t acpi_wait_ibe(void)
{
	uint16_t	retries = ACPI_RETRIES;

	while (inb(ACPI_STS_PORT) & ACPI_STS_IBF)
	{
		if (--retries == 0)
			return ACPI_ERR_IBE;
	}

	return ACPI_OK;
}

//=============================================================================
//  acpi_wait_obf
//=============================================================================
static uint8_t acpi_wait_obf(void)
{
	uint16_t	retries = ACPI_RETRIES;

	while ((inb(ACPI_STS_PORT) & ACPI_STS_OBF) == 0)
	{
		if (--retries == 0)
			return ACPI_ERR_OBF;
	}

	return ACPI_OK;
}

//=============================================================================
//  acpi_ec_read
//=============================================================================
uint8_t acpi_ec_read(uint8_t idx, uint8_t *datap)
{
	uint8_t		res = 0;

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(ACPI_CMD_READ, ACPI_CMD_PORT);

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(idx, ACPI_DAT_PORT);

	res = acpi_wait_obf();
	ACPI_CHECK_RESULT(res);

	*datap = inb(ACPI_DAT_PORT);

	return res;
}

//=============================================================================
//  acpi_ec_write
//=============================================================================
uint8_t acpi_ec_write(uint8_t idx, uint8_t data)
{
	uint8_t		res = 0;

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(ACPI_CMD_WRITE, ACPI_CMD_PORT);

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(idx, ACPI_DAT_PORT);

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(data, ACPI_DAT_PORT);

	return res;
}
