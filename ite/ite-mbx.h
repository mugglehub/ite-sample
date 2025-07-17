#ifndef __ITE_MBX_H
#define __ITE_MBX_H

//=============================================================================
// mailbox: base offset
//=============================================================================
#define ITE_MBX_WR_BASE		0x50	// ADT  mailbox (write)
#define ITE_MBX_RD_BASE		0xA0	// ADT  mailbox (read)

#define ITE_MBX_ITE			0x80	// ITE  mailbox (hardware mbox)
#define ITE_MBX_ITE_0		0x00	// ITE  mailbox (hardware mbox) ?

//=============================================================================
// mailbox: reg offset
//=============================================================================
#define ITE_MBX_OFF_CMD		0
#define ITE_MBX_OFF_STS		1
#define ITE_MBX_OFF_PAR		2
#define ITE_MBX_OFF_DAT		3
#define ITE_MBX_OFF_DAT0	3	// DAT0
#define ITE_MBX_OFF_DAT1	4	// DAT1
#define ITE_MBX_OFF_DAT44	47	// DAT0~DAT44 : 3~47
#define ITE_MBX_OFF_LEN		47	// for structure len

#define ITE_MBX_WR_CMD		(ITE_MBX_WR_BASE + ITE_MBX_OFF_CMD)
#define ITE_MBX_WR_STS		(ITE_MBX_WR_BASE + ITE_MBX_OFF_STS)
#define ITE_MBX_WR_PAR		(ITE_MBX_WR_BASE + ITE_MBX_OFF_PAR)
#define ITE_MBX_WR_DAT		(ITE_MBX_WR_BASE + ITE_MBX_OFF_DAT)

#define ITE_MBX_RD_CMD		(ITE_MBX_RD_BASE + ITE_MBX_OFF_CMD)
#define ITE_MBX_RD_STS		(ITE_MBX_RD_BASE + ITE_MBX_OFF_STS)
#define ITE_MBX_RD_PAR		(ITE_MBX_RD_BASE + ITE_MBX_OFF_PAR)
#define ITE_MBX_RD_DAT		(ITE_MBX_RD_BASE + ITE_MBX_OFF_DAT)

#define ITE_MBX_ITE_CMD			(ITE_MBX_ITE + ITE_MBX_OFF_CMD)
#define ITE_MBX_ITE_STS			(ITE_MBX_ITE + ITE_MBX_OFF_STS)
#define ITE_MBX_ITE_PAR			(ITE_MBX_ITE + ITE_MBX_OFF_PAR)
#define ITE_MBX_ITE_DAT			(ITE_MBX_ITE + ITE_MBX_OFF_DAT)

//=============================================================================
// mailbox command
//=============================================================================
// SMBus
#define ITE_MBX_CMD_I2C_BUFRAM			0x01	// (48) I2C Write-Read ? with read buffer (256-bytes)
#define ITE_MBX_CMD_SMB_WR_QUICK		0x02	// (8)  Write Quick
#define ITE_MBX_CMD_SMB_RD_QUICK		0x03	// (4)  Read Quick
#define ITE_MBX_CMD_SMB_SEND_BYTE		0x04	// (8)  Send Byte
#define ITE_MBX_CMD_SMB_RECV_BYTE		0x05	// (8)  Receive Byte
#define ITE_MBX_CMD_SMB_WR_BYTE			0x06	// (8)  Write Byte
#define ITE_MBX_CMD_SMB_RD_BYTE			0x07	// (8)  Read Byte
#define ITE_MBX_CMD_SMB_WR_WORD			0x08	// (9)  Write Word
#define ITE_MBX_CMD_SMB_RD_WORD			0x09	// (9)  Read Word
#define ITE_MBX_CMD_SMB_WR_BLOCK		0x0A	// (48) Block Write
#define ITE_MBX_CMD_SMB_RD_BLOCK		0x0B	// (48) Block Read
#define ITE_MBX_CMD_SMB_PROC			0x0C	// (48) Process Call
#define ITE_MBX_CMD_I2C_MASS_WR_RD		0x0D	// (256 or 320??) need f/w support
#define ITE_MBX_CMD_I2C_RD_WR			0x0E	// (48) I2C Read-Write = I2C Write-Read ?
#define ITE_MBX_CMD_I2C_WR_RD			0x0F	// (48) I2C Write-Read = I2C Read-Write ?
#define ITE_MBX_CMD_I2C_BUFRAM_W		0xC4	// (48) I2C Write-Read ? with read buffer (256-bytes)

// hwpin
#define ITE_MBX_CMD_HWPIN_RD			0x11	// (8) hw pin read
#define ITE_MBX_CMD_HWPIN_WR			0x12	// (4) hw pin write

// ecram
#define ITE_MBX_CMD_ECRAM_RD			0x1E	// (48) ec ram read
#define ITE_MBX_CMD_ECRAM_WR			0x1F	// (48) ec ram write
#define 	ITE_ECRAM_TYP_ACPI			1		// ACPI RAM
#define 	ITE_ECRAM_TYP_HW			2		// HW RAM
#define 	ITE_ECRAM_TYP_EXT			3		// EXT RAM (external)
#define 	ITE_ECRAM_TYP_BUF			6		// BUFFER RAM (for EEPROM accessing)

