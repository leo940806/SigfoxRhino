#include "linkdata.h"

#define GPSMSG 1<<5
#define UBIMSG 2<<5


void pack_ul_setup_payload(trackerparameter_t* para, uint8_t batt, uint8_t* msgbuf ){
	
	for (uint8_t i = 0 ; i < SIGFOX_MSGLEN ; i++ ){
		msgbuf[i] = 0;
	}
	msgbuf[0] = batt&0x1F;
	msgbuf[1] |= (para->product_ver << 2)&0xFC;
	msgbuf[1] |= ((para->time_ref)&0x03);

	msgbuf[2] |= (para->info_code <<4)&0xf0;
	msgbuf[2] |= (para->gps_power_mode <<2)&0x0c;
	msgbuf[2] |= (para->acc_mode <<0)&0x03;
	
	msgbuf[3] |= (para->uplink_freq <<2)&0xfc;
	if(para->no_movement_time == 0x00)			//	0: off, 1: low power, 2: active, 3: not used
	{
		para->no_movement = 0;										//no movement interrupt not enable
	}
	else
	{
		para->no_movement = 1;										//no movement interrupt enable
	}
	if (para->no_movement){msgbuf[3] |= 2;}
	if (para->force_transition){msgbuf[3] |= 1;}
	
	msgbuf[4] |= (para->downlink_freq <<3)&0xf8;
	msgbuf[4] |= (para->idle_mode <<0)&0x07;
	
	msgbuf[5] = para->gps_timeout;
	
	msgbuf[6] = para->acc_sensitivity;
	
	msgbuf[7] = para->no_movement_time;
	
	msgbuf[8] |= (para->scheduled_hour<<3)&0xf8;
	if (para->led_active){msgbuf[8] |= 4;}
	
	if(para->rf_output_power > 2)
	{
		msgbuf[8] |= (para->rf_output_power-1)&0x03;
	}
	else if(para->rf_output_power > 4)
	{
		para->rf_output_power = 0;
		msgbuf[8] |= (para->rf_output_power)&0x03;
	}
	else
	{
		msgbuf[8] |= (para->rf_output_power)&0x03;
	}
	
	
}

void pack_ul_gps_payload(sensordata_t* data, uint8_t batt, uint8_t* msgbuf ){
	for (uint8_t i = 0 ; i< SIGFOX_MSGLEN ; i++ ){
		msgbuf[i] = 0;
	}
	msgbuf[0] = batt&0x1F;
	msgbuf[0] |= GPSMSG;
	
	if (data->no_movement)
	{
		msgbuf[1] |= 0x80;
	}
	if (data->GPS_state){msgbuf[1] |= 0x40;}
	msgbuf[1] |= (data->gps_acq_time <<0)&0x3f;
	
	msgbuf[2] = (uint8_t)(data->gps_lat >> 24);
	msgbuf[3] = (uint8_t)(data->gps_lat >> 16);
	msgbuf[4] = (uint8_t)(data->gps_lat >> 8);
	msgbuf[5] = (uint8_t)(data->gps_lat);
	msgbuf[6] = (uint8_t)(data->gps_long >> 24);
	msgbuf[7] = (uint8_t)(data->gps_long >> 16);
	msgbuf[8] = (uint8_t)(data->gps_long >> 8);
	msgbuf[9] = (uint8_t)(data->gps_long);
//	memcpy(&msgbuf[2], &data->gps_lat,4);
//	memcpy(&msgbuf[6], &data->gps_long,4);
	
	msgbuf[10] = data->sensor1_val;
	
}


void unpack_dl_payload(trackerparameter_t* para, uint8_t* msgbuf ){
	uint8_t decodebuf;
	
	para->gps_mode = (msgbuf[0]>>5)&0x07;
	
	decodebuf = (msgbuf[0]>>0)&0x1f;												
	if(decodebuf != 0x00)																				
	{																														//update only not equal to 0
		para->downlink_freq = decodebuf;													//else keep last config
	}
	
	para->time_ref = (msgbuf[1]>>6)&0x03;
	
	decodebuf = (msgbuf[1]>>0)&0x3f;
	if(decodebuf != 0x00)																				
	{																														//update only not equal to 0 
		para->uplink_freq = (msgbuf[1]>>0)&0x3f;									//else keep last config
	}
	
	para->gps_timeout = msgbuf[2];
	para->acc_sensitivity = msgbuf[3];
	para->no_movement_time = msgbuf[4];
	para->gps_power_mode = (msgbuf[5]>>6)&0x03;
	para->acc_mode = (msgbuf[5]>>4)&0x03;
	if ((msgbuf[5]&0x0c)!=0){
		para->force_transition = (msgbuf[5]>>2)&0x03;
	}
	para->led_active = (msgbuf[5]>>1)&0x01;
	
	decodebuf = (msgbuf[6]>>3)&0x1f;
	if((decodebuf > 0) && (decodebuf < 24))											//if 0 < hour < 24, config as schedule hour
	{																														//else keep last config
		para->scheduled_hour = decodebuf;
	}
	
	para->rf_output_power = msgbuf[6]&0x07;
}


void pack_ul_ubignss_payload(sensordata_t* data, uint8_t batt, uint8_t* ubi_buf, uint8_t* msgbuf )
{
	for (uint8_t i = 0 ; i< SIGFOX_MSGLEN ; i++ )
	{
		msgbuf[i] = 0;
	}
	msgbuf[0] = batt&0x1F;
	msgbuf[0] |= UBIMSG;
	if (data->no_movement)
	{
		msgbuf[1] |= 0x80;
	}
	
	if (data->GPS_state){msgbuf[1] |= 0x40;}
	msgbuf[1] |= (data->gps_acq_time <<0)&0x3f;
	
	for(uint8_t i = 0; i < UBI_PAYLOAD_LEN; i++)
	{
		msgbuf[i+2] = ubi_buf[i];
	}
	
}



