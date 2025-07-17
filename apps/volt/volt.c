#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "ite.h"
	
//=============================================================================
//  ite_volt_show
//=============================================================================
void ite_volt_show(void)
{
	int			i;
	uint8_t		res;
	uint8_t		did;
	double		volt;

	fprintf(stderr, "Voltage Status\n");

	if (ite->volt.num == 0)
		fprintf(stderr, "Voltage device not found!\n");

	for (i=0; i<ite->volt.num; i++)
	{
		res = ite_volt_read((uint8_t)i,  &volt);
		
		did = ite->volt.dev[i];	// device id

		if (res == ITE_OK)
			fprintf(stderr, "VOLT %d : %7.4f V   %s\n", i, volt, ite_dev_name(did));
		else
			fprintf(stderr, "VOLT %d : err\n", i);
	}
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

	ite_volt_init();

	ite_volt_show();

	ite_exit();

	return 0;
}
