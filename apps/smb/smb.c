#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "ite.h"
	
//=============================================================================
//  ite_smb_show
//=============================================================================
void ite_smb_show(void)
{
	int			i;

	fprintf(stderr, "SMBus Status\n");

	if (ite->smb.num == 0)
		fprintf(stderr, "SMBus not found!\n");

	for (i=0; i<ITE_MAX_NUM_SMB; i++)
	{
		if (ite->smb.mask & (1<<i))
		{
			fprintf(stderr, "SMBus %d : %02Xh  %s\n", i, ite->smb.dev[i], ite_dev_name(ite->smb.dev[i]));
		}
	}
}

//=============================================================================
//  probe_ite_smb
//=============================================================================
void probe_ite_smb(uint8_t ch)
{
	uint8_t		res;
	uint8_t		sla;
	
	fprintf(stderr, "Probe SMB %d : ", ch);

	for (sla=0x10; sla <0xF0; sla+=2)
	{
		res = ite_smb_probe(ch, sla);
		if (!res)
			fprintf(stderr, "%02X ", sla);
	}
	fprintf(stderr, "\n");
}

//=============================================================================
//  read_ite_eeprom
//=============================================================================
void read_ite_eeprom(void)
{
	uint8_t		ch = 0;	// fixed ch=0
	uint8_t		res;
//	uint8_t		cmd;
//	uint8_t		dat[4];

#if 0
	cmd = 0x2;
	res = ite_smb_read_byte(ch, 0xA2, cmd, dat);
	if (res == ITE_OK)
		fprintf(stderr, "%02X ", dat[0]);
	else
		fprintf(stderr, "err");

	cmd = 0x3;
	res = ite_smb_read_byte(ch, 0xA2, cmd, dat);
	if (res == ITE_OK)
		fprintf(stderr, "%02X ", dat[0]);
	else
		fprintf(stderr, "err");

#endif

	res = ite_smb_write_quick(ch, 0xA2);
	if (res == ITE_OK)
		fprintf(stderr, "ok");
	else
		fprintf(stderr, "err");
	
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

	ite_smb_init();

	ite_smb_show();
	
	probe_ite_smb(0);
	probe_ite_smb(1);
	
	read_ite_eeprom();

	ite_exit();

	return 0;
}
