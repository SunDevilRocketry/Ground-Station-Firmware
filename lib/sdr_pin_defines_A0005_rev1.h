/*******************************************************************************
*                                                                              *
* FILE:                                                                        * 
* 		sdr_pin_defines_A0005_rev1.h                                           *
*                                                                              *
* DESCRIPTION:                                                                 * 
* 	    Contains all the MCU pin definitions for revision 1 of the ground      *	
*       station unit                                                           *
*                                                                              *
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SDR_PIN_DEFINES 
#define SDR_PIN_DEFINES 

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------------
 MCU Pin Assignments                                                          
------------------------------------------------------------------------------*/

/* LED */
#define STATUS_B_PIN	   GPIO_PIN_11 
#define STATUS_G_PIN       GPIO_PIN_12  
#define STATUS_R_PIN	   GPIO_PIN_10  

/* USB */
#define USB_RX_PIN         GPIO_PIN_9
#define USB_TX_PIN         GPIO_PIN_10

/* XBee Mopule */
#define XBEE_RX_PIN 	    GPIO_PIN_0
#define XBEE_TX_PIN         GPIO_PIN_1
#define XBEE_MOSI_PIN       GPIO_PIN_6
#define XBEE_MISO_PIN       GPIO_PIN_5
#define XBEE_SCK_PIN        GPIO_PIN_2
#define XBEE_SS_PIN         GPIO_PIN_5
#define XBEE_SLEEP_PIN		GPIO_PIN_2
#define XBEE_RST_PIN        GPIO_PIN_13
#define XBEE_PWM0_PIN       GPIO_PIN_0
#define XBEE_PWM1_PIN       GPIO_PIN_1
#define XBEE_DIO0_PIN       GPIO_PIN_7
#define XBEE_DIO1_PIN       GPIO_PIN_6
#define XBEE_DIO5_PIN       GPIO_PIN_3
#define XBEE_DIO6_PIN       GPIO_PIN_4
#define XBEE_DIO7_PIN       GPIO_PIN_1
#define XBEE_DIO8_PIN       GPIO_PIN_14

/* LoRa Module */
#define LORA_MOSI_PIN	    GPIO_PIN_
#define LORA_MISO_PIN	    GPIO_PIN_
#define LORA_SCK_PIN	    GPIO_PIN_
#define LORA_SS_PIN	        GPIO_PIN_
#define LORA_RST_PIN        GPIO_PIN_
#define LORA_IO0_PIN        GPIO_PIN_
#define LORA_IO1_PIN        GPIO_PIN_
#define LORA_IO2_PIN        GPIO_PIN_
#define LORA_IO3_PIN        GPIO_PIN_
#define LORA_IO4_PIN        GPIO_PIN_
#define LORA_IO5_PIN        GPIO_PIN_


/*------------------------------------------------------------------------------
 MCU Port Assignments                                                          
------------------------------------------------------------------------------*/

/* LED */
#define STATUS_GPIO_PORT	      GPIOE

/* USB */
#define USB_GPIO_PORT             GPIOA	

/* XBee Mopule */
#define XBEE_RX_GPIO_PORT	      GPIOA
#define XBEE_TX_GPIO_PORT         GPIOA
#define XBEE_MOSI_GPIO_PORT       GPIOE
#define XBEE_MISO_GPIO_PORT       GPIOE
#define XBEE_SCK_GPIO_PORT        GPIOE
#define XBEE_SS_GPIO_PORT         GPIOD
#define XBEE_SLEEP_GPIO_PORT	  GPIOD
#define XBEE_RST_GPIO_PORT        GPIOC
#define XBEE_PWM0_GPIO_PORT       GPIOB
#define XBEE_PWM1_GPIO_PORT       GPIOB
#define XBEE_DIO0_GPIO_PORT       GPIOD
#define XBEE_DIO1_GPIO_PORT       GPIOD
#define XBEE_DIO5_GPIO_PORT       GPIOD
#define XBEE_DIO6_GPIO_PORT       GPIOD
#define XBEE_DIO7_GPIO_PORT       GPIOD
#define XBEE_DIO8_GPIO_PORT       GPIOC

/* LoRa Module */
#define LORA_MOSI_GPIO_PORT	      GPIOB
#define LORA_MISO_GPIO_PORT       GPIOB
#define LORA_SCK_GPIO_PORT	      GPIOB
#define LORA_SS_GPIO_PORT	      GPIOA
#define LORA_RST_GPIO_PORT        GPIOA
#define LORA_IO0_GPIO_PORT        GPIOA
#define LORA_IO1_GPIO_PORT        GPIOA
#define LORA_IO2_GPIO_PORT        GPIOC
#define LORA_IO3_GPIO_PORT        GPIOA
#define LORA_IO4_GPIO_PORT        GPIOA
#define LORA_IO5_GPIO_PORT        GPIOC


#ifdef __cplusplus
}
#endif

#endif /* SDR_PIN_DEFINES */
