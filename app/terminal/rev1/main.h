/*******************************************************************************
*
* FILE: 
*       main.h
*
* DESCRIPTION: 
*		Pings another XBee module and indicates to PC when a ping code has 
*       received. Used to perform range testing and bidirectional communcation
*       testing.	
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