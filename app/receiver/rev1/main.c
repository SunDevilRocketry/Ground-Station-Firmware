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
#include "commands.h"
#include "LoRa.h"

/*------------------------------------------------------------------------------
Global Variables                                                                  
------------------------------------------------------------------------------*/

/* MCU Peripheral handles */
UART_HandleTypeDef huart4; /* Xbee UART */
UART_HandleTypeDef huart1; /* USB UART  */
SPI_HandleTypeDef  hspi2;  /* LoRa SPI */

/* Wireless Module Settings */
WIRELESS_MOD_CODES wireless_mod = LORA;


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
uint8_t    firmware_code;                   /* Firmware identifying code   	*/
uint8_t    rx_byte;     /* Byte recieved from Wireless module */
USB_STATUS usb_status;  /* Status of USB module               */
RF_STATUS  rf_status;   /* Status of wireless module          */


/*------------------------------------------------------------------------------
 Initializations
------------------------------------------------------------------------------*/
/* USB */
uint8_t	   rx_usb_data; 					/* USB incoming data buffer 	*/
usb_status = USB_OK;

/* RF */
rf_status  = RF_TIMEOUT;
rx_byte    = 0;
rf_status  = RF_OK;
uint8_t packet_buffer[32];


/*------------------------------------------------------------------------------
 MCU Initialization                                                                  
------------------------------------------------------------------------------*/
HAL_Init          ();   /* CMSIS HAL */
SystemClock_Config();   /* SysClock  */
GPIO_Init         ();   /* GPIO Pins */
XBee_UART_Init    ();   /* XBee      */
USB_UART_Init     ();   /* USB       */

/*------------------------------------------------------------------------------
 External Hardware Initializations 
------------------------------------------------------------------------------*/
firmware_code = FIRMWARE_GD_RECEIVER;

/* Init LoRa Object */
lora_sx1276 lora;
uint8_t res = lora_init(&lora, &hspi2, LORA_SS_GPIO_PORT, LORA_SS_PIN, LORA_BASE_FREQUENCY_US);
while (res != LORA_OK) {
	// Initialization failed
	led_set_color( LED_PURPLE );
}

/* Indicate Successful Initialization */
led_set_color( LED_GREEN );


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
							// Put LoRa modem into continuous receive mode
							lora_mode_receive_continuous(&lora);
							
							/* Begin Listening */
							while (1)
							{
								/* LoRa Receive packet using blocking mode */
								uint8_t len = lora_receive_packet_blocking(&lora, packet_buffer, sizeof(packet_buffer), 10000, &res);

								/* Transmit byte to PC over USB */
								if ( res == LORA_OK )
									{
									usb_status = usb_transmit( &packet_buffer         , 
															sizeof( packet_buffer ),
															HAL_DEFAULT_TIMEOUT  );
									if ( usb_status != USB_OK )
										{
										Error_Handler( ERROR_USB_UART_ERROR );
										}
									}
							}

							break;	
							}
						default:
							{
							/* Unrecognized module: invoke error handler */
							Error_Handler( ERROR_UNSUPPORTED_OP_ERROR );
							break;
							}
						}

					// /* Transmit byte to PC over USB */
					// if ( rf_status != RF_TIMEOUT )
					// 	{
					// 	usb_status = usb_transmit( &rx_byte         , 
					// 							sizeof( uint8_t ),
					// 							HAL_DEFAULT_TIMEOUT  );
					// 	if ( usb_status != USB_OK )
					// 		{
					// 		Error_Handler( ERROR_USB_UART_ERROR );
					// 		}
					// 	}
				} /* while (1) */
			}
		}
	} /* main */
}
/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/