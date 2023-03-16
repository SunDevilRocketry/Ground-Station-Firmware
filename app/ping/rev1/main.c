/*******************************************************************************
*
* FILE: 
*       main.c
*
* DESCRIPTION: 
*		Pings another XBee module and indicates to PC when a ping code has 
*       received. Used to perform range testing and bidirectional communcation
*       testing.	
*
*******************************************************************************/


/*------------------------------------------------------------------------------
Standard Includes                                                                     
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
Project Includes                                                                     
------------------------------------------------------------------------------*/

/* Pin definitions, main prototypes, error codes */
#include "main.h"
#include "init.h"
#include "sdr_pin_defines_A0005.h"
#include "sdr_error.h"

/* SDR Modules */
#include "commands.h"
#include "led.h"
#include "usb.h"
#include "wireless.h"


/*------------------------------------------------------------------------------
Global Variables                                                                  
------------------------------------------------------------------------------*/

/* MCU Peripheral handles */
UART_HandleTypeDef huart4; /* Xbee UART */
UART_HandleTypeDef huart1; /* USB UART  */


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
rx_byte = 0;


/*------------------------------------------------------------------------------
 MCU Initialization                                                             
------------------------------------------------------------------------------*/
HAL_Init          ();   /* CMSIS HAL */
SystemClock_Config();   /* SysClock  */
GPIO_Init         ();   /* GPIO Pins */
XBee_UART_Init    ();   /* XBee      */
USB_UART_Init     ();   /* USB       */


/*------------------------------------------------------------------------------
 Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
	/* Poll the wireless module for incoming data */
	rf_status = rf_xbee_receive_byte( &rx_byte ); 

	/* Received byte */
	if ( rf_status != RF_TIMEOUT )
		{
		/* Correct byte? */
		if ( rx_byte != PING_OP )
			{
			Error_Handler( ERROR_RF_UNRECOGNIZED_PING );
			}

		/* Send a ping to the other wireless module */
		rf_xbee_transmit_byte( PING_OP );

		/* Tell PC a ping has been received */
	    usb_status = usb_transmit( &rx_byte         , 
                                   sizeof( uint8_t ),
                                   HAL_DEFAULT_TIMEOUT  );
		if ( usb_status != USB_OK )
			{
			Error_Handler( ERROR_USB_UART_ERROR );
			}
		} /* if ( rf_status != RF_TIMEOUT ) */
	}
} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/