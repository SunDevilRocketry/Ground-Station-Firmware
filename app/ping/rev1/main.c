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
uint8_t    rx_byte;            /* Byte recieved from Wireless module  */
#ifdef USE_RS485
	RS485_STATUS rs485_status; /* RS485 return codes                  */
#else
	RF_STATUS    rf_status;    /* Status of wireless module           */
#endif
USB_STATUS usb_status;         /* Status of USB module                */
uint32_t   time;               /* Time for initial ping timeout       */
uint32_t   start_time;         /* Starting time for timeout counter   */
uint32_t   timeout_cnt;        /* Number of times ping has timeed out */


/*------------------------------------------------------------------------------
 Initializations                                                             
------------------------------------------------------------------------------*/
rx_byte     = 0;
timeout_cnt = 0;


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
start_time = HAL_GetTick();
time       = HAL_GetTick() - start_time;

/* Successfuly initialization LED indication */
led_set_color( LED_GREEN );


/*------------------------------------------------------------------------------
 Event Loop                                                                  
------------------------------------------------------------------------------*/
#ifdef USE_RS485
	while (1)
		{
		/* Update timeout counter */
		time = HAL_GetTick() - start_time; 

		/* Poll the wireless module for incoming data */
		rs485_status = rs485_receive_byte( &rx_byte ); 

		/* Received byte */
		if ( rs485_status != RS485_TIMEOUT )
			{
			led_set_color( LED_GREEN );
			/* Correct byte? */
			if ( rx_byte != PING_CODE )
				{
				Error_Handler( ERROR_RF_UNRECOGNIZED_PING );
				}

			/* Send a ping to the other wireless module */
			rs485_transmit_byte( PING_CODE );

			/* Tell PC a ping has been received */
			usb_status = usb_transmit( &rx_byte         , 
									sizeof( uint8_t ),
									2 );
			if ( usb_status != USB_OK )
				{
				Error_Handler( ERROR_USB_UART_ERROR );
				}
			} /* if ( rs485_status != RS485_TIMEOUT ) */
		else
			{
			led_set_color( LED_YELLOW );
			}

		/* Send out a ping if a ping hasn't been receieved recently */
		if ( time >= PING_TIMEOUT )
			{
			rs485_status = rs485_transmit_byte( PING_CODE );
			if ( rs485_status != RS485_OK )
				{
				Error_Handler( ERROR_RF_ERROR );
				}
			start_time = HAL_GetTick();
			timeout_cnt++;
			}

		/* Error out if timeout count is too large */
		if ( timeout_cnt > 10 )
			{
			while ( 1 )
				{
				led_set_color( LED_BLUE );
				}
			}

		}
#else
	while (1)
		{
		/* Update timeout counter */
		time = HAL_GetTick() - start_time; 

		/* Poll the wireless module for incoming data */
		rf_status = rf_xbee_receive_byte( &rx_byte ); 

		/* Received byte */
		if ( rf_status != RF_TIMEOUT )
			{
			led_set_color( LED_GREEN );
			/* Correct byte? */
			/*
			if ( rx_byte != PING_CODE )
				{
				Error_Handler( ERROR_RF_UNRECOGNIZED_PING );
				}
			*/

			/* Send a ping to the other wireless module */
			rf_xbee_transmit_byte( PING_CODE );

			/* Tell PC a ping has been received */
			/*
			usb_status = usb_transmit( &rx_byte         , 
									sizeof( uint8_t ),
									2 );
			if ( usb_status != USB_OK )
				{
				Error_Handler( ERROR_USB_UART_ERROR );
				}
			*/
			} /* if ( rf_status != RF_TIMEOUT ) */
		else
			{
			led_set_color( LED_YELLOW );
			}

		/* Send out a ping if a ping hasn't been receieved recently */
		if ( time >= PING_TIMEOUT )
			{
			rf_status = rf_xbee_transmit_byte( PING_CODE );
			if ( rf_status != RF_OK )
				{
				Error_Handler( ERROR_RF_ERROR );
				}
			start_time = HAL_GetTick();
			timeout_cnt++;
			}

		/* Error out if timeout count is too large */
		/*
		if ( timeout_cnt > 10 )
			{
			while ( 1 )
				{
				led_set_color( LED_BLUE );
				}
			}
		*/

		}
#endif
} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/