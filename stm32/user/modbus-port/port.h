#ifndef _PORT_H
#define _PORT_H

#include <stdio.h>

//#include <assert.h>
#define assert( exp ) \
    do{ \
        if( !(exp) ){ \
            printf( "ASSERT: %s %d\n", __FILE__, __LINE__ ); \
            while( 1 ){} \
        } \
    } while( 0 ) \

#define INLINE
#define PR_BEGIN_EXTERN_C           extern "C" {
#define PR_END_EXTERN_C             }

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
/* ----------------------- Defines ------------------------------------------*/
#define ENTER_CRITICAL_SECTION( ) //vMBPortEnterCritical()
#define EXIT_CRITICAL_SECTION( ) //vMBPortExitCritical()
#define MB_PORT_HAS_CLOSE   0

#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif
/* ----------------------- Type definitions ---------------------------------*/
    typedef enum
{
    MB_LOG_ERROR = 0,
    MB_LOG_WARN = 1,
    MB_LOG_INFO = 2,
    MB_LOG_DEBUG = 3
} eMBPortLogLevel;

typedef char    BOOL;
typedef unsigned char UCHAR;
typedef char    CHAR;
typedef unsigned short USHORT;
typedef short   SHORT;

typedef unsigned long ULONG;
typedef long    LONG;

/* ----------------------- Function prototypes ------------------------------*/

void            vMBPortEnterCritical( void );
void            vMBPortExitCritical( void );
void            vMBPortLog( eMBPortLogLevel eLevel, const CHAR * szModule,
                            const CHAR * szFmt, ... );
void            vMBPortTimerPoll( void );
BOOL            xMBPortSerialPoll( void );
BOOL            xMBPortSerialSetTimeout( ULONG dwTimeoutMs );

void porttimer_timeout( void );
void portserial_rxISR( void );
void portserial_txISR( void );

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
