#include <stdio.h>
#include <stdint.h>

#include "ite.h"

//=============================================================================
//  ite_fw_show
//=============================================================================
void ite_fw_show(void)
{
	fprintf(stderr, "Chip ID         : %04Xh\n", ite->hw.cid);
	fprintf(stderr, "Vendor ID       : %02Xh = %c\n", ite->hw.vid, ite->hw.vid);
	fprintf(stderr, "Chip Code       : %02Xh\n", ite->hw.fcc);
	
	fprintf(stderr, "\n");

	fprintf(stderr, "Project Name    : %s\n", ite->fw.prjname);
	fprintf(stderr, "Kernel Version  : %04Xh\n", ite->fw.kver);
	fprintf(stderr, "Table Code      : %02Xh\n", ite->fw.tbc);
	fprintf(stderr, "Chip Vendor     : %02Xh = %c\n", ite->fw.ccv, ite->fw.ccv);
	fprintf(stderr, "Chip ID         : %02Xh\n", ite->fw.ccd);

	fprintf(stderr, "\n");

	fprintf(stderr, "Project ID      : %02Xh\n", ite->fw.pid);
	fprintf(stderr, "Project Type    : %02Xh %c\n", ite->fw.pty, ite->fw.pty);
	fprintf(stderr, "Project Version : %04Xh\n", ite->fw.pver);
	
	fprintf(stderr, "\n");
	
	// <chip_vendor> <chip_id> <project id> <project_type> <project_version>
	fprintf(stderr, "Firmware Version: %c%02X%02X%c%04X\n\n", ite->fw.ccv, ite->fw.ccd, ite->fw.pid, ite->fw.pty, ite->fw.pver);
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

	ite_fw_show();

	ite_exit();

	return 0;
}
