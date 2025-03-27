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
#include "lora.h"

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


/* Testing Purposes */
lora_reset();

LORA_CONFIG lora_config = {
	LORA_SLEEP_MODE,
	LORA_SPREAD_6,
	LORA_BANDWIDTH_7_8_KHZ,
	LORA_ECR_4_5,
	LORA_IMPLICIT_HEADER,
	915
};

uint8_t device_id = 0;

LORA_STATUS lora_status = LORA_OK;

lora_status = lora_get_device_id(&device_id);

lora_status = lora_init(&lora_config);

/* Testing Purpose */
uint8_t operation_mode_register;
LORA_STATUS read_status1 = lora_read_register( LORA_REG_OPERATION_MODE, &operation_mode_register );

uint8_t modem_config1_register;
LORA_STATUS read_status2 = lora_read_register( LORA_REG_NUM_RX_BYTES, &modem_config1_register );

uint8_t modem_config2_register;
LORA_STATUS read_status3 = lora_read_register( LORA_REG_RX_HEADER_INFO, &modem_config2_register );

uint8_t freq_reg;
LORA_STATUS read_status4 = lora_read_register( LORA_REG_FREQ_MSB, &freq_reg );
LORA_STATUS read_status5 = lora_read_register( LORA_REG_FREQ_MSD, &freq_reg );
LORA_STATUS read_status6 = lora_read_register( LORA_REG_FREQ_LSB, &freq_reg );

/*------------------------------------------------------------------------------
Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
	/* Receive byte from USB port */
	usb_status = usb_receive( &usb_rx_byte         , 
                              sizeof( uint8_t ), 
                              200 );

	if ( usb_status == USB_OK )
	{
		switch ( usb_rx_byte )
			{
			/*-------------------------------------------------------------
				CONNECT_OP	
			-------------------------------------------------------------*/
			case CONNECT_OP:
				{
				led_set_color(LED_YELLOW);
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
}

/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/