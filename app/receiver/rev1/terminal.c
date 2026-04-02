/*******************************************************************************
*
* FILE: 
*       terminal.c
*
* DESCRIPTION: 
*		Terminal command loop for reciever.
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
#include "sdr_pin_defines_A0005.h"

/* SDR Modules */
#include "led.h"
#include "math_sdr.h"
#include "usb.h"
#include "commands.h"
#include "error_sdr.h"
#include "onboard_flash.h"
#include "lora.h"

/*------------------------------------------------------------------------------
 Globals                                                                    
------------------------------------------------------------------------------*/
extern LORA_PRESET lora_preset;

/*------------------------------------------------------------------------------
 Procedures                                                 
------------------------------------------------------------------------------*/

USB_STATUS terminal_loop
	(
	uint8_t	firmware_code  /* Board configuration */
	)
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
uint8_t     command_code;                      /* Command opcode              */
//uint8_t     subcommand_code;                   /* Subcommand opcode           */
USB_STATUS 	usb_status;  					   /* Status of USB module        */

/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
// subcommand_code = 0;
usb_status = USB_OK;                 

/* Terminal Mode */
led_set_color( LED_GREEN );

/*------------------------------------------------------------------------------
 Terminal Handler                                                                  
------------------------------------------------------------------------------*/
/* Receive byte from USB port */
usb_status = usb_receive( &command_code         , 
							sizeof( uint8_t ), 
							RECIEVER_TERMINAL_TIMEOUT );

if ( usb_status == USB_OK )
	{
	switch ( command_code )
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
			DASHBOARD_OP	
		-------------------------------------------------------------*/
		case DASHBOARD_OP:
			{
			/* Get dashboard data */
			usb_status = telem_loop();
			break;
			} /* DASHBOARD_OP */
		/*-------------------------------------------------------------
			LORA_OP	
		-------------------------------------------------------------*/
		case LORA_OP:
			{
			uint8_t subcommand_code;
            LORA_STATUS command_status;
			/* Recieve telem subcommand over USB */
			usb_status = usb_receive( &subcommand_code       ,
									sizeof( subcommand_code ),
									HAL_DEFAULT_TIMEOUT );
			
			/* Execute subcommand to mutate or transmit buffer */
            command_status = lora_cmd_execute(subcommand_code, &lora_preset);

			if ( command_status == LORA_OK && subcommand_code == LORA_PRESET_UPLOAD )
				{
                if(onboard_flash_write_addr( (uint32_t)USER_CONFIG_ADDR, &lora_preset, sizeof(lora_preset) ) != HAL_OK)
                    {
                    /* panic, this is worst-case scenario. */
                    error_fail_fast(ERROR_FLASH_CMD_ERROR);
                    }
                
                /* Finally, re-configure the modem */
                if(lora_configure(&lora_preset) != LORA_OK) /* also errors out if configs are invalid! */
                    {
                    error_fail_fast( ERROR_LORA_CMD_ERROR );
                    }
				}
            else if ( command_status != LORA_OK || subcommand_code != LORA_PRESET_DOWNLOAD )
                {
                /* unknown subcommand or usb fail */
                error_fail_fast( ERROR_CONFIG_VALIDITY_ERROR );
                }
			break;
			}
		/*-------------------------------------------------------------
			Unrecognized command code  
		-------------------------------------------------------------*/
		default:
			{
			error_fail_fast(ERROR_INVALID_STATE_ERROR);
			break;
			}

		} /* switch( usb_rx_data ) */
	} /* if ( usb_status == USB_OK ) */

return usb_status;

} /* terminal_loop */


USB_STATUS telem_loop
	(
	void
	)
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
USB_STATUS 	usb_status;  					   /* Status of USB module        */
uint8_t lora_payload[96];

led_set_color(LED_YELLOW);

/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
memset(lora_payload, 0, 96);
usb_status = USB_OK;

/*------------------------------------------------------------------------------
 DUMMY: Construct Payload 
------------------------------------------------------------------------------*/

/* HEADER */
get_uid((void*)lora_payload);
lora_payload[12] = 2;
uint32_t tick = HAL_GetTick();
memcpy( &(lora_payload[16]), &tick, 4 );

/* CONTENTS */
lora_payload[20] = 3; /* state */
float stestims[6] = { 45, 50, 45, 0.2, 0.3, 0.1 };
memcpy( &(lora_payload[21+24]), stestims, 24 );

usb_status = usb_transmit( lora_payload, 96, 40 );

led_set_color(LED_GREEN);

return usb_status;
}

/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/