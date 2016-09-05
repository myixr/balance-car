#ifndef _PLATFORM_STM32_
#define _PLATFORM_STM32_

#include <math.h>
#include "i2c.h"

#define __no_operation() __NOP()

static inline int i2c_write(unsigned char slave_addr, unsigned char reg_addr,
		unsigned char length, unsigned char const *data)
{
	//printf( "i2c write: %x, %x, %d\r\n", slave_addr, reg_addr, length );
	HAL_StatusTypeDef ret_status;
	ret_status = HAL_I2C_Mem_Write( &hi2c2, slave_addr<<1, reg_addr, I2C_MEMADD_SIZE_8BIT, (unsigned char*)data, length, 0xFFFF );
	if( ret_status == HAL_OK ){
	       return 0;	
	}
	else{
		return -1;
	}
}

static inline int i2c_read(unsigned char slave_addr, unsigned char reg_addr,
		unsigned char length, unsigned char *data)
{
	//printf( "i2c read: %x, %x, %d\r\n", slave_addr, reg_addr, length );
	HAL_StatusTypeDef ret_status;
	ret_status = HAL_I2C_Mem_Read( &hi2c2, slave_addr<<1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, length, 0xFFFF );
	if( ret_status == HAL_OK ){
	       return 0;	
	}
	else{
		return -1;
	}
}

static inline void delay_ms(unsigned long num_ms)
{
	HAL_Delay( num_ms );
}

static inline void get_ms(unsigned long *count)
{
	*count =  HAL_GetTick();
}

//static inline void reg_int_cb(void (*cb)(void), unsigned char port, unsigned char pin)
static inline void reg_int_cb(void (*cb)(void))
{
	/* TODO */
}

#include <math.h>
/*
inline long  labs(long x){ return (x>0? x : -x); }
inline float fabsf(float x){ return (x>0? x : -x); };
*/
static inline int   min(int a, int b){ return (a>b? b : a); }

#define log_i     printf
#define log_e     printf


#endif
