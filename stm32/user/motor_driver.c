#include "motor_driver.h"
#include "tim.h"

motordrv_t motor_A;
motordrv_t motor_B;

void motordrv_init( void ){
    /*
     * TIM3 PWM channel 1: GPIOA 6 - PWM A
     * TIM3 PWM channel 2: GPIOA 7 - PWM B
     * GPIOB 3 - AIN1
     * GPIOB 4 - AIN2
     * GPIOB 5 - BIN1
     * GPIOB 6 - BIN2
     * GPIOB 7 - STBY
     */
    GPIO_InitTypeDef gpio_config;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
    
    HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOB, GPIO_PIN_7, GPIO_PIN_SET );
    
    gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Speed = GPIO_SPEED_HIGH;
    
    //gpio_config.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    //HAL_GPIO_Init( GPIOA, &gpio_config );
    
    gpio_config.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init( GPIOB, &gpio_config );
    
    /* 启动timer */
    HAL_TIM_Base_Start( &htim3 );
    HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_1 );
    HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_2 );
}

/* @brief 控制电机 */
void motordrv_control( motordrv_t *drv, int power ){
    assert_param( drv==&motor_A || drv==&motor_B );
    
    if( power < -1000 ) power = -1000;
    if( power > 1000  ) power = 1000;
    
    if( power < 0 ){
        if( drv == &motor_A ){
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_SET );
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_4, GPIO_PIN_RESET );
        }
        else{
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_5, GPIO_PIN_SET );
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET );
        }
    }
    else if( power > 0 ){
        if( drv == &motor_A ){
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_4, GPIO_PIN_SET );
        }
        else{
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_5, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_SET );
        }       
    }
    else{
        if( drv == &motor_A ){
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_4, GPIO_PIN_RESET );
        }
        else{
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_5, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET );
        }  
    }
    if( power < 0 ) power = -power;
    if( drv == &motor_A ){
        __HAL_TIM_SET_COMPARE( &htim3, TIM_CHANNEL_1, power );
    }
    else{
        __HAL_TIM_SET_COMPARE( &htim3, TIM_CHANNEL_2, power );
    }
}
