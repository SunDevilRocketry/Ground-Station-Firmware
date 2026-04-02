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
#include "error_sdr.h"
#include "sdr_pin_defines_A0005.h"

/* SDR Modules */
#include "led.h"
#include "usb.h"
#include "commands.h"
#include "math_sdr.h"
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
USB_STATUS 	usb_status;  					   /* Status of USB module */
uint8_t		firmware_code;					   /* Board configuration */

/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/

/* General Board configuration */
firmware_code                 = FIRMWARE_RECEIVER;

/* LORA configs */
// memset( &lora_preset, 0, sizeof(lora_preset) );
// lora_preset.lora_bandwidth = 0x07;
// lora_preset.lora_spreading_factor = 0x12;
// lora_preset.lora_frequency = 915000;

/*------------------------------------------------------------------------------
 MCU Initialization                                                                  
------------------------------------------------------------------------------*/
HAL_Init          ();   /* CMSIS HAL */
SystemClock_Config();   /* SysClock  */
GPIO_Init         ();   /* GPIO Pins */
USB_UART_Init     ();   /* USB       */
LORA_SPI_Init     ();   /* LoRa      */

/* Get LoRa configs from nonvol. This is technically memory unsafe, but we know
   from the linker script that we have 256 bytes in this region, so its fine. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overread"
memcpy(&lora_preset, USER_CONFIG_ADDR, sizeof(LORA_PRESET));
LORA_STATUS lora_init_status = lora_configure(&lora_preset);
#pragma GCC diagnostic pop

if( lora_init_status == LORA_USING_DEFAULTS )
    {
    /* give an indicator of default configs*/
    for( int i = 0; i < 4; i++ )
        {
        led_set_color( LED_CYAN );
        HAL_Delay(200);
        led_set_color( LED_YELLOW );
        HAL_Delay(200);
        }
    }
else if( lora_init_status != LORA_OK )
    {
    error_fail_fast( ERROR_LORA_INIT_ERROR );
    }

/* Indicate Successful Initialization */
led_set_color( LED_GREEN );


/*------------------------------------------------------------------------------
Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
	usb_status = terminal_loop( firmware_code );
	assert_fail_fast( (usb_status != USB_FAIL), ERROR_USB_UART_ERROR );

	}
} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/