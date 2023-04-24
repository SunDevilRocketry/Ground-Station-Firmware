/*******************************************************************************
*
* FILE: 
*       main.h
*
* DESCRIPTION: 
*		Interfaces between PC commands and the engine controller remotely 
*       during a static hotfire test	
*
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------------
Standard Includes                                                                     
------------------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "sdr_error.h"


/*------------------------------------------------------------------------------
Standard Includes                                                                     
------------------------------------------------------------------------------*/

/* General HAL related macros */
#define HAL_DEFAULT_TIMEOUT		( 1 )

/* Ping timeout */
#define PING_TIMEOUT            ( 5000 ) /* ms */
#define PING_CODE               0x67

/* Protocol Commands */
#define ABORT_OP                ( 0x90 )
#define TELREQ_OP               ( 0x96 )
#define PFPURGE_OP              ( 0x91 )
#define CHILL_OP                ( 0x92 )
#define STANDBY_OP              ( 0x93 )
#define FIRE_OP                 ( 0x94 )
#define STOPPURGE_OP            ( 0x97 )
#define ACK_OP                  ( 0x95 )
#define NO_ACK_OP               ( 0x98 )
#define HOTFIRE_GETSTATE_OP     ( 0x99 )
#define STOP_HOTFIRE_OP         ( 0x9A )


/*------------------------------------------------------------------------------
Project Includes                                                                     
------------------------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/* Handles miscellaneous errors */
void Error_Handler
    ( 
    volatile ERROR_CODE error_code 
    );

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/