/*******************************************************************************
*
* FILE: 
*       main.c
*
* DESCRIPTION: 
*		Recieves data over LoRa and relays it to SDEC.
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
#include "common.h"
#include "lora.h"

/*------------------------------------------------------------------------------
 Global Variables                                                                  
------------------------------------------------------------------------------*/

/* LoRa config settings */
LORA_PRESET lora_preset;

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
USB_STATUS 	usb_status;  					   /* Status of USB module               */
uint8_t		firmware_code;					   /* Board configuration */

/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/

/* General Board configuration */
firmware_code                 = FIRMWARE_RECEIVER;

/* LORA configs */
memset( &lora_preset, 0, sizeof(lora_preset) );
lora_preset.lora_bandwidth = 0x07;
lora_preset.lora_spreading_factor = 0x12;
lora_preset.lora_frequency = 915000;

/*------------------------------------------------------------------------------
 MCU Initialization                                                                  
------------------------------------------------------------------------------*/
HAL_Init          ();   /* CMSIS HAL */
SystemClock_Config();   /* SysClock  */
GPIO_Init         ();   /* GPIO Pins */
USB_UART_Init     ();   /* USB       */

/* Indicate Successful Initialization */
led_set_color( LED_GREEN );


/*------------------------------------------------------------------------------
Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
	usb_status = terminal_loop( firmware_code );
	assert_fail_fast( usb_status != USB_FAIL, ERROR_USB_UART_ERROR );

	} /* main */
}

/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/