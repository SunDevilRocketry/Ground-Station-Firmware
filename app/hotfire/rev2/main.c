/*******************************************************************************
*
* FILE: 
*       main.c
*
* DESCRIPTION: 
*		Interfaces between PC commands and the engine controller remotely 
*       during a static hotfire test	
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                                     
------------------------------------------------------------------------------*/
#include <string.h>
#include <stdbool.h>


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
#include "pressure.h"
#include "loadcell.h"
#include "rs485.h"
#include "sensor.h"
#include "temp.h"
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
 Function Prototypes 
------------------------------------------------------------------------------*/

/* Sends a signal to the PC that engine controller is responsive */
static void send_ack
	(
	void
	);

/* Sends a signal to the PC that engine controller is unresponsive */
static void send_no_ack
	(
	void
	);


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
uint8_t command;                              /* SDEC command                 */
uint8_t subcommand;                           /* SDEC subcommand              */
uint8_t board_code;                           /* Ping code of connected board */
uint8_t response;                             /* Response from engine         */
uint8_t sensor_data[ sizeof( SENSOR_DATA ) ]; /* Data from engine sensors     */
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
memset( &sensor_data[0], 0, sizeof( sensor_data ) );


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

				/* Receive Engine controller ping response */
				rs485_status = rs485_receive( &board_code         , 
				                              sizeof( board_code ), 
											  RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					response = NO_ACK_OP;
					}
				else
					{
					response = board_code;
					}
				
				/* Transmit Ping response back to PC */
				usb_status = usb_transmit( &response         , 
				                           sizeof( response ), 
										   RS485_DEFAULT_TIMEOUT );
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

				/* Receive Engine controller ping response */
				rs485_status = rs485_receive( &board_code         , 
				                              sizeof( board_code ), 
											  RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					response = NO_ACK_OP;
					}
				else
					{
					response = board_code;
					}
				
				/* Transmit Ping response back to PC */
				usb_status = usb_transmit( &response         , 
				                           sizeof( response ), 
										   RS485_DEFAULT_TIMEOUT );
				break;
				} /* CONNECT_OP */
			
			/*-----------------------------------------------------------------
			 SOL Command	
			------------------------------------------------------------------*/
			case SOL_OP:
				{
				/* Get subcommand */
				usb_status = usb_receive( &subcommand, 
				                          sizeof( subcommand ), 
										  HAL_DEFAULT_TIMEOUT );
				if ( usb_status != USB_OK )
					{
					break;
					}
				
				/* Pass on command and subcommand to engine controller */
				rs485_status = rs485_transmit( &command         , 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					send_no_ack();
					break;
					}
				rs485_status = rs485_transmit( &subcommand         , 
				                               sizeof( subcommand ), 
											   RS485_DEFAULT_TIMEOUT );				
				if ( rs485_status != RS485_OK )
					{
					send_no_ack();
					break;
					}
	
				/* Wait for acknowledge signal/response */
				rs485_status = rs485_receive( &response         , 
				                              sizeof( response ), 
											  RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					send_no_ack();
					}
				else
					{
					usb_transmit( &response, 
					              sizeof( response ), 
								  HAL_DEFAULT_TIMEOUT );
					}
				break;
				} /* SOL_OP */

			/*-----------------------------------------------------------------
			 VALVE Command	
			------------------------------------------------------------------*/
			case VALVE_OP:
				{
				break;
				/* Get subcommand */
				usb_status = usb_receive( &subcommand, 
				                          sizeof( subcommand ), 
										  HAL_DEFAULT_TIMEOUT );
				if ( usb_status != USB_OK )
					{
					break;
					}
				
				/* Pass on command and subcommand to engine controller */
				rs485_status = rs485_transmit( &command         , 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					send_no_ack();
					break;
					}
				rs485_status = rs485_transmit( &subcommand         , 
				                               sizeof( subcommand ), 
											   RS485_DEFAULT_TIMEOUT );				
				if ( rs485_status != RS485_OK )
					{
					send_no_ack();
					break;
					}
	
				/* Wait for acknowledge signal */
				rs485_status = rs485_receive( &response         , 
				                              sizeof( response ), 
											  RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					send_no_ack();
					}
				else
					{
					usb_transmit( &response, 
					              sizeof( response ), 
								  HAL_DEFAULT_TIMEOUT );
					}
				} /* VALVE_OP */

			/*-----------------------------------------------------------------
			 ABORT Command	
			------------------------------------------------------------------*/
			case ABORT_OP:
				{
				/* Send ABORT Command */
				command = ABORT_OP;
				rs485_status = rs485_transmit( &command, 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}

				/* Wait for ACK Command */
				rs485_status = rs485_receive( &response, 
				                              sizeof( response ),
											  RS485_DEFAULT_TIMEOUT );
				if ( ( rs485_status != RS485_OK ) || ( response != ACK_OP ) )
					{
					response = NO_ACK_OP;
					}
				usb_transmit( &response, sizeof( response ), HAL_DEFAULT_TIMEOUT );
				break;
				} /* ABORT_OP */

			/*-----------------------------------------------------------------
			 TELREQ Command	
			------------------------------------------------------------------*/
			case TELREQ_OP:
				{
				/* Send the TELREQ Command to the engine controller */
				rs485_transmit( &command, sizeof( command ), RS485_DEFAULT_TIMEOUT );

				/* Receive the sensor data */
				rs485_status = rs485_receive( &sensor_data[0]      , 
				                              sizeof( sensor_data ),
											  sizeof( sensor_data )*RS485_DEFAULT_TIMEOUT );

				/* Pass on the sensor data back to the PC */
				if ( rs485_status != RS485_OK )
					{
					send_no_ack();
					}
				else
					{
					send_ack();
					usb_transmit( &sensor_data[0], 
					              sizeof( sensor_data ), 
								  sizeof( sensor_data )*HAL_DEFAULT_TIMEOUT );	
					}
				
				break;
				} /* TELREQ_OP */

			/*-----------------------------------------------------------------
			 PFPURGE Command	
			------------------------------------------------------------------*/
			case PFPURGE_OP:
				{
				/* Send ABORT Command */
				command = PFPURGE_OP;
				rs485_status = rs485_transmit( &command, 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}

				/* Wait for ACK Command */
				rs485_status = rs485_receive( &response, 
				                              sizeof( response ),
											  RS485_DEFAULT_TIMEOUT );
				if ( ( rs485_status != RS485_OK ) || ( response != ACK_OP ) )
					{
					response = NO_ACK_OP;
					}
				usb_transmit( &response, sizeof( response ), HAL_DEFAULT_TIMEOUT );
				break;
				} /* PFPURGE_OP */
			
			/*-----------------------------------------------------------------
			 CHILL Command	
			------------------------------------------------------------------*/
			case CHILL_OP:
				{
				/* Send ABORT Command */
				command = CHILL_OP;
				rs485_status = rs485_transmit( &command, 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}

				/* Wait for ACK Command */
				rs485_status = rs485_receive( &response, 
				                              sizeof( response ),
											  RS485_DEFAULT_TIMEOUT );
				if ( ( rs485_status != RS485_OK ) || ( response != ACK_OP ) )
					{
					response = NO_ACK_OP;
					}
				usb_transmit( &response, sizeof( response ), HAL_DEFAULT_TIMEOUT );
				break;
				} /* CHILL_OP */

			/*-----------------------------------------------------------------
			 STANDBY Command	
			------------------------------------------------------------------*/
			case STANDBY_OP:
				{
				/* Send ABORT Command */
				command = STANDBY_OP;
				rs485_status = rs485_transmit( &command, 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}

				/* Wait for ACK Command */
				rs485_status = rs485_receive( &response, 
				                              sizeof( response ),
											  RS485_DEFAULT_TIMEOUT );
				if ( ( rs485_status != RS485_OK ) || ( response != ACK_OP ) )
					{
					response = NO_ACK_OP;
					}
				usb_transmit( &response, sizeof( response ), HAL_DEFAULT_TIMEOUT );
				break;
				} /* STANDBY_OP */

			/*-----------------------------------------------------------------
			 FIRE Command 
			------------------------------------------------------------------*/
			case FIRE_OP:
				{
				/* Send ABORT Command */
				command = FIRE_OP;
				rs485_status = rs485_transmit( &command, 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}

				/* Wait for ACK Command */
				rs485_status = rs485_receive( &response, 
				                              sizeof( response ),
											  RS485_DEFAULT_TIMEOUT );
				if ( ( rs485_status != RS485_OK ) || ( response != ACK_OP ) )
					{
					response = NO_ACK_OP;
					}
				usb_transmit( &response, sizeof( response ), HAL_DEFAULT_TIMEOUT );
				break;
				} /* FIRE_OP */

			/*-----------------------------------------------------------------
			 STOPPURGE Command 
			------------------------------------------------------------------*/
			case STOPPURGE_OP:
				{
				/* Send ABORT Command */
				command = STOPPURGE_OP;
				rs485_status = rs485_transmit( &command, 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					Error_Handler( ERROR_RS485_UART_ERROR );
					}

				/* Wait for ACK Command */
				rs485_status = rs485_receive( &response, 
				                              sizeof( response ),
											  RS485_DEFAULT_TIMEOUT );
				if ( ( rs485_status != RS485_OK ) || ( response != ACK_OP ) )
					{
					response = NO_ACK_OP;
					}
				usb_transmit( &response, sizeof( response ), HAL_DEFAULT_TIMEOUT );
				break;
				} /* STOPPURGE_OP */

			/*-----------------------------------------------------------------
			 GETSTATE Command 
			------------------------------------------------------------------*/
			case HOTFIRE_GETSTATE_OP:
				{
				/* Send getstate command */
				command = HOTFIRE_GETSTATE_OP;
				rs485_status = rs485_transmit( &command         , 
				                               sizeof( command ), 
											   RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					led_set_color( LED_YELLOW );
					break;
					}
				
				/* Get state from engine controller */
				rs485_status = rs485_receive( &response         , 
				                              sizeof( response ), 
											  RS485_DEFAULT_TIMEOUT );
				if ( rs485_status != RS485_OK )
					{
					response = NO_ACK_OP;
					}
				
				/* Send back to PC */
				usb_transmit( &response, sizeof( response ), HAL_DEFAULT_TIMEOUT );
				break;
				}

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
*                                                                              *
* PROCEDURE:                                                                   *
*       send_ack                                                               *
*                                                                              *
* DESCRIPTION:                                                                 *
*		Sends a signal to the PC that engine controller is responsive          *
*                                                                              *
*******************************************************************************/
static void send_ack
	(
	void
	)

{ 
uint8_t response = ACK_OP; 
usb_transmit( &response, sizeof( response ), HAL_DEFAULT_TIMEOUT );
} /* send_no_ack */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
*       send_no_ack                                                            *
*                                                                              *
* DESCRIPTION:                                                                 *
*		Sends a signal to the PC that engine controller is unresponsive        *
*                                                                              *
*******************************************************************************/
static void send_no_ack
	(
	void
	)

{ 
uint8_t response = NO_ACK_OP; 
usb_transmit( &response, sizeof( response ), HAL_DEFAULT_TIMEOUT );
} /* send_no_ack */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/