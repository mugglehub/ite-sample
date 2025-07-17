#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "ite.h"
	
//=============================================================================
//  ite_curr_show
//=============================================================================
void ite_curr_show(void)
{
	int			i;
	uint8_t		res;
	double		curr;
	uint8_t		did;

	fprintf(stderr, "Current Status\n");

	if (ite->curr.num == 0)
		fprintf(stderr, "Current device not found!\n");
	
	for (i=0; i<ite->curr.num; i++)
	{
		did = ite->curr.dev[i];

		res = ite_curr_read((uint8_t)i,  &curr);
		if (res == ITE_OK)
			fprintf(stderr, "CURR %d : %.1f mA    %s\n", i, curr*1000.0, ite_dev_name(did));
		else
			fprintf(stderr, "CURR %d : err\n", i);
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

	ite_curr_init();

	ite_curr_show();

	ite_exit();

	return 0;
}
