#ifndef __ACPI_H
#define __ACPI_H

// port
#define ACPI_CMD_PORT	0x66
#define ACPI_STS_PORT	0x66
#define ACPI_DAT_PORT	0x62

// command
#define ACPI_CMD_READ	0x80
#define ACPI_CMD_WRITE	0x81

// status
#define ACPI_STS_OBF	0x01
#define ACPI_STS_IBF	0x02

// retries
#define ACPI_RETRIES	(1000)

// error code
#define ACPI_OK			0
#define ACPI_ERR_OBF	1
#define ACPI_ERR_IBE	2

#define ACPI_CHECK_RESULT(res)	{ if (res != ACPI_OK) return res; }

//=============================================================================
//  functions
//=============================================================================
uint8_t acpi_ec_read(uint8_t idx, uint8_t *datap);
uint8_t acpi_ec_write(uint8_t idx, uint8_t data);

#endif