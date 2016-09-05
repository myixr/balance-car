/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "led.h"
#include "tim.h"

/* ----------------------- Defines ------------------------------------------*/
#define PDEBUG(fmt,args...)

/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    PDEBUG( "porttimer init %d\n", usTim1Timerout50us );
    vMBPortTimersDisable(  );
    __HAL_TIM_SET_AUTORELOAD( &htim2, usTim1Timerout50us );
    return TRUE;
}

inline void vMBPortTimersEnable(  )
{
    led_control( LED1, LED_TURN );
    __HAL_TIM_CLEAR_IT( &htim2, TIM_IT_UPDATE );
    __HAL_TIM_SET_COUNTER( &htim2, 0 );
	HAL_TIM_Base_Start_IT( &htim2 );
}

inline void vMBPortTimersDisable(  )
{
	HAL_TIM_Base_Stop_IT( &htim2 );
}

void porttimer_timeout( void ){
    led_control( LED2, LED_TURN );
    pxMBPortCBTimerExpired();
}