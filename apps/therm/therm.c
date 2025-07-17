#include <stdio.h>
#include <stdint.h>

#include "ite.h"
#include "ite_therm.h"

//=============================================================================
//  ite_show_therm
//=============================================================================
void ite_show_therm(void)
{
	int			i;
	uint8_t		res;
	int8_t		temp;


	//-------------------------------------------------------------------------
	//  thermal temperature
	//-------------------------------------------------------------------------
	for (i=0; i<ite->tzchnum; i++)
	{
		if (ite->thermmask & (1 << i))
		{
			res = ite_therm_read_temp((uint8_t)i, &temp);	// unit=1C
			if (res == ITE_OK)
				fprintf(stderr, "   Temp %d = %d C   %s\n", i, temp, ite_dev_name(ite->tzdev[i]));
			else
				fprintf(stderr, "   Temp %d = error!\n", i);
		}
	}

	//-------------------------------------------------------------------------
	//  CPU temperature
	//-------------------------------------------------------------------------
	for (i=0; i<ite->tzcpunum; i++)
	{
		res = ite_therm_read_cputemp((uint8_t)i, &temp);	// unit=1C
		fprintf(stderr, "CPU%d Temp = ", i);
		if (res == ITE_OK)
			fprintf(stderr, "%d C\n", temp);
		else
			fprintf(stderr, "error!\n");
	}

	//-------------------------------------------------------------------------
	//  SYS temperature
	//-------------------------------------------------------------------------
	for (i=0; i<ite->tzsysnum; i++)
	{
		res = ite_therm_read_systemp((uint8_t)i, &temp);	// unit=1C
		fprintf(stderr, "SYS%d Temp = ", i);
		if (res == ITE_OK)
			fprintf(stderr, "%d C\n", temp);
		else
			fprintf(stderr, "error!\n");
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

	res = ite_therm_init();

	if (res != ITE_OK)
	{
		fprintf(stderr, "ite_therm_init() error! res=%d (%s)\n", res, ite_err_msg_str(res));
		return (int)res;
	}

	ite_show_therm();
	
	ite_exit();

	return 0;
}
