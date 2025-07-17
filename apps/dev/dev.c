#include <stdio.h>
#include <stdint.h>

#include "ite.h"

//=============================================================================
//  ite_dev_show
//=============================================================================
void ite_dev_show(void)
{
	int		i;

	fprintf(stderr, "device table : ");
	fprintf(stderr, " number of item = %d\n", ite->dt.num);
	
	if (ite->dt.num)
	{
		for (i=0; i<ite->dt.num; i++)
		{
			fprintf(stderr, "%2d : DID=%02Xh, PIN=%02Xh, POL=%02Xh, TYP=%d (%8s) > %s\n",
					i,
					ite->dt.dev[i].did,
					ite->dt.dev[i].pin,
					ite->dt.dev[i].pol,
					ite->dt.dev[i].typ,
					ite_dev_type_name(ite->dt.dev[i].typ),
					ite_dev_name(ite->dt.dev[i].did));
		}
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

	ite_dev_show();

	ite_exit();

	return 0;
}
