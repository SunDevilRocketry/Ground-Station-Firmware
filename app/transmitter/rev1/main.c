/*******************************************************************************
*
* FILE: 
*       main.c
*
* DESCRIPTION: 
*		Transmits data recieved over UART/USB in order to test the wireless 
*       transcievers
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
#include "sdr_error.h"
#include "sdr_pin_defines_A0005.h"

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
uint8_t    tx_byte;     /* Byte to transmit with wireless module */
USB_STATUS usb_status;  /* Status of USB module               */
RF_STATUS  rf_status;   /* Status of wireless module          */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
tx_byte    = 0;
usb_status = USB_OK;
rf_status  = RF_OK;


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
	/* Receive byte from USB port */
	usb_status = usb_receive( &tx_byte         , 
                              sizeof( uint8_t ), 
                              HAL_DEFAULT_TIMEOUT );

	/* Transmit byte with wireless module */
	if ( usb_status != USB_TIMEOUT )
		{
		switch ( wireless_mod )
			{
			case XBEE:
				{
				rf_status = rf_xbee_transmit_byte( tx_byte );
				if ( rf_status != RF_OK )
					{
					Error_Handler( ERROR_RF_ERROR );
					}
				break;
				}
			case LORA:
				{
				// TODO: Implement lora transmit byte function
				// TODO: Get rid of Error_Handler()
				Error_Handler( ERROR_UNSUPPORTED_OP_ERROR );
				break;
				}
			default:
				{
				/* Unrecognized wireless module, invoke error handler */
				Error_Handler( ERROR_UNSUPPORTED_OP_ERROR );
				break;
				}
			}
		}
	else
		{
		/* USB timeout, do noting until next byte arrives */
		}
	}

} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/