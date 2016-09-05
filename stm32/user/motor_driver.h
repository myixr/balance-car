#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

typedef struct{
    int reversed;
}motordrv_t;

extern motordrv_t motor_A;
extern motordrv_t motor_B;

/* @brief 初始化 */
void motordrv_init( void );


/* @brief 控制电机 
 * @param power [-1000-1000]
 */
void motordrv_control( motordrv_t *drv, int power );


#endif
