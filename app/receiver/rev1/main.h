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
 Includes                                                                     
------------------------------------------------------------------------------*/

/* HAL includes */
#include "stm32h7xx_hal.h"

/* project includes */
#include "usb.h"
#include "lora.h"

/*------------------------------------------------------------------------------
 Macros
------------------------------------------------------------------------------*/

/* Generic HAL related macros */
#define HAL_DEFAULT_TIMEOUT		( 10 )
#define RECIEVER_TERMINAL_TIMEOUT    ( 100 )

/*------------------------------------------------------------------------------
 Function Prototypes
------------------------------------------------------------------------------*/

USB_STATUS terminal_loop
	(
	uint8_t	firmware_code  /* Board configuration */
	);

USB_STATUS telem_loop
	(
	void
	);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/