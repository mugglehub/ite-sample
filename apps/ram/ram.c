#include <stdio.h>
#include <stdint.h>

#include "ite.h"

//=============================================================================
//  ite_show_ram
//=============================================================================
void ite_show_ram(void)
{
	int		i;
	uint8_t	buf[32];
	
	// read ecram : addr=0x40~0x5F
	ite_ram_read(ITE_ECRAM_TYP_ACPI, 0x40, 32, buf);
	
	for (i=0; i<32; i++)
	{
		if ((i&0xF) == 0)
			fprintf(stderr, "\n%02X  ", i+0x40);
	
		fprintf(stderr, "%02X ", buf[i]);
	}
	
	fprintf(stderr, "\n");

}

//=============================================================================
//  ite_write_ram
//=============================================================================
void ite_write_ram(void)
{
	uint8_t		buf[8];
	
	buf[0] = 0xB2;

	// addr=0x50, data=0xB2
	ite_ram_write(ITE_ECRAM_TYP_ACPI, 0x50, 1, buf);
}
	
//=============================================================================
//  main
//=============================================================================
int main(void)
{
	uint8_t res;
	
	res = ite_init();

	if (res != ITE_OK)
	{
		fprintf(stderr, "ite_init() error! res=%d (%s)\n", res, ite_err_msg_str(res));
		return (int)res;
	}

	ite_show_ram();
	
	ite_write_ram();
	
	ite_exit();

	return 0;
}
