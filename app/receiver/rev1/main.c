/*******************************************************************************
*
* FILE: 
*       main.c
*
* DESCRIPTION: 
*		Receives wireless data and relays the data to a PC over UART/USB 
*       in order to test the wireless transcievers
*
*******************************************************************************/


/*------------------------------------------------------------------------------
Standard Includes                                                                     
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
Project Includes                                                                     
------------------------------------------------------------------------------*/

/* Pin definitions and main prototypes */
#include "main.h"
#include "init.h"
#include "sdr_pin_defines_A0005.h"
#include "sdr_error.h"

/* SDR Modules */
#include "led.h"
#include "usb.h"
#include "wireless.h"


/*------------------------------------------------------------------------------
Global Variables                                                                  
------------------------------------------------------------------------------*/

/* MCU Peripheral handles */
UART_HandleTypeDef huart4; /* Xbee UART */
UART_HandleTypeDef huart1; /* USB UART  */

/* Wireless Module Settings */
WIRELESS_MOD_CODES wireless_mod = XBEE;


/*------------------------------------------------------------------------------
 Application entry point                                                      
------------------------------------------------------------------------------*/
int main
	(
	void
	)
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
uint8_t    rx_byte;     /* Byte recieved from Wireless module */
USB_STATUS usb_status;  /* Status of USB module               */
RF_STATUS  rf_status;   /* Status of wireless module          */


/*------------------------------------------------------------------------------
 Initializations
------------------------------------------------------------------------------*/
rf_status  = RF_TIMEOUT;
rx_byte    = 0;
usb_status = USB_OK;


/*------------------------------------------------------------------------------
 MCU Initialization                                                                  
------------------------------------------------------------------------------*/
HAL_Init          ();   /* CMSIS HAL */
SystemClock_Config();   /* SysClock  */
GPIO_Init         ();   /* GPIO Pins */
XBee_UART_Init    ();   /* XBee      */
USB_UART_Init     ();   /* USB       */

/* Indicate Successful Initialization */
led_set_color( LED_GREEN );


/*------------------------------------------------------------------------------
 Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
	/* Recieve Byte from Wireless module */
	switch ( wireless_mod )
		{
		case XBEE:
			{
			rf_status = rf_xbee_receive_byte( &rx_byte );
			break;
			}
		case LORA:
			{
			// TODO: Implement rf_lora_receive_byte function 
			// TODO: Remove Error_Handler()
			Error_Handler( ERROR_UNSUPPORTED_OP_ERROR );
			break;	
			}
		default:
			{
			/* Unrecognized module: invoke error handler */
			Error_Handler( ERROR_UNSUPPORTED_OP_ERROR );
			break;
			}
		}

	/* Transmit byte to PC over USB */
	if ( rf_status != RF_TIMEOUT )
		{
	    usb_status = usb_transmit( &rx_byte         , 
                                   sizeof( uint8_t ),
                                   HAL_DEFAULT_TIMEOUT  );
		if ( usb_status != USB_OK )
			{
			Error_Handler( ERROR_USB_UART_ERROR );
			}
		}
	} /* while (1) */

} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/