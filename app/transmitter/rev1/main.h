/*******************************************************************************
*
* FILE: 
*       main.h
*
* DESCRIPTION: 
*		Transmits data recieved over UART/USB in order to test the wireless 
*       transcievers
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


/*------------------------------------------------------------------------------
 Macros
------------------------------------------------------------------------------*/

/* Generic HAL related macros */
#define HAL_DEFAULT_TIMEOUT		( 1 )


/*------------------------------------------------------------------------------
 Project Includes                                                                     
------------------------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
