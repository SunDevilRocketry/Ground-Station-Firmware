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
#include "rs485.h"
#include "usb.h"
#include "wireless.h"


/*------------------------------------------------------------------------------
Global Variables                                                                  
------------------------------------------------------------------------------*/

/* MCU Peripheral handles */
UART_HandleTypeDef huart1; /* USB UART    */
UART_HandleTypeDef huart4; /* Xbee UART   */
UART_HandleTypeDef huart5; /* RS-485 UART */


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
uint8_t       command;          /* SDEC command                               */
uint8_t       board_code;       /* Ping code of connected board               */
#ifdef USE_RS485
	RS485_STATUS rs485_status; /* RS485 return codes                  */
#else
	RF_STATUS    rf_status;    /* Status of wireless module           */
#endif
USB_STATUS usb_status;         /* Status of USB module                */


/*------------------------------------------------------------------------------
 Initializations                                                             
------------------------------------------------------------------------------*/
#ifdef USE_RS485
	rs485_status = RS485_OK;
#else
	rf_status    = RF_OK;
#endif
usb_status = USB_OK;


/*------------------------------------------------------------------------------
 MCU Initialization                                                             
------------------------------------------------------------------------------*/
HAL_Init          ();   /* CMSIS HAL */
SystemClock_Config();   /* SysClock  */
GPIO_Init         ();   /* GPIO Pins */
XBee_UART_Init    ();   /* XBee      */
USB_UART_Init     ();   /* USB       */
RS485_UART_Init   ();   /* RS485     */


/*------------------------------------------------------------------------------
 Initial Setup 
------------------------------------------------------------------------------*/

/* Successfuly initialization LED indication */
led_set_color( LED_GREEN );


/*------------------------------------------------------------------------------
 Event Loop                                                                  
------------------------------------------------------------------------------*/
while ( 1 )
	{
	/* Read data from UART reciever */
    usb_status = usb_receive( &command, sizeof( command ), HAL_DEFAULT_TIMEOUT );

		/* Parse command input if HAL_UART_Receive doesn't timeout */
	if ( usb_status != USB_TIMEOUT )
		{
		switch( command )
			{
			/*-----------------------------------------------------------------
			 PING Command	
			------------------------------------------------------------------*/
			case PING_OP:
				{
				/* Send Command to engine controller */
				rs485_status = rs485_transmit( &command         , 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}

				/* Receive Engine controller ping response */
				rs485_status = rs485_receive( &board_code         , 
				                              sizeof( board_code ), 
											  RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}
				
				/* Transmit Ping response back to PC */
				usb_status = usb_transmit( &board_code, 
				                           sizeof( board_code ), 
										   RS485_DEFAULT_TIMEOUT );
				if ( usb_status != USB_OK ) 
					{
					Error_Handler( ERROR_USB_UART_ERROR );
					}
				
				break;
				} /* PING_OP */

			/*-----------------------------------------------------------------
			 CONNECT Command	
			------------------------------------------------------------------*/
			case CONNECT_OP:
				{
				/* Send Command to engine controller */
				rs485_status = rs485_transmit( &command         , 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}

				/* Receive Engine controller ping response */
				rs485_status = rs485_receive( &board_code         , 
				                              sizeof( board_code ), 
											  RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}
				
				/* Transmit Ping response back to PC */
				usb_status = usb_transmit( &board_code, 
				                           sizeof( board_code ), 
										   RS485_DEFAULT_TIMEOUT );
				if ( usb_status != USB_OK ) 
					{
					Error_Handler( ERROR_USB_UART_ERROR );
					}
				break;
				} /* CONNECT_OP */

			/*-----------------------------------------------------------------
			 Unrecognized Command 
			------------------------------------------------------------------*/
			default:
				{
				/* Unsupported command code flash the red LED */
				//Error_Handler();
				} /* default */
			} 
		} 
	
	}
} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/