// dynamic table
#define ITE_MBX_CMD_DYNATBL				0x20	// (48) get dynamic table
#define   MBX_DYNATBL_DID				(0)		//   device id
#define   MBX_DYNATBL_HWPIN				(1)		//   hardware pin
#define   MBX_DYNATBL_POL				(2)		//   polarity (active ?)
#define   MBX_DYNATBL_ITEM_MAX			45
#define PARA_DYNATBL_DID    			0
#define PARA_DYNATBL_HWPIN       		1
#define PARA_DYNATBL_ACTIVE      		2

// caseopen
#define ITE_MBX_CMD_CASEOPEN			0x27		// () caseopen
#define   ITE_CASEOPEN_SET_SWITCH			0x01	// caseopen set enable / disable
#define   ITE_CASEOPEN_GET_SWITCH			0x02	// caseopen get enable / disable
#define   ITE_CASEOPEN_GET_STATUS			0x03	// caseopen get case open status
#define   ITE_CASEOPEN_CLEAN_STATUS			0x04	// caseopen clean case open status
#define   ITE_CASEOPEN_SET_BEEP_ALARM		0x05	// caseopen set beep alarm enable / disable
#define   ITE_CASEOPEN_GET_BEEP_ALARM		0x06	// caseopen get beep alarm enable / disable

// gpio config
#define ITE_MBX_CMD_GPIOCFG_RD			0x30	// (4) gpio cfg read, read hw pin in/out/alt function
#define ITE_MBX_CMD_GPIOCFG_WR			0x31	// (4) gpio cfg write,write hw pin in/out/alt function

// i2c, smbus
#define ITE_MBX_CMD_SMB_FREQ_RD			0x34	// (5) SMBus freq read
#define ITE_MBX_CMD_SMB_FREQ_WR			0x35	// (5) SMBus freq write

// thermal zone
#define ITE_MBX_CMD_TZ_RD				0x42	// (48) thermal zone read
#define ITE_MBX_CMD_TZ_WR				0x43	// (48) thermal zone write
#define   TZ_TMP_NONE					(0)
#define   TZ_TMP_1L						(1)		// ?
#define   TZ_TMP_1R						(2)		// ?
#define   TZ_TMP_2L						(3)		// ?
#define   TZ_TMP_2R						(4)		// ?

// SMBus (PEC)
#define ITE_MBX_CMD_SMB_WR_QUICK_PEC	0x82	// (8) with PEC
#define ITE_MBX_CMD_SMB_RD_QUICK_PEC	0x83	// (4)
#define ITE_MBX_CMD_SMB_SEND_BYTE_PEC	0x84	// (8)
#define ITE_MBX_CMD_SMB_RECV_BYTE_PEC	0x85	// (8)
#define ITE_MBX_CMD_SMB_WR_BYTE_PEC		0x86	// (8)
#define ITE_MBX_CMD_SMB_RD_BYTE_PEC		0x87	// (8)
#define ITE_MBX_CMD_SMB_WR_WORD_PEC		0x88	// (9)
#define ITE_MBX_CMD_SMB_RD_WORD_PEC		0x89	// (9)
#define ITE_MBX_CMD_SMB_WR_BLOCK_PEC	0x8A	// (48)
#define ITE_MBX_CMD_SMB_RD_BLOCK_PEC	0x8B	// (48)

// buffer ram
#define ITE_MBX_CMD_BUFRAM_CLR			0xC0	// (2) buffer ram clear
#define ITE_MBX_CMD_BUFRAM_RD			0xC1	// (35) buffer ram read
#define ITE_MBX_CMD_BUFRAM_WR			0xC2	// (35) buffer ram write
#define ITE_MBX_CMD_BUFRAM_I2C_WR		0xC3	// (  ) read EEPROM data into 256 bytes buffer

#define BUFRAM_SZ						(256)
#define BUFRAM_PAGE_SZ					(32)

// version info
#define ITE_MBX_CMD_VER					0xF0	// (20) version
#define   MBX_VER_LEN					17
#define   ITE_FW_KVER_1408				0x1408	// ITE EC f/w structure version, kernel version = 14.8

// clear
#define ITE_MBX_CMD_CLEAR				0xFF	// (2) clear mailbox

//=============================================================================
//  functions
//=============================================================================
uint8_t ite_mbx_write(uint8_t off, uint8_t dat);
uint8_t ite_mbx_read(uint8_t off, uint8_t *dat);
uint8_t ite_mbx_wait(void);

uint8_t ite_hmbx_write(uint8_t off, uint8_t dat);
uint8_t ite_hmbx_write0(uint8_t off, uint8_t dat);
uint8_t ite_hmbx_read(uint8_t off, uint8_t *dat);
uint8_t ite_hmbx_wait(void);

uint8_t ite_mbx_wait_cmd_done(void);
uint8_t ite_mbx_clear(void);
uint8_t ite_mbx_trans(uint8_t cmd, uint8_t *wb, uint8_t *rb);

uint8_t ite_mbx_smb_trans(uint8_t cmd, uint8_t did, uint8_t sla, uint8_t smbcmd, uint8_t *wb, uint32_t wlen, uint8_t *rb, uint32_t *rlen);

#endif
