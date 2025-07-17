#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <ncurses.h>

#include "ite.h"

//=============================================================================
//  main
//=============================================================================
int main(void)
{
	uint8_t 	res;
	uint8_t		blen;
	uint8_t		blpwm;
	uint8_t		buf[4];
	WINDOW		*win;
	int			c;
	int			key;
	
	res = ite_init();

	if (res != ITE_OK)
	{
		fprintf(stderr, "ite_init() error! res=%d (%s)\n", res, ite_err_msg_str(res));
		return (int)res;
	}

	// ECRAM[4Bh].bit0 : 1=manual, 0=auto
	ite_ram_read(ITE_ECRAM_TYP_ACPI, 0x4B, 1, buf);
	buf[0] |= 0x01;
	ite_ram_write(ITE_ECRAM_TYP_ACPI, 0x4B, 1, buf);
	


	win = initscr();
	nodelay(win, TRUE);
	noecho();

	move(0, 0);		printw("================  Menu  =================");
	move(1, 1);		printw(" +    : Increasing backlight brightness");
	move(2, 1);		printw(" -    : Decreasing backlight brightness");
	move(3, 1);		printw("m/M   : Select Mode");
	move(4, 1);		printw("q/Q   : Quit");

	move(6, 0);		printw("================ Monitor ================");
	move(7, 0);		printw("Auto Brightness Mode : ");
	move(8, 0);		printw("Light Sensor         : ");
	move(9, 0);		printw("Backlight Brightness : ");

	move(7, 23); 	printw("Manual");
	
	
	while (1)
	{
		refresh();
		
		while ((key = getch()) != EOF)
		{
			c = key;
		}
		
		if ((c == '+') || (c == '-'))
		{
			ite_ram_read(ITE_ECRAM_TYP_ACPI, 0x50, 1, &blpwm);
			
			// ECRAM[50h].bit7: backlight enable pin status
			blen = blpwm & 0x80;
			
			//  ECRAM[50h].bit[6:0]: backlight pwm duty
			blpwm &= 0x7F;
			
			if (c == '+')
			{
				if (blpwm < 100)
					blpwm++;
			}
			else if (c == '-')
			{
				if (blpwm > 0)
					blpwm--;
			}

			blpwm |= blen;
			ite_ram_write(ITE_ECRAM_TYP_ACPI, 0x50, 1, &blpwm);
		}
		else if ((c == 'q') || (c == 'Q'))
		{
			break;
		}
		else if  ((c == 'm') || (c == 'M'))
		{
			// ECRAM[4Bh].bit0 : 1=manual, 0=auto
			ite_ram_read(ITE_ECRAM_TYP_ACPI, 0x4B, 1, buf);
			if (buf[0] & 0x01)
				buf[0] &= ~0x01;
			else
				buf[0] |= 0x01;
			ite_ram_write(ITE_ECRAM_TYP_ACPI, 0x4B, 1, buf);
			
			move(7, 23);
			if (buf[0] & 0x01)
				printw("Manual");
			else
				printw("  Auto");
		}
		
		// light sensor
		ite_ram_read(ITE_ECRAM_TYP_ACPI, 0x40, 2, buf);
		move(8, 23);
		printw("%5d", *(uint16_t *)buf);

		// backlight pwm duty
		ite_ram_read(ITE_ECRAM_TYP_ACPI, 0x50, 1, buf);
		blpwm = buf[0] & 0x7F;
		move(9, 23);
		printw("%5d %%", blpwm);
		
		c = 0;
	}
	
	endwin();

	ite_exit();

	return 0;
}
