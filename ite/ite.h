#ifndef __ITE_H
#define __ITE_H

#include "ite-mbx.h"
#include "ite-dt.h"
#include "acpi.h"

//=============================================================================
//  ITE
//=============================================================================
// cfg port: IT8528, IT8518, IT5121, IT5782
#define ITE_CFG_IDX			0x29C				// CFG_IDX (W)
#define ITE_CFG_DAT			0x29D				// CFG_DAT (RW)

// mbx port: IT5121
#define ITE_5121_CMD_PORT	0x29E				// ITE_CMD (W)
#define ITE_5121_STS_PORT	ITE_5121_CMD_PORT	// ITE_STS (R)
#define ITE_5121_DAT_PORT	0x29F				// ITE_DAT (RW)

// mbx port: IT5782
#define ITE_5782_CMD_PORT	0x29E				// ITE_CMD (W)
#define ITE_5782_STS_PORT	ITE_5782_CMD_PORT	// ITE_STS (R)
#define ITE_5782_DAT_PORT	0x29F				// ITE_DAT (RW)

// mbx port: IT8528
#define ITE_8528_CMD_PORT	0x29A				// ADT_CMD (W)
#define ITE_8528_STS_PORT	ITE_8528_CMD_PORT	// ADT_STS (R)
#define ITE_8528_DAT_PORT	0x299				// ADT_DAT (RW)

// mbx port: IT8518
#define ITE_8518_CMD_PORT	0x29A				// ADT_CMD (W)
#define ITE_8518_STS_PORT	ITE_8518_CMD_PORT	// ADT_STS (R)
#define ITE_8518_DAT_PORT	0x299				// ADT_DAT (RW)

// sts port
#define ITE_STS_OBF			0x01
#define ITE_STS_IBF			0x02

// retries
#define ITE_RETRIES					(100000)
#define ITE_IBF_RETRIES				(100000)
#define ITE_OBF_RETRIES				(100000)
#define ITE_MBX_BUSY_RETRIES		(100000)
#define ITE_MBX_CMD_DONE_RETRIES	(100000)
#define ITE_ERR_CHECK(res)			{ if (res) return res; }

// chip id
#define ITE_CHIPID_8518		0x8518
#define ITE_CHIPID_8528		0x8528
#define ITE_CHIPID_5121		0x5121
#define ITE_CHIPID_5782		0x5782

//=============================================================================
//  max number
//=============================================================================
#define ITE_MAX_NUM_TZ		(4)
#define ITE_MAX_NUM_GPIO	(16)
#define ITE_MAX_NUM_VOLT	(12)
#define ITE_MAX_NUM_CURR	(1)
#define ITE_MAX_NUM_BAT		(2)
#define ITE_MAX_NUM_BL		(2)
#define ITE_MAX_NUM_I2C		(8)
#define ITE_MAX_NUM_SMB		(8)
#define ITE_MAX_NUM_CASEOPEN	(1)

#define ITE_MAX_NUM_DEV		(45)

//=============================================================================
//  error code
//=============================================================================
#define ITE_OK				0
#define ITE_ERR_OBF			1
#define ITE_ERR_OBE			2
#define ITE_ERR_IBF			3
#define ITE_ERR_CHIPID		4
#define ITE_ERR_VENDORID	5
#define ITE_ERR_FWCHIPID	6
#define ITE_ERR_CH			7
#define ITE_ERR_NODEV		8
#define ITE_ERR_MBX_TMOUT	9
#define ITE_ERR_RAM_SIZE	10
#define ITE_ERR_TMOUT_MBX	11
#define ITE_ERR_SIZE		12
#define ITE_ERR				13


#define ITE_ERR_MBX					0x20
#define ITE_ERR_MBX_FAIL			ITE_ERR_MBX
#define ITE_ERR_MBX_UNDEF_ITEM		0x21
#define ITE_ERR_MBX_UNDEF_DID		0x22
#define ITE_ERR_MBX_HWPIN_TYPE		0x23


