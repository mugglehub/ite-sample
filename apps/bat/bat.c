#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "ite.h"
	
//=============================================================================
//  ite_bat_show
//=============================================================================
void ite_bat_show(void)
{
	int			i;

	fprintf(stderr, "Battery Status\n");

	if (ite->bat.num == 0)
		fprintf(stderr, "Battery device not found!\n");
	else
		fprintf(stderr, "AC Status : %d\n", ite_bat_ac_status());
	
	for (i=0; i<ite->bat.num; i++)
	{
		fprintf(stderr, "BAT%d Status : %d\n", i, ite_bat_status((uint8_t)i));
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

	ite_bat_init();

	ite_bat_show();

	ite_exit();

	return 0;
}
