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
extern UART_HandleTypeDef huart1; /* USB UART  */
extern SPI_HandleTypeDef  hspi2;  /* LORA SPI */

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
RCC_OscInitStruct.PLL.PLLN       = 12;
RCC_OscInitStruct.PLL.PLLP       = 2;
RCC_OscInitStruct.PLL.PLLQ       = 30;
RCC_OscInitStruct.PLL.PLLR       = 2;
RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_3;
RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOMEDIUM;
RCC_OscInitStruct.PLL.PLLFRACN   = 4096;
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
RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

/* Set the configuration */
if ( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_1 ) != HAL_OK )
	{
	Error_Handler( ERROR_SYSCLOCK_CONFIG_ERROR );
	}
} /* SystemClock_Config */

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
huart1.Init.BaudRate               = 921600;
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
*       LORA_SPI_Init                                                          *
*                                                                              *
* DESCRIPTION:                                                                 *
*		Initializes the SPI peripheral for Long Range RFM95 Module               *
*                                                                              *
*******************************************************************************/
void LORA_SPI_Init
	(
		void
	)
{

/* Fill configuration struct with Lora settings */
hspi2.Instance = SPI2;
hspi2.Init.Mode = SPI_MODE_MASTER;
hspi2.Init.Direction = SPI_DIRECTION_2LINES;
hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
hspi2.Init.NSS = SPI_NSS_SOFT;
hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
hspi2.Init.CRCPolynomial = 0x0;
hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
if (HAL_SPI_Init(&hspi2) != HAL_OK)
{
Error_Handler(ERROR_LORA_SPI_INIT_ERROR);
}

}

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
__HAL_RCC_GPIOC_CLK_ENABLE();
__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_GPIOE_CLK_ENABLE();
__HAL_RCC_GPIOB_CLK_ENABLE();

/*--------------------------- LED MCU PINS -----------------------------------*/

/*Configure GPIO pin Output Level */
HAL_GPIO_WritePin( STATUS_GPIO_PORT, 
			       STATUS_R_PIN      |
		           STATUS_G_PIN      |
                   STATUS_B_PIN    , 
				   GPIO_PIN_SET );

HAL_GPIO_WritePin( GPIOC, 
				   LORA_IO5_PIN	|
				   LORA_IO2_PIN, 
				   GPIO_PIN_RESET );

HAL_GPIO_WritePin( GPIOA, 
				   LORA_RST_PIN |
				   LORA_NSS_PIN  | 
				   LORA_IO3_PIN |
				   LORA_IO4_PIN |
				   LORA_IO0_PIN |
				   LORA_IO1_PIN, 
				   GPIO_PIN_RESET );

HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

GPIO_InitStruct.Pin = LORA_IO5_PIN |
					  LORA_IO2_PIN;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

GPIO_InitStruct.Pin = LORA_RST_PIN |
					  LORA_NSS_PIN  |
					  LORA_IO3_PIN |
					  LORA_IO4_PIN |
					  LORA_IO0_PIN |
					  LORA_IO1_PIN;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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