// added mbx error code for ITE SMBus
#define	ITE_SMB_DONE				0x80

#define ITE_SMB_ERR_MASK			(0x7F)

#define ITE_SMB_ERR_EE_DATA			0x76
#define ITE_SMB_ERR_SLV_CH			0x75
#define ITE_SMB_ERR_PG_WR_SZ		0x74
#define ITE_SMB_ERR_NO_DEF_ADDR		0x73
#define ITE_SMB_ERR_UN_DEV_TYP		0x72
#define ITE_SMB_ERR_NOT_I2C_DEV		0x71
#define ITE_SMB_ERR_DEV_ID_FAIL		0x70

#define ITE_SMB_ERR_ALM				0x40

#define ITE_SMB_ERR_MBX_BUF_READ	0x2A
#define ITE_SMB_ERR_MBX_CMD_DONE	0x29
#define ITE_SMB_ERR_MBX_BUF_CLEAR	0x28
#define ITE_SMB_ERR_MBX_DAT_READ	0x27
#define ITE_SMB_ERR_MBX_DAT_WRITE	0x26
#define ITE_SMB_ERR_MBX_SMBCMD		0x25
#define ITE_SMB_ERR_MBX_SLA  		0x24
#define ITE_SMB_ERR_MBX_PAR  		0x23
#define ITE_SMB_ERR_MBX_CLEAR		0x22
#define ITE_SMB_ERR_MBX_STS			0x21
#define ITE_SMB_ERR_MBX_CMD			0x20

#define ITE_SMB_ERR_PEC				0x1F
#define ITE_SMB_ERR_BUSY			0x1A
#define ITE_SMB_ERR_PROT_NA			0x19
#define ITE_SMB_ERR_TMOUT			0x18
#define ITE_SMB_ERR_DEV_ACC_DENY	0x17
#define ITE_SMB_ERR_UNKNOWN			0x13
#define ITE_SMB_ERR_CMD_ACC_DENY	0x12
#define ITE_SMB_ERR_CMD_NOT_ACK		0x11
#define ITE_SMB_ERR_ADDR_NOT_ACK	0x10

#define ITE_SMB_ERR_CHECK(res, errcode)		{ if (res != ITE_OK) return errcode; }


//=============================================================================
//  ite_port_t
//=============================================================================
typedef struct _ite_port_t
{
	// port address
	uint16_t	cmd;	// command port
	uint16_t	sts;	// status port
	uint16_t	dat;	// data port

} ite_port_t;

//=============================================================================
//  ite_hw_t
//=============================================================================
typedef struct _ite_hw_t
{
	uint16_t	cid;	// chip id
	uint8_t		vid;	// vendor id
	uint8_t		fcc;	// fw chip code
	
} ite_hw_t;

//=============================================================================
//  ite_fw_t
//=============================================================================
typedef struct _ite_fw_t
{
	char		prjname[16];	// project name
	uint16_t	kver;			// kernel version
	uint8_t		tbc;			// table code
	uint8_t		ccv;			// chip vendor
	uint8_t		ccd;			// chip id
	uint8_t		pid;			// project id
	uint8_t		pty;			// project type
	uint16_t	pver;			// project version

} ite_fw_t;

//===========================================================================
//  ite_mbx_t
//===========================================================================
typedef struct _ite_mbx_t
{
	// mbx operator
	uint8_t		(*write)(uint8_t off, uint8_t dat);
	uint8_t		(*read)(uint8_t off, uint8_t *dat);
	uint8_t		(*wait)(void);

	// buffer ram size
	uint16_t	brsz;

} ite_mbx_t;

//=============================================================================
//  ite_dev_nm_t
//=============================================================================
typedef struct _ite_dev_nm_t
{
	uint8_t		did;	// device id
	uint8_t		typ;	// type
	char		*name;
	
} ite_dev_nm_t;

