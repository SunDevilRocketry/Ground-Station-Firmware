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
#include "telemetry.h"

/*------------------------------------------------------------------------------
 Globals                                                                    
------------------------------------------------------------------------------*/
extern LORA_PRESET lora_preset;
extern uint8_t usb_tx_byte[ USB_BUF_SIZE ];
extern uint8_t usb_rx_byte[ USB_BUF_SIZE ];
extern bool start_lora;
extern LORA_MESSAGE last_lora_message;

/*------------------------------------------------------------------------------
 Procedures                                                 
------------------------------------------------------------------------------*/

USB_STATUS terminal_loop
	(
	void
	)
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
uint8_t     command_code = usb_rx_byte[0];             /* Command opcode              */
static USB_STATUS usb_status = USB_OK;  			   /* Status of USB module        */                

/*------------------------------------------------------------------------------
 Terminal Handler                                                                  
------------------------------------------------------------------------------*/

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
            usb_tx_byte[0] = FIRMWARE_RECEIVER;
			usb_status = usb_transmit_IT( usb_tx_byte, 
						                  sizeof( uint8_t ) );
			break;
			} /* CONNECT_OP */
		/*-------------------------------------------------------------
			DASHBOARD_OP	
		-------------------------------------------------------------*/
		case DASHBOARD_OP:
			{
            /* begin LoRa polling mode*/
            start_lora = true;

			/* Get dashboard data */
			memcpy(usb_tx_byte, &last_lora_message, LORA_MESSAGE_SIZE);

            /* transmit */
            usb_status = usb_transmit_IT(usb_tx_byte, LORA_MESSAGE_SIZE);
			break;
			} /* DASHBOARD_OP */
		/*-------------------------------------------------------------
			LORA_OP	
		-------------------------------------------------------------*/
		case LORA_OP:
			{
			uint8_t subcommand_code;
            LORA_STATUS command_status;
            
            /* This command will block long enough to mess up the LoRa blocking functions. 
               If LoRa has already started, we can't allow re-configuration without implementing
               a mechanism to request a stop from the other thread. */
            if( start_lora )
                {
                error_fail_fast( ERROR_LORA_CMD_ERROR );
                }

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
if( usb_status == USB_OK )
    {
    usb_status = usb_receive_IT( usb_rx_byte, 1 );
    }
else
    {
    error_fail_fast( ERROR_USB_UART_ERROR );
    }

return usb_status;

} /* terminal_loop */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/