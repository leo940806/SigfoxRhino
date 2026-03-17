#ifndef __GPS_H
#define __GPS_H

#ifdef __cplusplus
 extern "C" {
#endif


//#include "stm32l0xx_hal.h"
#include "main.h"
#include "String.h"
#include "linkdata.h"
#include "stdlib.h"
#include "time.h"
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
/*-------------------------------------------------*/
#define UTC_OFFSET			2										//Zimbabwe UTC + 2hours
/*-------------------------------------------------*/
//GPS ON/OFF
#define GPS_ON 		1
#define GPS_OFF 	0

/*-------------------------------------------------*/
//GPS RX STATE
#define BEGIN	1
#define STOP	2
#define SEND	3
#define ERROR	4
#define IDLE	0
/*-------------------------------------------------*/
//GPS I2C define
#define gps_i2c			hi2c1
#define gps_dev			0x42
#define max_frame 	64
/*-------------------------------------------------*/
//GPS UART define
#define gps_uart		huart1
#define baud_9600		0
#define baud_19200	1
#define baud_115200	2
#define baud_921600	3
/*-------------------------------------------------*/
//UBX GNSS ID
#define GPS_ID					0
#define SBAS_ID					1
#define Galileo_ID			2
#define	BeiDou_ID				3
#define IMES_ID					4
#define QZSS_ID					5
#define GLONASS_ID			6
/*-------------------------------------------------*/
//UBX Header
#define UBX_HEADER_H	0xB5
#define UBX_HEADER_L	0x62
/*-------------------------------------------------*/
//UBX Class ID
#define	UBX_NAV_ID		0x01
#define	UBX_RXM_ID		0x02
#define	UBX_INF_ID		0x04
#define	UBX_ACK_ID		0x05
#define	UBX_CFG_ID		0x06
#define	UBX_UPD_ID		0x09
#define	UBX_MON_ID		0x0A
#define	UBX_AID_ID		0x0B
#define	UBX_TIM_ID		0x0D
#define	UBX_ESF_ID		0x10
#define	UBX_MGA_ID		0x13
#define	UBX_LOG_ID		0x21
#define	UBX_SEC_ID		0x27
#define	UBX_HNR_ID		0x28
/*-------------------------------------------------*/
//UBX CFG Message ID	
#define	CFG_PRT					0x00
#define	CFG_MSG					0x01
#define	CFG_INF					0x02
#define	CFG_RST					0x04
#define	CFG_DAT					0x06
#define	CFG_RATE				0x08
#define	CFG_CFG					0x09
#define	CFG_RXM					0x11
#define	CFG_ANT					0x13
#define	CFG_SBAS				0x16
#define	CFG_NMEA				0x17
#define	CFG_USB					0x1B
#define	CFG_ODO					0x1E
#define	CFG_NAVX5				0x23
#define	CFG_NAV5				0x24
#define	CFG_TP5					0x31
#define	CFG_RINV				0x34
#define	CFG_ITFM				0x39
#define	CFG_PM2					0x3B
#define	CFG_TMODE2			0x3D
#define	CFG_GNSS				0x3E
#define	CFG_LOGFILTER		0x47
#define	CFG_TXSLOT			0x53
#define	CFG_PWR					0x57
#define	CFG_HNR					0x5C
#define	CFG_ESRC				0x60
#define	CFG_DOSC				0x61
#define	CFG_SMGR				0x62
#define	CFG_GEOFENCE		0x69
#define	CFG_DGNSS				0x70
#define	CFG_TMODE3			0x71
#define	CFG_FIXSEED			0x84
#define	CFG_DYNSEED			0x85
#define	CFG_PMS					0x86
#define	CFG_BATCH				0x93
/*-------------------------------------------------*/
//UBX MGA Message ID	
#define MGA_ACK					0x60
#define MGA_ANO					0x20
#define MGA_BDS					0x03
#define MGA_DBD					0x80
#define MGA_FLASH 			0x21
#define MGA_GAL					0x02
#define MGA_GLO					0x06
#define MGA_GPS					0x00
#define MGA_INI					0x40
#define MGA_QZSS				0x05
/*-------------------------------------------------*/
#define GPS_COLD_START_SEL	0
#define GPS_WARM_START_SEL	1
#define GPS_HOT_START_SEL		2




typedef struct GPS_UBX_DATAFRAME
{
	uint8_t Sync_Char1;
	uint8_t Sync_Char2;
	uint8_t Class;
	uint8_t ID;
	uint8_t Payload_length_H;
	uint8_t Payload_length_L;
	const uint8_t *Payload;
	uint8_t CKA;
	uint8_t	CKB;
}gps_ubx_dataframe;


void gps_en(_Bool en, uint8_t gps_mode_sel);
void decodeNext(char** ptr);
void gps_decode(void);
void gps_strcpy(char *data, uint8_t size);
void gps_uart_tx(uint8_t *data, uint32_t bytecount);
void gps_sbas_config(const uint8_t* payload);
void gps_gnss_config(const uint8_t* payload);
void gps_rst_config(const uint8_t* payload);
void gps_pms_config(const uint8_t* payload);
void gps_stop(void);
void gps_i2c_send(uint8_t *data, uint8_t bytecount);
void gps_ubx_checksum(uint8_t *data, uint8_t bytecount);
void gps_nema_uart_tx(const uint8_t* data);	
void gps_baud_set(uint32_t baud_mode);
void gps_fill_dataframe(gps_ubx_dataframe gps_data, const uint8_t* payload);
void gps_get_lat_long(uint32_t* lati, uint32_t* longi);
uint8_t gps_nmea_checksum(const uint8_t *data, int count);
uint8_t hex2ascii(uint8_t input);
uint8_t ascii2hex(uint8_t input);
_Bool getfix(void);
void gps_get_time_date(struct tm* time_struct);
void gps_mga_config(const uint8_t* payload);
_Bool gps_test(void);
void gps_parameter_reset(void);
#endif

