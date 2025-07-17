#include <stdio.h>
#include <stdint.h>

#include "ite.h"

//=============================================================================
//  ite_caseopen_show
//=============================================================================
void ite_caseopen_show(void)
{
	int			i;
	uint8_t		res;
	uint8_t		sts;

	fprintf(stderr, "Caseopen Status\n");

	if (ite->caseopen.num == 0)
		fprintf(stderr, "Caseopen device not found!\n");

	for (i=0; i<ite->caseopen.num; i++)
	{
		res = ite_caseopen_read_status(&sts);
		if (res != ITE_OK)
			fprintf(stderr, "ite_caseopen_read_status err! res=%d (%s)\n", res, ite_err_msg_str(res));
		else
			fprintf(stderr, "caseopen status = %02Xh\n", sts);
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

	ite_caseopen_init();

	ite_caseopen_show();

	ite_exit();

	return 0;
}
