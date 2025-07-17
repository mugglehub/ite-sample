#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ite.h"


//=============================================================================
//  ite_bl_show
//=============================================================================
void ite_bl_show(uint8_t ch)
{
	uint8_t		res;
	uint8_t		pwm;
	uint8_t		sw;
	
	fprintf(stderr, "BL num  : %d\n", ite->bl.num);
	fprintf(stderr, "BL mask : %02Xh\n", ite->bl.mask);
	
	res = ite_bl_sw_read(ch, &sw);
	if (res == ITE_OK)
		fprintf(stderr, "BL %d SW  : %s\n", ch, sw==0?"off":"on");
	else
		fprintf(stderr, "BL %d SW  : error!\n", ch);
	
	res = ite_bl_pwm_read(ch, &pwm);
	if (res == ITE_OK)
		fprintf(stderr, "BL %d PWM : %d\n", ch, pwm);
	else
		fprintf(stderr, "BL %d PWM : error!\n", ch);
}

//=============================================================================
//  main
//=============================================================================
int main(int argc, char *argv[])
{
	uint8_t 	res;
	uint8_t		ch;
	uint8_t		mode = 0xFF;
	uint8_t		pwm;

	if (argc == 3)
	{
		if (strcmp(argv[2], "on") == 0)
		{
			mode = 1;
		}
		else if (strcmp(argv[2], "off") == 0)
		{
			mode = 0;
		}
		else
		{
			pwm = atoi(argv[2]);

			if (pwm >= 0 && pwm <= 100)
				mode = 2;
			else
			{
				fprintf(stderr, "bl <ch>\n");
				fprintf(stderr, "bl <ch> [ on|off|<pwm> ]\n");
				return -1;
			}
		}
		
	}
	else if (argc == 2)
	{
		mode = 3;
	}
	else
	{
		fprintf(stderr, "bl <ch>\n");
		fprintf(stderr, "bl <ch> [ on|off|<pwm> ]\n");
		return -1;
	}

	ch = atoi(argv[1]);
	
	
	res = ite_init();

	if (res != ITE_OK)
	{
		fprintf(stderr, "ite_init() error! res=%d (%s)\n", res, ite_err_msg_str(res));
		return (int)res;
	}
	
	ite_bl_init();

	switch(mode)
	{
		case 0:	ite_bl_sw_write(ch, 0);		break;
		case 1:	ite_bl_sw_write(ch, 1);		break;
		case 2: ite_bl_pwm_write(ch, pwm);	break;
		case 3: ite_bl_show(ch);			break;
	}
	
	ite_exit();

	return 0;
}
