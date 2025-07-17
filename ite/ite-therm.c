#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ite.h"

//=============================================================================
//  ite_therm_read_tz
//=============================================================================
static uint8_t ite_therm_read_tz(uint8_t tz, uint8_t *smbid, uint8_t *fanid, uint8_t *buf, uint8_t *len)
{
	uint8_t		res;
	uint8_t		slen;
	uint8_t		wb[50];
	uint8_t		rb[50];


	memset(wb, 0, sizeof(wb));
	memset(rb, 0, sizeof(rb));

	//-------------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// wb : len,  par,  dlen, woff, wlen  dat0, dat1...........dat43, dat44
	// rb : rlen, roff, slen, soff, dat0, dat1.................dat44
	//-------------------------------------------------------------------------
	wb[0] = 2;		// wblen=2
	wb[1] = tz;		// par  : thermal zone no#

	rb[0] = 2;		// rlen : 2-bytes
	rb[1] = 0;		// roff : dat0=smbus_id, dat1=fan_id
	rb[2] = 255;	// slen : depends on fw
	rb[3] = 2;		// soff : dat2~ : structure

	// cmd issued
	res = ite_mbx_trans(ITE_MBX_CMD_TZ_RD, wb, rb);

	if (res != ITE_OK)
	{
		*smbid = 0;
		*fanid = 0;
		*buf  = 0;
		return res;
	}

	//---------------------------------------------------------------------
	//      <0>   <1>   <2>   <3>   <4>   <5>   <6>            <48>   <49>
	// rb : rlen, roff, slen, soff, dat0, dat1, dat2...........dat44
	//---------------------------------------------------------------------
	if (ite->fw.kver < ITE_FW_KVER_1408)
	{
		// kver < 14.8
		*smbid	= rb[4];	// dat0: smbid
		*fanid	= rb[5];	// dat1: fanid
		slen	= rb[2];	// structure length

		memcpy(buf, &rb[6], slen);	// 6=dat2~ : tz_structure = rbuf
	}
	else
	{
		// kver >= 14.8
		slen = rb[2];
		memcpy(buf, &rb[4], slen);
	}

	*len = slen;

	return ITE_OK;
}

