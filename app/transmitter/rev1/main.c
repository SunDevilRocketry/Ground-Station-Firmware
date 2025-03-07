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
#include "commands.h"

/*------------------------------------------------------------------------------
 Global Variables                                                                  
------------------------------------------------------------------------------*/

/* MCU Peripheral handles */
UART_HandleTypeDef huart1; /* USB UART  */
SPI_HandleTypeDef  hspi2;  /* LORA SPI */

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
uint8_t     subcommand_code;                   /* Subcommand opcode           */
uint8_t    	usb_rx_byte;     				   /* Byte to transmit with wireless module */
USB_STATUS 	usb_status;  					   /* Status of USB module               */
uint8_t		firmware_code;					   /* Board configuration */

/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
usb_rx_byte    = 0;
usb_status = USB_OK;


/* General Board configuration */
firmware_code                 = FIRMWARE_FGD_TRANSMITTER;                   

/*------------------------------------------------------------------------------
 MCU Initialization                                                                  
------------------------------------------------------------------------------*/
HAL_Init          ();   /* CMSIS HAL */
SystemClock_Config();   /* SysClock  */
GPIO_Init         ();   /* GPIO Pins */
USB_UART_Init     ();   /* USB       */
LORA_SPI_Init	  ();	/* LORA		 */

/* Indicate Successful Initialization */
led_set_color( LED_GREEN );


/*------------------------------------------------------------------------------
Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
	/* Receive byte from USB port */
	usb_status = usb_receive( &usb_rx_byte         , 
                              sizeof( uint8_t ), 
                              200 );
	}

	if ( usb_status == USB_OK )
	{
	switch ( usb_rx_byte )
		{
		/*-------------------------------------------------------------
			CONNECT_OP	
		-------------------------------------------------------------*/
		case CONNECT_OP:
			{
			/* Send board identifying code    */
			ping();

			/* Send firmware identifying code */
			usb_transmit( &firmware_code   , 
						sizeof( uint8_t ), 
						HAL_DEFAULT_TIMEOUT );
			break;
			} /* CONNECT_OP */
		/*-------------------------------------------------------------
			Unrecognized command code  
		-------------------------------------------------------------*/
		default:
			{
			//Error_Handler();
			break;
			}

		} /* switch( usb_rx_data ) */
	} /* if ( usb_status != USB_OK ) */

} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/