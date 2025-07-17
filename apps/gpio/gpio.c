#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "ite.h"

uint16_t dirmask = 0;
uint16_t datmask = 0;

//=============================================================================
//  ite_show_current_gpio
//=============================================================================
void ite_show_current_gpio(void)
{
	int			i;
	uint8_t		res;
	uint8_t		dir;
	uint8_t		lvl;

	// current gpio status
	fprintf(stderr, "  GPIO:0123456789ABCDEF\n");

	fprintf(stderr, "  Dir  ");
	for (i=0; i<16; i++)
	{
		res = ite_gpio_get_dir((uint8_t)i, &dir);
		if (res == ITE_OK)
		{
			if (dir)
				fprintf(stderr, "O");	// output
			else
				fprintf(stderr, "I");	// input
			
			dirmask |= (1<<i);			// pin avaliable
		}
		else
		{
			fprintf(stderr, "-");	// n/a or error
			
			dirmask &= ~(1<<i);		// pin n/a
		}
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "  Dat  ");
	for (i=0; i<16; i++)
	{
		res = ite_gpio_get_dat((uint8_t)i, &lvl);
		if (res == ITE_OK)
		{
			if (lvl)
				fprintf(stderr, "H");	// high
			else
				fprintf(stderr, "L");	// low
			
			datmask |= (1<<i);			// pin avaliable
		}
		else
		{
			fprintf(stderr, "-");	// n/a or error
			
			datmask &= ~(1<<i);		// pin n/a
		}
	}

	fprintf(stderr, "\n\n");
}
	
//=============================================================================
//  ite_show_gpio
//=============================================================================
void ite_show_gpio(void)
{
	int			i;
	uint8_t		res;
	uint8_t		dir;
	uint8_t		lvl;

	fprintf(stderr, "\n");

	fprintf(stderr, "Current GPIO Status\n\n");
	
	ite_show_current_gpio();

	//-------------------------------------------------------------------------
	//  set all gpio : output high
	//-------------------------------------------------------------------------
	fprintf(stderr, "Set all GPIO pin : OUTPUT high\n\n");
	
	for (i=0; i<16; i++)
	{
		if (dirmask & (1<<i))
		{
			dir = 1;	// output
			res = ite_gpio_set_dir((uint8_t)i, dir);
			if (res != ITE_OK)
				fprintf(stderr, "pin %d : set dir err=%d\n", i, res);
		}
	}

	for (i=0; i<16; i++)
	{
		if (datmask & (1<<i))
		{
			lvl = 1; // output high
			res = ite_gpio_set_dat((uint8_t)i, lvl);
			if (res != ITE_OK)
				fprintf(stderr, "pin %d : set dat err=%d\n", i, res);
		}
	}

	ite_show_current_gpio();

	//-------------------------------------------------------------------------
	//  set all gpio : output low
	//-------------------------------------------------------------------------
	fprintf(stderr, "Set all GPIO pin : OUTPUT Low\n\n");
	
	for (i=0; i<16; i++)
	{
		if (dirmask & (1<<i))
		{
			dir = 1;	// output
			res = ite_gpio_set_dir((uint8_t)i, dir);
			if (res != ITE_OK)
				fprintf(stderr, "pin %d : set dir err=%d\n", i, res);
		}
	}

	for (i=0; i<16; i++)
	{
		if (datmask & (1<<i))
		{
			lvl = 0; // output low
			res = ite_gpio_set_dat((uint8_t)i, lvl);
			if (res != ITE_OK)
				fprintf(stderr, "pin %d : set dat err=%d\n", i, res);
		}
	}
	
	ite_show_current_gpio();

	//-------------------------------------------------------------------------
	//  set all gpio : input
	//-------------------------------------------------------------------------
	fprintf(stderr, "Set all GPIO pin : INPUT\n\n");
	
	for (i=0; i<16; i++)
	{
		if (dirmask & (1<<i))
		{
			dir = 0;	// input
			res = ite_gpio_set_dir((uint8_t)i, dir);
			if (res != ITE_OK)
				fprintf(stderr, "pin %d : set dir err=%d\n", i, res);
		}
	}

	ite_show_current_gpio();

	//-------------------------------------------------------------------------
	//  Set GPIO0~3: output, GPIO4~7: input
	//-------------------------------------------------------------------------
	fprintf(stderr, "Set GPIO0~3: output, GPIO4~7: input\n\n");
	for (i=0; i<8; i++)
	{
		if (dirmask & (1<<i))
		{
			if (i < 4)
				dir = 1;	// GPIO0~3: output
			else
				dir = 0;	// GPIO4~7: input
			res = ite_gpio_set_dir((uint8_t)i, dir);
			if (res != ITE_OK)
				fprintf(stderr, "pin %d : set dir err=%d\n", i, res);
		}
	}

	ite_show_current_gpio();
	

	//-------------------------------------------------------------------------
	//  Set GPIO0~3: output, GPIO4~7: input
	//-------------------------------------------------------------------------
	fprintf(stderr, "Set GPIO0 GPIO2 : OUTPUT High\n\n");

	// GPIO0 : output high
	if (datmask & 0x01)
	{
		res = ite_gpio_set_dat(0, 1);
		if (res != ITE_OK)
			fprintf(stderr, "pin %d : set dat err=%d\n", i, res);
	}

	// GPIO2 : output high
	if (datmask & 0x04)
	{
		res = ite_gpio_set_dat(2, 1);
		if (res != ITE_OK)
			fprintf(stderr, "pin %d : set dat err=%d\n", i, res);
	}
	
	ite_show_current_gpio();


	//-------------------------------------------------------------------------
	//  Set GPIO0~3: output low, GPIO4~7: input
	//-------------------------------------------------------------------------
	fprintf(stderr, "Set GPIO0~3: output low, GPIO4~7: input\n\n");
	for (i=0; i<8; i++)
	{
		if (i < 4)
			dir = 1;	// GPIO0~3: output
		else
			dir = 0;	// GPIO4~7: input
		res = ite_gpio_set_dir((uint8_t)i, dir);
		if (res != ITE_OK)
			fprintf(stderr, "pin %d : set dir err=%d\n", i, res);
	}

	for (i=0; i<3; i++)
	{
		if (datmask & (1<<i))
		{
			lvl = 0; // output low
			res = ite_gpio_set_dat((uint8_t)i, lvl);
			if (res != ITE_OK)
				fprintf(stderr, "pin %d : set dat err=%d\n", i, res);
		}
	}

	ite_show_current_gpio();
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

	ite_gpio_init();

	ite_show_gpio();

	ite_exit();

	return 0;
}
