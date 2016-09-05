/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "led.h"
#include "usart.h"

#define UART_BAUD_RATE          115200
static uint8_t recv_buf[1];

#define PDEBUG(fmt,args...)

void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    PDEBUG( "portserial enable %d,%d\n", xRxEnable, xTxEnable );
    (void)xRxEnable;
    (void)xTxEnable;
    
    if( xRxEnable && !xTxEnable ){
        HAL_HalfDuplex_EnableReceiver( &huart1 );
        HAL_UART_Receive_IT( &huart1, recv_buf, 1 );
    }
    else if( !xRxEnable && xTxEnable ){
        HAL_HalfDuplex_EnableTransmitter( &huart1 );
        pxMBFrameCBTransmitterEmpty();
        
    }
    else{
        assert_param( 0 );
    }
}

BOOL xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    PDEBUG( "port serial init\n" );
    return TRUE;
}

BOOL xMBPortSerialPutByte( CHAR ucByte )
{
    HAL_UART_Transmit_IT( &huart1, (uint8_t*)&ucByte, 1 );
    return TRUE;
}

BOOL xMBPortSerialGetByte( CHAR * pucByte )
{
    HAL_StatusTypeDef ret;
    *pucByte = recv_buf[0];
    /* 接受下一个数据 */
    ret = HAL_UART_Receive_IT( &huart1, recv_buf, 1 );
    return (ret==HAL_OK);
}

void portserial_rxISR( void ){
    led_control( LED3, LED_TURN );
    pxMBFrameCBByteReceived();
}
void portserial_txISR( void ){
    pxMBFrameCBTransmitterEmpty();
}

