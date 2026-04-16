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
#include "telemetry.h"
#include "lora.h"

/*------------------------------------------------------------------------------
 Global Variables                                                                  
------------------------------------------------------------------------------*/

/* USB data buffer */
uint8_t usb_tx_byte[ USB_BUF_SIZE ];
uint8_t usb_rx_byte[ USB_BUF_SIZE ];

/* LoRa global receive buffer */
LORA_STATUS lora_status;
LORA_MESSAGE last_lora_message;
bool start_lora = false;

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

/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/

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

/* Initialize LoRa buffer */
memset(&last_lora_message, 0, LORA_MESSAGE_SIZE);

/* Indicate Successful Initialization */
led_set_color( LED_GREEN );

/* start terminal loop */
usb_receive_IT( usb_rx_byte, 1 );

/* Terminal Mode */
led_set_color( LED_GREEN );

lora_status = lora_set_chip_mode( LORA_RX_CONTINUOUS_MODE );

/*------------------------------------------------------------------------------
Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
    if( start_lora && lora_receive_ready() == LORA_READY )
        {
        led_set_color( LED_CYAN );
        uint8_t rx_buf[LORA_MESSAGE_SIZE];
        uint8_t rx_size = 0;
	    lora_status = lora_receive(rx_buf, LORA_MESSAGE_SIZE, &rx_size);

        if( lora_status == LORA_OK && rx_size == LORA_MESSAGE_SIZE )
            {
            memcpy( &last_lora_message, rx_buf, LORA_MESSAGE_SIZE );
            }
        }

	}
} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/