//=============================================================================
//  ite_dev_t
//=============================================================================
typedef struct _ite_dev_t
{
	uint8_t		did;	// device id
	uint8_t		pin;	// hardware pin
	uint8_t		pol;	// polarity
	uint8_t		typ;	// device type

} ite_dev_t;

//=============================================================================
//  ite_dt_t
//=============================================================================
typedef struct _ite_dt_t
{
	uint8_t		num;
	ite_dev_t	dev[ITE_MAX_NUM_DEV];

} ite_dt_t;

//=============================================================================
//  ite_tz_t
//=============================================================================
typedef struct _ite_tz_t
{
	uint8_t		num;						// tznum
	uint8_t		ch[ ITE_MAX_NUM_TZ ];		// tzch
	uint8_t		dev[ITE_MAX_NUM_TZ ];		// tzdev
	
	uint8_t		chnum;						// tzchnum
	uint8_t		chdev[ ITE_MAX_NUM_TZ ];	// tzchdev
	uint16_t	chmask;						// thermmask;

	uint8_t		tempch[ ITE_MAX_NUM_TZ ];	// tempch
	uint8_t		tempdev[ ITE_MAX_NUM_TZ ];	// tempdev
	uint8_t		temptz[ ITE_MAX_NUM_TZ ];	// temptz

	uint8_t		cpunum;						// tzcpunum
	uint8_t		cpudev[2];					// tzcpudev
	
	uint8_t		sysnum;						// tzsysnum
	uint8_t		sysdev[2];					// tzsysdev

} ite_tz_t;

//=============================================================================
//  ite_gpio_t
//=============================================================================
typedef struct _ite_gpio_t
{
	uint8_t		num;
	uint8_t		dev[ITE_MAX_NUM_GPIO];
	uint16_t	mask;

} ite_gpio_t;
	
//=============================================================================
//  ite_volt_t
//=============================================================================
typedef struct _ite_volt_t
{
	uint8_t		num;
	uint8_t		dev[ITE_MAX_NUM_VOLT];
	uint16_t	mask;

} ite_volt_t;

//=============================================================================
//  ite_curr_t
//=============================================================================
typedef struct _ite_curr_t
{
	uint8_t		num;
	uint8_t		dev[ITE_MAX_NUM_CURR];
	uint8_t		mask;

} ite_curr_t;

//=============================================================================
//  ite_bat_t
//=============================================================================
typedef struct _ite_bat_t
{
	uint8_t		num;
	uint8_t		dev[ITE_MAX_NUM_BAT];
	uint8_t		mask;

} ite_bat_t;

//=============================================================================
//  ite_bl_t
//=============================================================================
typedef struct _ite_bl_t
{
	uint8_t		num;
	uint8_t		dev[ITE_MAX_NUM_BL];
	uint8_t		mask;

} ite_bl_t;

//=============================================================================
//  ite_i2c_t
//=============================================================================
typedef struct _ite_i2c_t
{
	uint8_t		num;
	uint8_t		dev[ITE_MAX_NUM_I2C];
	uint8_t		mask;

} ite_i2c_t;

//=============================================================================
//  ite_smb_t
//=============================================================================
typedef struct _ite_smb_t
{
	uint8_t		num;
	uint8_t		dev[ITE_MAX_NUM_SMB];
	uint8_t		mask;

} ite_smb_t;

//=============================================================================
//  ite_caseopen_t
//=============================================================================
typedef struct _ite_caseopen_t
{
	uint8_t		num;
	uint8_t		dev[ITE_MAX_NUM_CASEOPEN];
	uint8_t		mask;

} ite_caseopen_t;

	
//=============================================================================
//  ite_t
//=============================================================================
typedef struct _ite_t
{
	ite_port_t		port;	// port
	ite_hw_t		hw;		// hw info
	ite_fw_t		fw;		// fw info
	ite_mbx_t		mbx;	// mailbox
	ite_dt_t		dt;		// dynamic table
	ite_tz_t		tz;		// thermal zone
	ite_gpio_t		gpio;	// gpio
	ite_volt_t		volt;	// volt
	ite_curr_t		curr;	// current
	ite_bat_t		bat;	// battery
	ite_bl_t		bl;		// backlight
	ite_i2c_t		i2c;	// i2c
	ite_smb_t		smb;	// smbus
	ite_caseopen_t	caseopen;	// caseopen

} ite_t;

