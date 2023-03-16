/*******************************************************************************
*                                                                              *
* FILE:                                                                        *
* 		init.h                                                                 *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Contains initialization routines for MCU core and peripherals          *
*                                                                              *
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INIT_H 
#define INIT_H

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                               
------------------------------------------------------------------------------*/
#include "main.h"


/*------------------------------------------------------------------------------
 Function prototypes                                                          
------------------------------------------------------------------------------*/
void SystemClock_Config      ( void );      /* clock configuration            */
void GPIO_Init               ( void );      /* GPIO configurations            */
void USB_UART_Init           ( void );      /* USB UART configuration         */
void XBee_UART_Init          ( void );      /* XBee UART configuration        */


#ifdef __cplusplus
}
#endif
#endif /* INIT_H */

/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/