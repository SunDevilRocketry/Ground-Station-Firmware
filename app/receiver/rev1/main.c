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

/* SDR Modules */
#include "led.h"
#include "usb.h"
#include "wireless.h"


/*------------------------------------------------------------------------------
Global Variables                                                                  
------------------------------------------------------------------------------*/

/* MCU Peripheral handles */
UART_HandleTypeDef huart4; /* Xbee UART */
UART_HandleTypeDef huart1; /* USB UART  */

/* Wireless Module Settings */
WIRELESS_MOD_CODES wireless_mod = XBEE;


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
uint8_t    rx_byte;     /* Byte recieved from Wireless module */
USB_STATUS usb_status;  /* Status of USB module               */
RF_STATUS  rf_status;   /* Status of wireless module          */


/*------------------------------------------------------------------------------
MCU Initialization                                                                  
------------------------------------------------------------------------------*/
HAL_Init();             /* CMSIS HAL */
SystemClock_Config();   /* SysClock  */
GPIO_Init();            /* GPIO Pins */
XBee_UART_Init();       /* XBee      */
USB_UART_Init();        /* USB       */


/*------------------------------------------------------------------------------
Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
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
			// TODO: Implement rf_lora_receive_byte function 
			// TODO: Remove Error_Handler()
			Error_Handler();
			break;	
			}
		default:
			{
			/* Unrecognized module: invoke error handler */
			Error_Handler();
			break;
			}
		}

	/* Transmit byte to PC over USB */
	if ( rf_status != RF_TIMEOUT )
		{
	    usb_status = usb_transmit( &rx_byte         , 
                                   sizeof( uint8_t ),
                                   HAL_DEFAULT_TIMEOUT  );
		if ( usb_status != USB_OK )
			{
			Error_Handler();
			}
		}
	else
		{
		/* RF UART timeout, do nothing until next byte arrives */
		}
		
	}

} /* main */




/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
*       Error_Handler                                                          * 
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		This function is executed in case of error occurrence                  *
*                                                                              *
*******************************************************************************/
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed
	(
	uint8_t *file, 
	uint32_t line
	)
{
}
#endif /* USE_FULL_ASSERT */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/