//=============================================================================
//  ite_therm_init
//=============================================================================
uint8_t ite_therm_init(void)
{
	int			i, j;
	uint8_t		res;
	uint8_t		did;
	uint8_t		smbid;
	uint8_t		fanid;
	uint8_t		len;
	uint8_t		buf[50];
	ite_tz_t	*tz;

	ite->tz.num = 0;
	ite->tz.chnum = 0;
		
	if (ite->dt.num == 0)
		return ITE_ERR_NODEV;
	
	for (i=0; i<ite->dt.num; i++)
	{
		did = ite->dt.dev[i].did;
		
		if (did >= DEV_ID_TMP_CPU0 && did <= DEV_ID_TMP_SYS1)
		{
			ite->tz.dev[ ite->tznum ] = did;
			ite->tz.num++;
		}
			
	}

//	if (ite->tznum > 0)
//		return ITE_OK;
	
	
	// TZ: if not reporting did of TZ (90h~93h), we also check thermal zone to find out temperature.
	for (i=0; i<ITE_MAX_NUM_TZ; i++)
	{
		//res = ite_get_thermal_zone((uint8_t)i, &smbid, &fanid, buf, &len);
		res = ite_therm_read_tz((uint8_t)i, &smbid, &fanid, buf, &len);

		if (res != ITE_OK)
			continue;	

		tz = (ite_tz_t *)buf;

		if (tz->addr == 0)
			continue;	// skip: device not found if address == 0

		if (ite->hw.cid == ITE_CHIPID_8528 || ite->hw.cid == ITE_CHIPID_8518)
		{
			if (tz->sts & 0x06)
				continue;	// skip: bit[2]=i2cf, bit[1]=smblf
		}
		else if (ite->hw.cid == ITE_CHIPID_5121 || ite->hw.cid == ITE_CHIPID_5782)
		{
			if (tz->sts & 0x0E)
				continue;	// skip: bit[3]=pecif, bit[2]=i2cf, bit[1]=smblf
		}

		if (ite->tzchnum < ITE_MAX_NUM_TZ)
		{
			// ITE kernel version < 0x1408
			if (ite->fw.kver < ITE_FW_KVER_1408)
			{
				for (j=0; j<ite->tzchnum; j++)	//check duplicate
				{
					if (ite->tempdev[j] == (uint8_t)((tz->sts >> 5) & 0x07))	//skip duplicate temperature source
					{
						break;
					}
				}
				
				if (j != ite->tzchnum)
					continue;
					
				ite->tempdev[ ite->tzchnum ] = (uint8_t)i;		// tz no.
					
				// kernel_ver < 14.08h
				// 0=?, 1=SYS0_TZ_TEMP, 2=CPU0_TZ_TEMP, 3=SYS1_TZ_TEMP, 4=CPU1_TZ_TEMP, 
				ite->temptz[ ite->tzchnum ] = (uint8_t)((tz->sts >> 5) & 0x07);	// tz_type

				// CPU
				if (ite->temptz[ ite->tzchnum ] == 2 || ite->temptz[ ite->tzchnum ] == 4)
				{
					if (ite->tzcpunum < 2)
					{
						ite->tzcpudev[ ite->tzcpunum ] = (uint8_t)i;	// tz no.
						ite->tzcpunum++;
					}
				}

				// SYS
				if (ite->temptz[ ite->tzchnum ] == 1 || ite->temptz[ ite->tzchnum ] == 3)
				{
					if (ite->tzsysnum < 2)
					{
						ite->tzsysdev[ ite->tzsysnum ] = (uint8_t)i;	// tz no.
						ite->tzsysnum++;
					}
				}
			}
			// kernel_ver >= 14.08h
			else
			{
				for (j=0; j<ite->tzchnum; j++)	//check duplicate
				{
					if (ite->tempdev[j] == (uint8_t)tz->tmpid)	//skip duplicate temperature source
					{
						break;
					}
				}

				if (j != ite->tzchnum)
					continue;
					
				// kernel_ver >= 14.08h
				ite->tempdev[ ite->tzchnum ] = (uint8_t)tz->tmpid;	// did of temperature
					
				// CPU
				if (ite->tempdev[ ite->tzchnum ] == DEV_ID_TMP_CPU0 || ite->tempdev[ ite->tzchnum ] == DEV_ID_TMP_CPU1)
				{
					if (ite->tzcpunum < 2)
					{
						ite->tzcpudev[ ite->tzcpunum ] = ite->tempdev[ ite->tzchnum ];	// did: device id
						ite->tzcpunum++;
					}
				}

				// SYS
				if (ite->tempdev[ ite->tzchnum ] == DEV_ID_TMP_SYS0 || ite->tempdev[ ite->tzchnum ] == DEV_ID_TMP_SYS1)
				{
					if (ite->tzsysnum < 2)
					{
						ite->tzsysdev[ ite->tzsysnum ] = ite->tempdev[ ite->tzchnum ];	// did: device id
						ite->tzsysnum++;
					}
				}
			}

			ite->tzchdev[ ite->tzchnum ] = (uint8_t)i;

			// thermal mask
			ite->thermmask |= (1 << ite->tzchnum);

			// temperaure derived tz
			ite->tempch[ ite->tzchnum ] = ite->tzchnum;		// sn
			
			ite->tzchnum++;
		}
	}
	
	return ITE_OK;
}

