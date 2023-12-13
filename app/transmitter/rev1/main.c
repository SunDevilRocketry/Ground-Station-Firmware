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
#include "commands.h"


/*------------------------------------------------------------------------------
 Global Variables                                                                  
------------------------------------------------------------------------------*/

/* MCU Peripheral handles */
UART_HandleTypeDef huart4; /* Xbee UART */
UART_HandleTypeDef huart1; /* USB UART  */
SPI_HandleTypeDef  hspi2;

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
/* USB */
uint8_t    firmware_code;                   /* Firmware identifying code   	*/
uint8_t	   rx_usb_data; 					/* USB incoming data buffer 	*/
USB_STATUS usb_status;  					/* Status of USB module     	*/

/* RF */
uint8_t    tx_byte;     /* Byte to transmit with wireless module */
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
 External Hardware Initializations 
------------------------------------------------------------------------------*/
firmware_code = FIRMWARE_GD_TRANSMITTER;

/*------------------------------------------------------------------------------
Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
	/* Receive byte from USB port */
	usb_status = usb_receive( &rx_usb_data     , 
                              sizeof( uint8_t ), 
                              HAL_DEFAULT_TIMEOUT );

	if (usb_status == USB_FAIL)
	{
		led_set_color(LED_RED);
	}

	/* Transmit byte with wireless module */
	if ( usb_status != USB_TIMEOUT )
	{
		led_set_color(LED_WHITE);
		switch (rx_usb_data)
		{
		case PING_OP:
		{
			ping();
			break;
		}
		case CONNECT_OP:
		{
			ping();

			/* TODO: Add firmware version code */
			usb_transmit( &firmware_code	,
						  sizeof( uint8_t ),
						  HAL_DEFAULT_TIMEOUT	
						 );
			break;
		}
		case SEND_OP:
		{
			switch ( wireless_mod )
			{
			case XBEE:
				{
				uint8_t sample_code = 0x69;
				rf_status = rf_xbee_transmit_byte( sample_code );
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
			break;
		}
		default:
			break;
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