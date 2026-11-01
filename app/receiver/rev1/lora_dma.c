/**
  ******************************************************************************
  * @file           : lora_dma.c
  * @brief          : SPI2 DMA and DIO0 interrupt setup for the LoRa DMA driver.
  ******************************************************************************
  * @copyright
  *
  * Copyright (c) 2026 Sun Devil Rocketry.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is covered under the
  * BSD-3-Clause.
  *
  * https://opensource.org/license/bsd-3-clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "pindefs.h"
#include "error_sdr.h"
#include "lora.h"

/* Globals -------------------------------------------------------------------*/
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

/* Procedures ----------------------------------------------------------------*/

/**
 * @brief Set up the LoRa SPI DMA streams and the DIO0 interrupt.
 *
 * @note Call after LORA_SPI_Init() and before lora_configure().
 */
void lora_dma_init
    (
    void
    )
{
GPIO_InitTypeDef gpio = {0};

__HAL_RCC_DMA1_CLK_ENABLE();

hdma_spi2_rx.Instance                 = DMA1_Stream0;
hdma_spi2_rx.Init.Request             = DMA_REQUEST_SPI2_RX;
hdma_spi2_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
hdma_spi2_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
hdma_spi2_rx.Init.MemInc              = DMA_MINC_ENABLE;
hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
hdma_spi2_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
hdma_spi2_rx.Init.Mode                = DMA_NORMAL;
hdma_spi2_rx.Init.Priority            = DMA_PRIORITY_LOW;
hdma_spi2_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
if ( HAL_DMA_Init( &hdma_spi2_rx ) != HAL_OK )
    {
    error_fail_fast( ERROR_LORA_SPI_INIT_ERROR );
    }
__HAL_LINKDMA( &( LORA_SPI ), hdmarx, hdma_spi2_rx );

hdma_spi2_tx.Instance                 = DMA1_Stream1;
hdma_spi2_tx.Init.Request             = DMA_REQUEST_SPI2_TX;
hdma_spi2_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
hdma_spi2_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
hdma_spi2_tx.Init.MemInc              = DMA_MINC_ENABLE;
hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
hdma_spi2_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
hdma_spi2_tx.Init.Mode                = DMA_NORMAL;
hdma_spi2_tx.Init.Priority            = DMA_PRIORITY_LOW;
hdma_spi2_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
if ( HAL_DMA_Init( &hdma_spi2_tx ) != HAL_OK )
    {
    error_fail_fast( ERROR_LORA_SPI_INIT_ERROR );
    }
__HAL_LINKDMA( &( LORA_SPI ), hdmatx, hdma_spi2_tx );

/* DMA below SPI2. The H7 HAL completes a DMA transfer on the SPI EOT interrupt. */
HAL_NVIC_SetPriority( DMA1_Stream0_IRQn, 3, 0 );
HAL_NVIC_EnableIRQ( DMA1_Stream0_IRQn );
HAL_NVIC_SetPriority( DMA1_Stream1_IRQn, 3, 0 );
HAL_NVIC_EnableIRQ( DMA1_Stream1_IRQn );
HAL_NVIC_SetPriority( SPI2_IRQn, 2, 0 );
HAL_NVIC_EnableIRQ( SPI2_IRQn );

/* DIO0 idles low and goes high on RxDone */
/* EXTI line 6 is shared with PD6 (XBEE_DIO1), which must stay off EXTI */
__HAL_RCC_GPIOA_CLK_ENABLE();
gpio.Pin  = LORA_IO0_PIN;
gpio.Mode = GPIO_MODE_IT_RISING;
gpio.Pull = GPIO_NOPULL;
HAL_GPIO_Init( LORA_IO0_GPIO_PORT, &gpio );
HAL_NVIC_SetPriority( LORA_IO0_EXTI_IRQn, 2, 0 );
HAL_NVIC_EnableIRQ( LORA_IO0_EXTI_IRQn );

} /* lora_dma_init */


/** @brief SPI2 RX DMA stream interrupt. */
void DMA1_Stream0_IRQHandler( void )
{
HAL_DMA_IRQHandler( &hdma_spi2_rx );
}


/** @brief SPI2 TX DMA stream interrupt. */
void DMA1_Stream1_IRQHandler( void )
{
HAL_DMA_IRQHandler( &hdma_spi2_tx );
}


/** @brief SPI2 interrupt, where the H7 HAL completes each DMA transfer. */
void SPI2_IRQHandler( void )
{
HAL_SPI_IRQHandler( &( LORA_SPI ) );
}


/** @brief EXTI lines 5 to 9, carrying DIO0 on PA6. */
void EXTI9_5_IRQHandler( void )
{
HAL_GPIO_EXTI_IRQHandler( LORA_IO0_PIN );
}


/* The LoRa driver releases NSS itself in these callbacks */

/** @brief A DMA receive finished. */
void HAL_SPI_TxRxCpltCallback( SPI_HandleTypeDef *hspi )
{
if ( hspi == &( LORA_SPI ) )
    {
    lora_process_async_cb();
    }
}


/** @brief A DMA transmit finished. */
void HAL_SPI_TxCpltCallback( SPI_HandleTypeDef *hspi )
{
if ( hspi == &( LORA_SPI ) )
    {
    lora_process_async_cb();
    }
}


/** @brief A DMA transfer failed. */
void HAL_SPI_ErrorCallback( SPI_HandleTypeDef *hspi )
{
if ( hspi == &( LORA_SPI ) )
    {
    lora_process_async_error_cb();
    }
}


/** @brief DIO0 rising edge: RxDone, or TxDone after a transmit. */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
if ( GPIO_Pin == LORA_IO0_PIN )
    {
    lora_process_dio0_cb();
    }
}
