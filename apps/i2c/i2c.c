#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "ite.h"
	
//=============================================================================
//  ite_i2c_show
//=============================================================================
void ite_i2c_show(void)
{
	int			i;

	fprintf(stderr, "I2C Status\n");

	if (ite->i2c.num == 0)
		fprintf(stderr, "I2C not found!\n");

	for (i=0; i<ITE_MAX_NUM_I2C; i++)
	{
		if (ite->i2c.mask & (1<<i))
		{
			fprintf(stderr, "I2C %d : %02Xh  %s\n", i, ite->i2c.dev[i], ite_dev_name(ite->i2c.dev[i]));
		}
	}
}

//=============================================================================
//  main
//=============================================================================
int main(int argc, char **argv)
{
	uint8_t 	res;
	uint8_t		ch;
	uint16_t	ck;
	uint32_t	sla;
	uint32_t	reg;
	uint8_t		wdat[4];
	uint8_t		*rdat;
	uint32_t	rlen;
	int			i;
	
	res = ite_init();

	if (res != ITE_OK)
	{
		fprintf(stderr, "ite_init() error! res=%d (%s)\n", res, ite_err_msg_str(res));
		return (int)res;
	}

	ite_i2c_init();

	if (argc == 2 && strcmp(argv[1], "-s")==0)
	{
		ite_i2c_show();
	}
	else if (argc == 3)
	{
		if (strcmp(argv[1], "-p") == 0)
		{
			ch = (uint8_t)atoi(argv[2]);

			fprintf(stderr, "I2C %d : ", ch);
			for (sla=0x10; sla <0xF0; sla+=2)
			{
				res = ite_i2c_probe(ch, sla);
				if (!res)
					fprintf(stderr, "%02X ", sla);
			}
			fprintf(stderr, "\n");
		}
	}
	else if (argc == 4)
	{
		if (strcmp(argv[1], "-c") == 0)
		{
			ch = (uint8_t)atoi(argv[2]);
			ck = (uint16_t)atoi(argv[3]);
		
			res = ite_i2c_set_clock(ch, ck);
			
			if (res)
				fprintf(stderr, "i2c set clock error!\n");
			else
				fprintf(stderr, "i2c set clock ok!\n");
		}
		else if (strcmp(argv[1], "-p") == 0)
		{
			ch = (uint8_t)atoi(argv[2]);
			sscanf(argv[3], "%X", &sla);
		
			res = ite_i2c_probe(ch, (uint8_t)sla);
			if (res)
				fprintf(stderr, "i2c probe %02Xh error!\n", sla);
			else
				fprintf(stderr, "i2c probe %02Xh ok!\n", sla);
		}
	}
	else if (argc == 6)
	{
		if (strcmp(argv[1], "-t") == 0)
		{
			ch = (uint8_t)atoi(argv[2]);
			sscanf(argv[3], "%X", &sla);
			sscanf(argv[4], "%X", &reg);
			rlen = atol(argv[5]);
			
			wdat[0] = (uint8_t)reg;
			
			rdat = (uint8_t *)malloc(sizeof(uint8_t) * rlen);
			if (rdat)
			{
				res = ite_i2c_trans(ch, (uint8_t)sla, wdat, 1, rdat, &rlen);
			
				if (res == ITE_OK)
				{
					if (rlen > 0)
					{
						for (i=0; i<(int)rlen; i++)
						{
							fprintf(stderr, "%02X ", rdat[i]);
							if ((i&0xF)==0xF)
								fprintf(stderr, "\n");
						}
					}
				}
				
				free(rdat);
			}
			else
			{
				fprintf(stderr, "malloc error!\n");
			}
		}
	}
	else
	{
		fprintf(stderr, "i2c -s            : i2c status\n");
		fprintf(stderr, "i2c -c <ch> <clk> : i2c set clock rate (ch=0~7, clk=50,100,400)\n");
		fprintf(stderr, "i2c -p <ch>       : i2c probe bus\n");
		fprintf(stderr, "i2c -p <ch> <sla> : i2c probe slave address\n");
		fprintf(stderr, "i2c -t <ch> <sla> <reg> : i2c trans\n");
		
	}

	ite_exit();

	return 0;
}
