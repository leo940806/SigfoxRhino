#include "lis2de.h"
#include "linkdata.h"

uint8_t xyz_status = 0x00, temp_status = 0x00;
uint8_t X_H = 0, Y_H = 0, Z_H = 0;
uint8_t accel_xyz_reg[3]        = {OUT_X_H, OUT_Y_H, OUT_Z_H};
uint8_t accel_temp_reg[2]       = {OUT_TEMP_H, OUT_TEMP_L};

extern trackerparameter_t tracker_data;
extern uint8_t nomove_counter;

uint8_t accel_init_reg[accel_num_config * 2] = 
{
	CTRL_REG0, 			0x90,
	TEMP_CFG_REG, 	0xC0,
	CTRL_REG1, 			ODR_1HZ,
	CTRL_REG2, 			0x01,
	CTRL_REG3, 			0x40,
	CTRL_REG4, 			0x80,
	CTRL_REG5, 			0x0A,
	CTRL_REG6, 			0x0A,
  REFERENCE,      0x00,
	FIFO_CTRL_REG, 	0x00,
	INT1_CFG,				0x00,
  INT1_THS,				0x00,
  INT1_DURATION,	0x00,
	INT2_CFG, 			0x15,
	INT2_THS,				0x02,
  INT2_DURATION,	0x0A,
	CLICK_CFG,			0x00,
  ACT_THS,        0x30,  //0x10
  ACT_DUR,        0xff
}; 

uint8_t accel_sleep_reg[accel_num_config * 2] = 
{

	TEMP_CFG_REG, 	0xC0,
	CTRL_REG0, 			0x90,
	CTRL_REG1, 			0x00,
	CTRL_REG2, 			0x00,
	CTRL_REG3, 			0x00,
	CTRL_REG4, 			0x80,
	CTRL_REG5, 			0x00,
	CTRL_REG6, 			0x00,
  REFERENCE,      0x00,
	FIFO_CTRL_REG, 	0x00,
	INT1_CFG,				0x00,
  INT1_THS,				0x00,
  INT1_DURATION,	0x00,
	INT2_CFG, 			0x00,
  INT2_THS,				0x00,
  INT2_DURATION,	0x00,
	CLICK_CFG,			0x00,
  ACT_THS,        0x00,  //0x10
  ACT_DUR,        0x00
}; 



//void accel_get_count()
//{
//  uint8_t *aptr , addr = INT_COUNTER_REG;

//  accel_i2c_rx(&addr, aptr);
//  
//  INT_COUNT = *aptr;

//}

/*-------------------------------------------------*/
void accel_init()
{
	int i = 0;

	
	for(i = 0; i < accel_num_config*2; i++)
	{
		accel_i2c_tx(accel_init_reg+i, accel_init_reg+i+1);
    while(accel_i2c.Instance->ISR & I2C_ISR_TC != I2C_ISR_TC);
		i++;
	}
}
/*-------------------------------------------------*/
void accel_sleep()
{
	int i = 0;
	for(i = 0; i < accel_num_config*2; i++)
	{
		accel_i2c_tx(accel_sleep_reg+i, accel_sleep_reg+i+1);
    while(accel_i2c.Instance->ISR & I2C_ISR_TC != I2C_ISR_TC);
		i++;
	}
}
/*-------------------------------------------------*/
void accel_i2c_rx(uint8_t *addr, uint8_t *data)
{
        uint32_t time = HAL_GetTick();
	while(HAL_I2C_Master_Transmit_IT(&accel_i2c, accel_dev, addr, 1)!=HAL_OK)
        {
//          if(time - HAL_GetTick() > 100)
//          {
//            time = HAL_GetTick();
//            break;
//          }
        }
	while(HAL_I2C_Master_Receive_IT(&accel_i2c, accel_dev, data, 1)!=HAL_OK)
        {
//          if(time - HAL_GetTick() > 100)
//          {
//            time = HAL_GetTick();
//            break;
//          }
        }
}
/*-------------------------------------------------*/
void accel_i2c_tx(uint8_t *addr, uint8_t *data)
{
	uint8_t ptr[2];
	ptr[0] = *addr;
	ptr[1] = *data;
        uint32_t time = HAL_GetTick();
	while(HAL_I2C_Master_Transmit_IT(&accel_i2c, accel_dev, ptr, 2)!=HAL_OK);
//        {
//          if(time - HAL_GetTick() > 100)
//          {
//            time = HAL_GetTick();
//            break;
//          }
//        }
}

