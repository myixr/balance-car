#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "led.h"

struct LedPin{
    GPIO_TypeDef *port;
    uint16_t      pin;
};

struct LedPin led_pins[] = {
    {
        .port = GPIOC,
        .pin  = GPIO_PIN_13,
    },
    {
        .port = GPIOC,
        .pin  = GPIO_PIN_14,
    },
    {
        .port = GPIOC,
        .pin  = GPIO_PIN_15,
    },
};

#define LED_COUNT (sizeof(led_pins)/sizeof(struct LedPin))

void led_init( void ){
    int idx;
    
    GPIO_InitTypeDef gpio_config;
    gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Speed = GPIO_SPEED_HIGH;
    
    for( idx=0; idx<LED_COUNT; ++idx ){
        gpio_config.Pin = led_pins[idx].pin;
        HAL_GPIO_Init( led_pins[idx].port, &gpio_config );
    }
}

void led_control( LED_INDEX idx, LED_CONTROL ctrl ){
    assert_param( ctrl==LED_ON || ctrl==LED_OFF || ctrl== LED_TURN );
    if( (int)idx >= LED_COUNT ){
        return;
    }

	uint16_t pin = led_pins[(int)idx].pin;
    GPIO_TypeDef *port = led_pins[(int)idx].port;
    
	if( ctrl == LED_ON ){
		HAL_GPIO_WritePin( port, pin, GPIO_PIN_RESET );
	}
	else if( ctrl == LED_OFF ){
		HAL_GPIO_WritePin( port, pin, GPIO_PIN_SET );
	}
	else if( ctrl == LED_TURN ){
		GPIO_PinState state = HAL_GPIO_ReadPin( port, pin );
		if( state == GPIO_PIN_SET ) state = GPIO_PIN_RESET;
		else state = GPIO_PIN_SET;
		HAL_GPIO_WritePin( port, pin, state );
	}
}