//=============================================================================
//  ite_therm_read
//=============================================================================
uint8_t ite_therm_read_temp(uint8_t ch, int8_t *temp)
{
	uint8_t		res;
	uint8_t		len;
	uint8_t		smbid;
	uint8_t		fanid;
	uint8_t		buf[16];
	ite_tz_t	*tz;
	

	if (ch >= ITE_MAX_NUM_TZ)
		return ITE_ERR_CH;
	
	// n/a
	if ((ite->tz.chmask & (1<<ch)) == 0)
		return ITE_ERR_NODEV;

	if (ite->tz.num > 0)
	{
		res = ite_dt_read_hwpin(ite->tz.dev[ch], buf);
		ITE_ERR_CHECK(res);

		*temp = (int8_t)buf[0];	// unit: 1C

		return ITE_OK;
	}
	else if (ite->tz.chnum > 0)
	{
		//res = ite_read_thermal_zone(ch, &smbid, &fanid, buf, &len);
		res = ite_therm_read_tz(ch, &smbid, &fanid, buf, &len);
		ITE_ERR_CHECK(res);

		tz = (ite_tz_t *)buf;
		
		*temp = (int8_t)tz->tmp; // unit: 1C

		return ITE_OK;
	}
		
	return ITE_ERR_NODEV;
}

//=============================================================================
//  ite_therm_read_cputemp
//=============================================================================
uint8_t ite_therm_read_cputemp(uint8_t ch, int8_t *temp)
{
	uint8_t		res;
	uint8_t		len;
	uint8_t		smbid;
	uint8_t		fanid;
	uint8_t		buf[16];
	ite_tz_t	*tz;
	
	if (ch >= ite->tz.cpunum)
		return ITE_ERR_CH;

	if (ite->tz.num > 0)
	{
		res = ite_dt_read_hwpin(ite->tz.cpudev[ch], buf);
		ITE_ERR_CHECK(res);

		// 1C
		*temp = (int8_t)buf[0];

		return ITE_OK;
	}
	else if (ite->tz.chnum > 0)
	{
		// kver < 14.8
		if (ite->fw.kver < ITE_FW_KVER_1408)
		{
			res = ite_therm_read_tz(ite->tz.cpudev[ch], &smbid, &fanid, buf, &len);
			ITE_ERR_CHECK(res);

			tz = (ite_tz_t *)buf;
		
			*temp = (int8_t)tz->tmp;	// unit: 1C

			return ITE_OK;
		}
		// kver >= 14.8
		else
		{
			res = ite_dt_read_hwpin(ite->tz.cpudev[ch], buf);
			ITE_ERR_CHECK(res);

			*temp = (int8_t)buf[0];	// unit: 1C

			return ITE_OK;
		}
	}

	return ITE_ERR_NODEV;
}

//=============================================================================
//  ite_therm_read_systemp
//=============================================================================
uint8_t ite_therm_read_systemp(uint8_t ch, int8_t *temp)
{
	uint8_t		res;
	uint8_t		len;
	uint8_t		smbid;
	uint8_t		fanid;
	uint8_t		buf[16];
	ite_tz_t	*tz;

	if (ch >= ite->tz.sysnum)
		return ITE_ERR_CH;

	if (ite->tz.num > 0)
	{
		res = ite_dt_read_hwpin(ite->tz.sysdev[ch], buf);
		ITE_ERR_CHECK(res);

		*temp = (int8_t)buf[0];	// unit: 1C

		return ITE_OK;
	}
	else if (ite->tz.chnum > 0)
	{
		// kver < 14.8
		if (ite->fw.kver < ITE_FW_KVER_1408)
		{
			res = ite_therm_read_tz(ite->tz.sysdev[ch], &smbid, &fanid, buf, &len);
			ITE_ERR_CHECK(res);

			tz = (ite_tz_t *)buf;
		
			*temp = (int8_t)tz->tmp;	// unit: 1C

			return ITE_OK;
		}
		// kver >= 14.8
		else
		{
			res = ite_dt_read_hwpin(ite->tz.sysdev[ch], buf);
			ITE_ERR_CHECK(res);

			*temp = (int8_t)buf[0];	// unit: 1C

			return ITE_OK;
		}
	}

	return ITE_ERR_NODEV;
}