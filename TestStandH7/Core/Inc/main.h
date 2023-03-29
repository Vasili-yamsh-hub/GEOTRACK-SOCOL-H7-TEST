/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SSD1306.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define SSD1306_Addr 0x78
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_RESET_CONTRAST 0x7F
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RESET_TOP_ROW 0
#define SSD1306_RESET_ROWS_NUMBER 64
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_RESET_LOWER_NIBBLE 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_RESET_HIGHER_NIBBLE 0x00
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_HORIZONTAL_ADDRESSING 0x00
#define SSD1306_VERTICAL_ADDRESSING 0x01
#define SSD1306_PAGE_ADDRESSING 0x02
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_RESET_COLUMNADDR 0x00
#define SSD1306_PAGEADDR 0x22
#define SSD1306_RESET_PAGEADDR 0x00
#define SSD1306_PAGESTARTADDR 0xB0
#define SSD1306_PAGE0 0x00
#define SSD1306_PAGE1 0x01
#define SSD1306_PAGE2 0x02
#define SSD1306_PAGE3 0x03
#define SSD1306_PAGE4 0x04
#define SSD1306_PAGE5 0x05
#define SSD1306_PAGE6 0x06
#define SSD1306_PAGE7 0x07
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_RESET_STARTLINE 0x00
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_SEGREMAP_OFF 0x00
#define SSD1306_SEGREMAP_ON 0x01
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_RESET_MUX_RATIO 0x3F
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_RESET_OFFSET 0x00
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_RESET_SETCOMPINS 0x12
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_RESET_CLOCKDIV 0x80
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_RESET_PRECHARGE 0x22
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_RESET_SETVCOMDETECT 0x20
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_CHARGEPUMP_DISABLE 0x10
#define SSD1306_CHARGEPUMP_ENABLE 0x14
#define SSD1306_BLACK 0
#define SSD1306_WHITE 1

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define W5500_INT_Pin GPIO_PIN_10
#define W5500_INT_GPIO_Port GPIOB
#define W5500_CS_Pin GPIO_PIN_11
#define W5500_CS_GPIO_Port GPIOB
#define W5500_RST_Pin GPIO_PIN_12
#define W5500_RST_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
