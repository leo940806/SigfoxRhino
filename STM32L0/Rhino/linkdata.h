#ifndef LINKDATA_H
#define LINKDATA_H

#include "main.h"
#include "String.h"
// Configuration Params
//typedef struct 
//{
//	uint8_t 					ul_counter;
//	uint8_t 					test_mode;
//	uint8_t						rc_sel;
//	uint8_t 					cw_power;
//	uint8_t 					move_counter;
//	uint8_t 					move_ths;
//	uint8_t 					nomove_counter;
//	uint8_t 					nomove_ths;
//	
//}internal_t;

typedef struct 
{
  // general config																
  uint8_t         	product_ver;																			
  uint8_t         	info_code;										
	uint8_t         	time_ref;	
  uint8_t         	gps_mode;											
  uint8_t						gps_power_mode;								
  uint8_t         	acc_mode;											
  uint8_t		  			uplink_freq;									
  uint8_t						downlink_freq;								
  uint8_t						idle_mode;										
  uint8_t						gps_timeout;									
  uint8_t						acc_sensitivity;							
  uint8_t						no_movement_time;							
  uint8_t						positioning_freq;							
  uint8_t						scheduled_hour;			
	uint8_t 					acc_res_dur;
	uint8_t 					acc_nomove_ths;
	uint8_t 					acc_nomove_subths;
	uint8_t 					rf_output_power;
//	uint8_t 					ul_counter;
//	uint8_t 					test_mode;
//	uint8_t						rc_sel;
//	uint8_t 					cw_power;
//	uint8_t 					move_counter;
//	uint8_t 					move_ths;
//	uint8_t 					nomove_counter;
//	uint8_t 					nomove_ths;
  _Bool		  				no_movement;									
  _Bool							force_transition;							
  _Bool							led_active;										
  
} trackerparameter_t;

typedef struct
{
	uint32_t	gps_lat;
	uint32_t	gps_long;
	uint8_t		sensor1_val;
	uint8_t 	gps_acq_time;
	_Bool	 		no_movement;
	_Bool	 		GPS_state;
	
} sensordata_t;


#define SETUP 		0
#define GPS 			1
#define UBISCALE	2

#define SIGFOX_MSGLEN		12		//define max sigfox message length for message buffer size
#define UBI_PAYLOAD_LEN	10		//10 bytes of Ubiscale payload is used

void pack_ul_ubignss_payload(sensordata_t* data, uint8_t batt, uint8_t* ubi_buf, uint8_t* msgbuf );
void pack_ul_setup_payload(trackerparameter_t* para, uint8_t batt, uint8_t* msgbuf );
void pack_ul_gps_payload(sensordata_t* data, uint8_t batt, uint8_t* msgbuf );
void unpack_dl_payload(trackerparameter_t* para, uint8_t* msgbuf );

#endif

