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
#include <string.h>

#include "main.h"

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
extern TELEMETRY_MESSAGE last_lora_message;

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
			memcpy(usb_tx_byte, &last_lora_message, TELEMETRY_MESSAGE_SIZE);

            /* transmit */
            usb_status = usb_transmit_IT(usb_tx_byte, TELEMETRY_MESSAGE_SIZE);
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

                /* lora_configure leaves the radio in standby */
                lora_set_chip_mode( LORA_RX_CONTINUOUS_MODE );
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
*                                                                              *
* PROCEDURE:                                                                   *
* 		lora_cmd_execute                                                       *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Execute a LoRa terminal command.                                       *
*                                                                              *
*******************************************************************************/
LORA_STATUS lora_cmd_execute
    (
    uint8_t subcommand_code,
    LORA_PRESET* lora_preset_buf
    )
{
switch (subcommand_code)
    {
    /*-------------------------------------------------------------
     Upload Preset (to FC)
    -------------------------------------------------------------*/
    case LORA_PRESET_UPLOAD:
        {
        /* Recieve preset subcommand over USB */
        uint8_t data_receive_buffer[sizeof( LORA_PRESET )];
        if (usb_receive( data_receive_buffer,
                                sizeof( LORA_PRESET ),
                                10 * HAL_DEFAULT_TIMEOUT ) == USB_OK)
            {
            /* Copy received data into preset data */
            memcpy(lora_preset_buf, data_receive_buffer, sizeof( LORA_PRESET ) );
            return LORA_OK;
            }
        else
            {
            /* lora presets remain untouched if usb receive fails */
            return LORA_FAIL;
            }
        }
    /*-------------------------------------------------------------
     Download Preset (from FC)
    -------------------------------------------------------------*/
    case LORA_PRESET_DOWNLOAD:
        {
        /* tx straight from buffer (usb transmit does not modify the buffer) */
        if( usb_transmit( lora_preset_buf, sizeof( LORA_PRESET ), 10 * HAL_DEFAULT_TIMEOUT ) == USB_OK )
            {
            return LORA_OK;
            }
        else
            {
            return LORA_FAIL;
            }
        }
    /*-------------------------------------------------------------
     Unrecognized command code
    -------------------------------------------------------------*/
    default:
        {
        return LORA_FAIL;
        }
    }

} /* lora_cmd_execute */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/