#include "stm32f1xx_hal.h"

typedef enum{
	LED1 = 0,
    LED2 = 1,
    LED3 = 2,
} LED_INDEX;

typedef enum{
	LED_ON,   /* turn on */
	LED_OFF,  /* turn off */ 
	LED_TURN, /* turn state */
} LED_CONTROL;

void led_init( void );
void led_control( LED_INDEX idx, LED_CONTROL ctrl );

