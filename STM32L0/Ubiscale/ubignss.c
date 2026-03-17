#include "ubignss.h"

#define JUNE2019_WEEKNUM	2056										//	default week number
#define SEC_IN_WEEK				604800									//	7*24*60*60	
#define JUNE							6-1											//	minus one for time.h 
#define START_YEAR				1980

time_t ubignss_get_time(gps_time_s time_in)
{
	uint16_t dweek;
	uint32_t dtime;
	struct tm ref_cal;
	time_t ref_time, out_time;
	
	ref_cal.tm_hour = 0;
	ref_cal.tm_min = 0;
	ref_cal.tm_sec = 0;
	ref_cal.tm_mday = 2;
	ref_cal.tm_mon = JUNE;
	ref_cal.tm_year = 119;
	
	ref_time = mktime(&ref_cal);
	
	if(time_in.time_validity == UBIGNSS_TIME_VALID)
	{
		dweek = time_in.week_number - JUNE2019_WEEKNUM;
		dtime = time_in.tow / 1000 + dweek * SEC_IN_WEEK;									//TOW is in milliseconds, SEC_IN_WEEK is in seconds
		
		out_time = ref_time + dtime;
	}
//	time_t out_time;
//	uint16_t year, mon, day;

//	if(time_in.time_validity == UBIGNSS_TIME_VALID)
//	{
//		year = time_in.week_number*7 / 365 + 1980;
//		mon = 
//	}
	
	
	return out_time;
}