/*-------------------------------------------------*/
void accel_get_xyz()
{
	
	uint8_t i = 0,  addr = STATUS_REG;
	
	accel_i2c_rx(&addr, &xyz_status);
	
	if((xyz_status & ZYXDA_mask) == ZYXDA_mask)
	{
	
	accel_i2c_rx(accel_xyz_reg+i, &X_H);
	i++;

	accel_i2c_rx(accel_xyz_reg+i, &Y_H);
	i++;

	accel_i2c_rx(accel_xyz_reg+i, &Z_H);
	i++;
	}
	else
	{
		
	}

}
/*-------------------------------------------------*/
float accel_get_temp()
{
	uint8_t addr = 0x00, data = 0x00;
	uint8_t temph = 0, templ = 0;
	float temperature = 0;
	
	addr = CTRL_REG4;						// BDU of CR4 set to 1, to enable temperature sensor, but the xyz will stop
	data = 0x80;
	accel_i2c_tx(&addr, &data);
	
	addr = STATUS_REG_AUX;
	accel_i2c_rx(&addr, &temp_status);
//	while((temp_status & TDA_mask) != TDA_mask);
		
//		accel_sleep();
	accel_i2c_rx(accel_temp_reg, &temph);
	accel_i2c_rx(accel_temp_reg+1, &templ);

	temperature =  (float)((int16_t)(temph << 8) + (int16_t)(templ)) / 256.0;

	return(temperature);
	
}
/*-------------------------------------------------*/
void accel_reboot()
{
        uint8_t addr = CTRL_REG5, data = 0x80;
        accel_i2c_tx(&addr, &data);
        HAL_Delay(5);
        data = 0x00;
        accel_i2c_tx(&addr, &data);
}

/*-------------------------------------------------*/
void accel_reset_act_int2()
{
	uint8_t addr, txdata, rst_data = 0x00, rxdata;

	uint32_t accel_timeout;
//	buf = (tracker_data.no_movement_time * 60 )/8 - 1;
//	txdata = (uint8_t)buf;
	txdata = (uint8_t)0x07;	
	addr = INT2_SRC;
	
//	while(HAL_GPIO_ReadPin(ACCEL_INT2_GPIO_Port, ACCEL_INT2_Pin))
//	{
//		accel_i2c_rx(&addr, &rxdata);
//	}
	accel_i2c_rx(&addr, &rxdata);
	HAL_Delay(100);
	
	addr = ACT_DUR;
	accel_timeout = HAL_GetTick();
	while(HAL_GPIO_ReadPin(ACCEL_INT2_GPIO_Port, ACCEL_INT2_Pin))
	{
		accel_i2c_tx(&addr, &rst_data);
		if(HAL_GetTick() - accel_timeout > 2000)
		{
			break;
		}
	}
	
	accel_i2c_tx(&addr, &txdata);
}
/*-------------------------------------------------*/
void accel_set_sensitivity(uint8_t sensitivity)
{
	uint8_t addr, data;

	
//	buf = (duration * 60) / 8 - 1;
	addr = ACT_THS;
	data = sensitivity;
	
	for(uint8_t i = 0; i < 5; i++)
	{
		addr = ACT_THS;
		data = sensitivity;
		accel_i2c_tx(&addr, &data);
		HAL_Delay(10);
//		addr = ACT_DUR;
//		data = buf;
//		accel_i2c_tx(&addr, &data);
//		HAL_Delay(10);
	}
//	
//	accel_i2c_rx(&addr, &rxdata);
	
}
/*-------------------------------------------------*/
void accel_set_act_duration(uint8_t duration)
{
	uint8_t* addr, *data;
	uint16_t buf;
	*addr = ACT_DUR;
	buf = ((duration * 60 )/8) - 1;
	
	*data = (uint8_t)buf;
	
	for(uint8_t i = 0; i < 5; i++)
	{
		accel_i2c_tx(addr, data);
		HAL_Delay(10);
	}
//	
//	accel_i2c_rx(&addr, &rxdata);
	
}
/*-------------------------------------------------*/
_Bool accel_test(void)
{
	uint8_t addr = WHO_AM_I, data;
	uint8_t tcount = 0;
	
	while((data != ACCEL_VER) && (tcount < 10))			//Check the Accelerometer I2C 
	{																								//if it succeeds, it breaks the while loop: 						return 1
		accel_i2c_rx(&addr, &data);										//if it fails 10 times, it breaks the while loop: 			return 0
		tcount ++;		
	}
	
	if(data == ACCEL_VER)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



