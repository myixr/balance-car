#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "stm32f1xx_hal.h"
#include "user.h"
#include "led.h"
#include "usart.h"
#include "mpu6050.h"
#include "motor_driver.h"

#include "eMPL/inv_mpu.h"
#include "eMPL/inv_mpu_dmp_motion_driver.h"

#include "mb.h"
#include "tim.h"

#define PDEBUG( msg, args... ) printf( msg, ##args )

extern UART_HandleTypeDef huart1;

//#define PORTTIMER_TEST
#define MOTOR_TEST

#ifdef PORTTIMER_TEST
static void porttimer_test( void );
static volatile int porttimer_cnt = 0;
static volatile int porttimer_istest = 0;
#endif

#ifdef MOTOR_TEST
static void motor_test( void );
#endif

void user_init1(){
}

void user_init2(){
    PDEBUG( "user init2 start\r\n" );
    HAL_UART_Transmit( &huart1, "This is comm serial\n", 20, 0xFFFF );
	
    const UCHAR     ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
    eMBErrorCode    eStatus;
	
    PDEBUG( "LED initialize.\n" );
    led_init();
    led_control( LED1, LED_ON );
    HAL_Delay(1000);
	led_control( LED1, LED_OFF );
    HAL_Delay( 1000 );
    led_control( LED1, LED_ON );
    
    led_control( LED1, LED_OFF );
    led_control( LED2, LED_OFF );
    led_control( LED3, LED_OFF );
    
    //PDEBUG( "MPU6050 initialize.\n" );
    //mpu6050_init();
    
    PDEBUG( "BM initialize.\n" );
    eStatus = eMBInit( MB_RTU, 0x0A, 0, 115200, MB_PAR_NONE );
    if( eStatus != MB_ENOERR ){
        PDEBUG( "Error code: %d\n", eStatus );
    }
    PDEBUG( "MB set slave ID\n" );
    eStatus = eMBSetSlaveID( 0x34, TRUE, ucSlaveID, 3 );
    if( eStatus != MB_ENOERR ){
        PDEBUG( "Error code: %d\n", eStatus );
    }
    //sei(  );
    
    #ifdef PORTTIMER_TEST
    PDEBUG( "start porttimer test\n" );
    porttimer_test();
    #endif
    
    motordrv_init();
    #ifdef MOTOR_TEST
    PDEBUG( "start motor test\n" );
    motor_test();
    #endif

    /* Enable the Modbus Protocol Stack. */
    PDEBUG( "MB enable.\n" );
    eStatus = eMBEnable(  );
    if( eStatus != MB_ENOERR ){
        PDEBUG( "Error code: %d\n", eStatus );
    }
}

void user_loop(){
	
    PDEBUG( "user loop start\n" );

    for( ;; )
    {
        ( void )eMBPoll(  );
        /* Here we simply count the number of poll cycles. */
        // usRegInputBuf[0]++;
    }
}

int fputc(int ch, FILE *f)
{
	unsigned char c = ch;
	if( c == '\n' ){
		unsigned char c = '\r';
		HAL_UART_Transmit( &huart2, &c, 1, 0xFFFF );
	}
	HAL_UART_Transmit( &huart2, &c, 1, 0xFFFF );
  
  return ch;
}

eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress,
                               USHORT usNRegs )
{
    /* TODO */
    PDEBUG( "read input %d,%d\n", usAddress, usNRegs );
    return MB_ENOERR;
}

static void tmp_fun( void ){
		short gyro[3],accel[3];
	long quat[4];
	float w,x,y,z;
	unsigned long timestamp;
	short sensors;
	unsigned char more;
	dmp_read_fifo(gyro, accel, quat, &timestamp, &sensors, &more);
	//printf( "sensors=%d\r\n", sensors );
	if( sensors & INV_WXYZ_QUAT ){
		//printf( "%d,%d,%d,%d\r\n", quat[0],quat[1],quat[2],quat[3] );
		//http://www.cnblogs.com/wqj1212/archive/2010/11/21/1883033.html
		w = quat[0]/1073741824.0f;
		x = quat[1]/1073741824.0f;
		y = quat[2]/1073741824.0f;
		z = quat[3]/1073741824.0f;

		float a = atan2( 2*(w*x+y*z),1-2*(x*x+y*y) );
		float b = asin( 2*(w*y-z*x) );
		float c = atan2( 2*(w*z+x*y),1-2*(y*y+z*z) );
		printf( "%f,%f,%f\n", a, b, c );
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if( htim == &htim2 ){
        #ifdef PORTTIMER_TEST
        if( porttimer_istest ){
            porttimer_cnt++;
        }
        else{
            porttimer_timeout();
        }
        #else
        porttimer_timeout();
        #endif
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if( huart == &huart1 ){
        portserial_rxISR();
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    if( huart == &huart1 ){
        portserial_txISR();
    }
}

#ifdef PORTTIMER_TEST
void porttimer_test(){
    porttimer_cnt = 0;
    porttimer_istest = 1;
    PDEBUG( "porttimer test: should blink 5 timers\n" );
    xMBPortTimersInit( 20 );
    vMBPortTimersEnable();
    int idx;
    for( idx=0; idx<10; ++idx ){
        while( porttimer_cnt < 500 ){}
        porttimer_cnt = 0;
        led_control( LED1, LED_TURN );
    }
    vMBPortTimersDisable();
    
    PDEBUG( "porttimer test: no blink for 2 seconds\n" );
    uint32_t tick = HAL_GetTick();
    while((HAL_GetTick() - tick) < 2000 ){
        if( porttimer_cnt >= 500 ){
            porttimer_cnt = 0;
            led_control( LED1, LED_TURN );
        }
    }
    
    PDEBUG( "porttimer test done\n" );
    porttimer_istest = 0;
}
#endif

#ifdef MOTOR_TEST
void motor_test( void ){
    for( int idx=0; idx<1000; ++idx ){
        HAL_Delay( 5 );
        motordrv_control( &motor_A, idx );
    }
    for( int idx=1000; idx>0; --idx ){
        HAL_Delay( 5 );
        motordrv_control( &motor_A, idx );
    }
    
    for( int idx=0; idx<1000; ++idx ){
        HAL_Delay( 5 );
        motordrv_control( &motor_B, idx );
    }
    for( int idx=1000; idx>0; --idx ){
        HAL_Delay( 5 );
        motordrv_control( &motor_B, idx );
    }
}
#endif
