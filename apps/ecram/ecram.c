#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "ite.h"
	
//=============================================================================
//  ite_show_ecram
//=============================================================================
void ite_show_ecram(void)
{
	int		i, j;
	uint8_t	buf[256];

	fprintf(stderr, "====================================================================\n");
	fprintf(stderr, "                              EC RAM\n");
	fprintf(stderr, "====================================================================\n");
	fprintf(stderr, "     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

	// read ecram : addr=0x00~0xFF
	for (i=0; i<256; i+=32)
		ite_ram_read(ITE_ECRAM_TYP_ACPI, (uint8_t)i, 32, &buf[i]);
	
	for (i=0; i<256; i++)
	{
		// print addr
		if ((i&0xF) == 0)
			fprintf(stderr, "%02X  ", i);
	
		// print hex
		fprintf(stderr, "%02X ", buf[i]);
		
		// print ascii
		if ((i&0xF) == 0xF)
		{
			for (j=i-15; j<=i; j++)
			{
				if (buf[j] >= 0x20 && buf[j] <= 0x7E)
					// printable
					fprintf(stderr, "%c", buf[j]);
				else
					// non-printable
					fprintf(stderr, ".");
			}
			
			fprintf(stderr, "\n");
		}
	}
	
	fprintf(stderr, "\n");

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

	ite_show_ecram();
	
	ite_exit();

	return 0;
}
