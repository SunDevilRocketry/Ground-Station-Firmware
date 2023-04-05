/*******************************************************************************
*                                                                              *
* FILE:                                                                        *
* 		init.c                                                                 *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Contains initialization routines for MCU core and peripherals          *
*                                                                              *
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                               
------------------------------------------------------------------------------*/
#include "sdr_pin_defines_A0005.h"
#include "main.h"
#include "init.h"
#include "sdr_error.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Procedures 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
*       SystemClock_Config                                                     *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Initializes the microcontroller clock. Enables peripheral clocks and   *
*       sets prescalers                                                        *
*                                                                              *
*******************************************************************************/
void SystemClock_Config
	(
	void
	)
{
/* Initialization Structs */
RCC_OscInitTypeDef RCC_OscInitStruct = {0};
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

/* Supply configuration update enable */
HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

/* Configure the main internal regulator output voltage */
__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) 
	{
	/* Wait for PWR_FLAG_VOSRDY flag */
	}

/* Initializes the RCC Oscillators according to the specified parameters
   in the RCC_OscInitTypeDef structure. */
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
RCC_OscInitStruct.PLL.PLLM       = 2;
RCC_OscInitStruct.PLL.PLLN       = 80;
RCC_OscInitStruct.PLL.PLLP       = 2;
RCC_OscInitStruct.PLL.PLLQ       = 2;
RCC_OscInitStruct.PLL.PLLR       = 2;
RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_3;
RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;
RCC_OscInitStruct.PLL.PLLFRACN   = 0;
if ( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
	{
	Error_Handler( ERROR_SYSCLOCK_CONFIG_ERROR );
	}

/* Initializes the CPU, AHB and APB buses clocks */
RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK    |
	                               RCC_CLOCKTYPE_SYSCLK  |
						           RCC_CLOCKTYPE_PCLK1   |
                                   RCC_CLOCKTYPE_PCLK2   |
						           RCC_CLOCKTYPE_D3PCLK1 |
		                           RCC_CLOCKTYPE_D1PCLK1;
RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

/* Set the configuration */
if ( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_4 ) != HAL_OK )
	{
	Error_Handler( ERROR_SYSCLOCK_CONFIG_ERROR );
	}
} /* SystemClock_Config */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
*       XBee_UART_Init                                                         *
*                                                                              *
* DESCRIPTION:                                                                 *
*		Initializes the UART peripheral attached to the XBee module            *
*                                                                              *
*******************************************************************************/
void XBee_UART_Init
	(
	void
	)
{
/* Fill configuration struct with UART settings */
huart4.Instance                    = UART4;
huart4.Init.BaudRate               = 9600;
huart4.Init.WordLength             = UART_WORDLENGTH_8B;
huart4.Init.StopBits               = UART_STOPBITS_1;
huart4.Init.Parity                 = UART_PARITY_NONE;
huart4.Init.Mode                   = UART_MODE_TX_RX;
huart4.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
huart4.Init.OverSampling           = UART_OVERSAMPLING_16;
huart4.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
huart4.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

/* Apply configuration */
if ( HAL_UART_Init( &huart4 ) != HAL_OK )
	{
	Error_Handler( ERROR_XBEE_UART_CONFIG_ERROR );
	}
if ( HAL_UARTEx_SetTxFifoThreshold( &huart4, UART_TXFIFO_THRESHOLD_1_8 ) != HAL_OK )
	{
	Error_Handler( ERROR_XBEE_UART_CONFIG_ERROR );
	}
if ( HAL_UARTEx_SetRxFifoThreshold( &huart4, UART_RXFIFO_THRESHOLD_1_8 ) != HAL_OK )
	{
	Error_Handler( ERROR_XBEE_UART_CONFIG_ERROR );
	}
if ( HAL_UARTEx_DisableFifoMode( &huart4 ) != HAL_OK )
	{
	Error_Handler( ERROR_XBEE_UART_CONFIG_ERROR );
	}

} /* XBee_UART_Init */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
*       USB_UART_Init                                                          *
*                                                                              *
* DESCRIPTION:                                                                 *
*		Initializes the UART peripheral attached to the USB port               *
*                                                                              *
*******************************************************************************/
void USB_UART_Init 
	(
	void
	)
{

/* Fill configuration struct with UART settings */
huart1.Instance                    = USART1;
huart1.Init.BaudRate               = 9600;
huart1.Init.WordLength             = UART_WORDLENGTH_8B;
huart1.Init.StopBits               = UART_STOPBITS_1;
huart1.Init.Parity                 = UART_PARITY_NONE;
huart1.Init.Mode                   = UART_MODE_TX_RX;
huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
huart1.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

/* Apply configuration */
if ( HAL_UART_Init( &huart1 ) != HAL_OK )
	{
	Error_Handler( ERROR_USB_UART_INIT_ERROR );
	}
if ( HAL_UARTEx_SetTxFifoThreshold( &huart1, UART_TXFIFO_THRESHOLD_1_8 ) != HAL_OK )
	{
	Error_Handler( ERROR_USB_UART_INIT_ERROR );
	}
if ( HAL_UARTEx_SetRxFifoThreshold( &huart1, UART_RXFIFO_THRESHOLD_1_8 ) != HAL_OK )
	{
	Error_Handler( ERROR_USB_UART_INIT_ERROR );
	}
if ( HAL_UARTEx_DisableFifoMode( &huart1 ) != HAL_OK )
	{
	Error_Handler( ERROR_USB_UART_INIT_ERROR );
	}

} /*  USB_UART_Init() */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
*       GPIO_Init                                                              *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Initializes all GPIO pins and sets alternate functions                 *
*                                                                              *
*******************************************************************************/
void GPIO_Init
	(
	void
	)
{
/* GPIO Initialization Struct */
GPIO_InitTypeDef GPIO_InitStruct = {0};

/* GPIO Ports Clock Enable */
__HAL_RCC_GPIOH_CLK_ENABLE();
__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_GPIOE_CLK_ENABLE();

/*--------------------------- LED MCU PINS -----------------------------------*/

/*Configure GPIO pin Output Level */
HAL_GPIO_WritePin( STATUS_GPIO_PORT, 
			       STATUS_R_PIN      |
		           STATUS_G_PIN      |
                   STATUS_B_PIN    , 
				   GPIO_PIN_SET );

/*Configure GPIO pins : PE10 PE11 PE12 */
GPIO_InitStruct.Pin   = STATUS_R_PIN |
                        STATUS_G_PIN |
                        STATUS_B_PIN;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
GPIO_InitStruct.Pull  = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init( GPIOE, &GPIO_InitStruct );

} /* GPIO_Init */


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/