//=============================================================================
//  extern
//=============================================================================
extern ite_t	ite_ent;
extern ite_t	*ite;

//=============================================================================
//  functions
//=============================================================================
uint8_t ite_wait_ibe(void);
uint8_t ite_wait_obf(void);

char*	ite_err_msg_str(uint8_t errcode);

void	ite_exit(void);
uint8_t ite_init(void);

uint16_t swap16(uint16_t data);

//=============================================================================
//  function api
//=============================================================================
// bat
uint8_t ite_bat_ac_status(void);
uint8_t ite_bat_status(uint8_t bat);
void	ite_bat_init(void);

// bl
uint8_t ite_bl_sw_read(uint8_t ch, uint8_t *sw);
uint8_t ite_bl_sw_write(uint8_t ch, uint8_t sw);
uint8_t ite_bl_pwm_read(uint8_t ch, uint8_t *pwm);
uint8_t ite_bl_pwm_write(uint8_t ch, uint8_t pwm);
uint8_t ite_bl_init(void);

// caseopen
uint8_t ite_caseopen_read(uint8_t *det);
uint8_t ite_caseopen_write(uint8_t *det);
uint8_t ite_caseopen_read_status(uint8_t *sts);
uint8_t ite_caseopen_clear_status(void);
void	ite_caseopen_init(void);

// curr
uint8_t ite_curr_read(uint8_t ch, double *curr);
void	ite_curr_init(void);

// gpio
uint8_t ite_gpio_get_dir(uint8_t ch, uint8_t *dir);
uint8_t ite_gpio_set_dir(uint8_t ch, uint8_t dir);
uint8_t ite_gpio_get_dat(uint8_t ch, uint8_t *dat);
uint8_t ite_gpio_set_dat(uint8_t ch, uint8_t dat);
void	ite_gpio_init(void);

// i2c
uint8_t ite_i2c_set_clock(uint8_t ch, uint16_t clk);
uint8_t ite_i2c_probe(uint8_t ch, uint8_t sla);
uint8_t ite_i2c_trans(uint8_t ch, uint8_t sla, uint8_t *wdat, uint32_t wlen, uint8_t *rbuf, uint32_t *rlen);
void	ite_i2c_init(void);

// ram
uint8_t ite_ram_read(uint8_t rt, uint8_t off, uint8_t sz, uint8_t *dat);
uint8_t ite_ram_write(uint8_t rt, uint8_t off, uint8_t sz, uint8_t *dat);

// smb
uint8_t ite_smb_set_clock(uint8_t ch, uint16_t clk);
uint8_t ite_smb_probe(uint8_t ch, uint8_t sla);
uint8_t ite_smb_read_quick(uint8_t ch, uint8_t sla);
uint8_t ite_smb_write_quick(uint8_t ch, uint8_t sla);
uint8_t ite_smb_receive_byte(uint8_t ch, uint8_t sla, uint8_t *dat);
uint8_t ite_smb_send_byte(uint8_t ch, uint8_t sla, uint8_t *dat);
uint8_t ite_smb_read_byte(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat);
uint8_t ite_smb_write_byte(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat);
uint8_t ite_smb_read_word(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat);
uint8_t ite_smb_write_word(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat);
uint8_t ite_smb_block_read(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat, uint8_t *blen);
uint8_t ite_smb_block_write(uint8_t ch, uint8_t sla, uint8_t cmd, uint8_t *dat, uint8_t blen);
void	ite_smb_init(void);

// volt
uint8_t ite_volt_read(uint8_t ch, double *volt);
void	ite_volt_init(void);

#endif