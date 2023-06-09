/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "wizchip_conf.h"
#include "socket.h"
#include "loopback.h"

#define HTTP_SOCKET 0
#define PORT_TCPS 5000
#define DATA_BUF_SIZE 2048
uint8_t gDATABUF [DATA_BUF_SIZE];

//cmd from pc names:
#define pcCmd_IP     (0x0001) // IP setting 
#define pcCmd_MASK   (0x0002) // MASK setting 
#define pcCmd_PORT   (0x0003) // PORT setting 
#define pcCmd_RTC    (0x0004) // RTC setting
#define pcCmd_VERS   (0x0005) // Firmware Version setting
#define pcCmd_SRT    (0x0006) // Sample Rate setting
#define pcCmd_SNCH   (0x0007) // sensor - channel setting
#define pcCmd_PRS0   (0x0008) // pressure 0
#define pcCmd_PRS1   (0x000A) // pressure 1
#define pcCmd_WGHT   (0x000B) // weight
#define pcCmd_TRNS_R (0x000C) // turns (drum), read cmd
#define pcCmd_TRNS_W (0x000D) // turns (drum), write cmd
#define pcCmd_RESET  (0x000E) // reset device
#define pcCmd_WTIME  (0x000F) // find out the operating time of the device
#define pcCmd_LOGS   (0x0010) // get logs 
#define pcCmd_HLTH   (0x0011) // get heath 
#define pcCmd_RFD    (0x0012) // setting transmitted data to RFD (RFD MODE)
#define pcCmd_FUPD   (0x0013) // firmware update



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint16_t cmdFromPC = 0;

typedef struct {
  uint16_t deviceNumber; //device part number
	uint16_t sensorChannel; 
	uint16_t pressure_0;
	uint16_t pressure_1;
	uint16_t weight;
	signed int turnsCounter; // max size -2147483648  +2147483647
	uint16_t pumpFrequency; 
	uint16_t rfdMode;
} __attribute__((packed, aligned(1))) DataToPC_t;


const uint8_t image_data_MonoFont1_6x13[] = 
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
0x00, 0x00, 0xc0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x11, 0x1f, 0xf1, 0x13, 0xfd, 0x10, 0xc0, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x00, 0x70, 0xdf, 0xfe, 0xc3, 0x00, 0x00, 0x40, 0xe0, 0xc0, 0x80, 0x00,
0x88, 0x73, 0x04, 0x18, 0x61, 0x82, 0xc0, 0x00, 0x00, 0x00, 0x80, 0x40,
0x43, 0xa4, 0xb8, 0xe4, 0x03, 0x03, 0x80, 0x40, 0x40, 0x40, 0x80, 0xc0,
0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00,
0x00, 0x40, 0xf0, 0xf0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x02, 0x02, 0x07, 0x02, 0x02, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
0x00, 0x03, 0x0c, 0x30, 0xc0, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x7f, 0x80, 0x80, 0x7f, 0x00, 0x00, 0x80, 0x40, 0x40, 0x80, 0x00,
0x00, 0x00, 0x20, 0x20, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00,
0x00, 0x40, 0x83, 0x8c, 0x70, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x40, 0x00,
0x00, 0x88, 0x88, 0x7c, 0x07, 0x00, 0x00, 0x40, 0x40, 0x40, 0x80, 0x00,
0x07, 0xf9, 0x01, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x00, 0x00, 0xf8, 0x88, 0x88, 0x87, 0x00, 0x00, 0x40, 0x40, 0x40, 0x80,
0x3f, 0x48, 0x88, 0x88, 0x0c, 0x07, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80,
0x00, 0xc0, 0x80, 0x80, 0x9f, 0xe0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x07, 0x78, 0x88, 0x88, 0x78, 0x07, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80,
0x00, 0x78, 0x84, 0x84, 0x85, 0x7e, 0x00, 0x00, 0x40, 0x40, 0x80, 0x00,
0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00,
0x00, 0x00, 0x03, 0x03, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80,
0x00, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x04, 0x02, 0x03, 0x01, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00,
0x40, 0x80, 0x8f, 0x88, 0x70, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
0x61, 0x7f, 0xa1, 0xa1, 0xbf, 0x41, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40,
0x00, 0x07, 0x3a, 0xe2, 0x1e, 0x01, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0,
0x00, 0xff, 0x88, 0x88, 0x78, 0x07, 0x00, 0xc0, 0x40, 0x40, 0x40, 0x80,
0x00, 0x00, 0x7f, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40,
0x00, 0xff, 0x80, 0x80, 0x7f, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00,
0x00, 0x00, 0xff, 0x88, 0x88, 0x80, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x40,
0x00, 0x00, 0xff, 0x88, 0x88, 0x88, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x7f, 0x80, 0x88, 0x8f, 0x00, 0x00, 0x80, 0x40, 0x40, 0xc0, 0x00,
0x00, 0xff, 0x08, 0x08, 0xff, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xc0, 0x00,
0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x40, 0x40, 0x40, 0x80, 0x00,
0x00, 0xff, 0x08, 0x1c, 0x63, 0x80, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0,
0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x40,
0xff, 0x60, 0x18, 0x18, 0x60, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0,
0x00, 0xff, 0x3c, 0x03, 0xff, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xc0, 0x00,
0x00, 0x7f, 0x80, 0x80, 0x7f, 0x00, 0x00, 0x80, 0x40, 0x40, 0x80, 0x00,
0x00, 0xff, 0x84, 0x84, 0x78, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x7f, 0x80, 0x82, 0x81, 0x7f, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80,
0x00, 0xff, 0x86, 0x85, 0x78, 0x00, 0x00, 0xc0, 0x00, 0x80, 0x40, 0x00,
0x00, 0x60, 0x98, 0x84, 0x82, 0x41, 0x00, 0x40, 0x40, 0x40, 0x40, 0x80,
0x00, 0x80, 0x80, 0xff, 0x80, 0x80, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x80, 0x40, 0x40, 0x80, 0x00,
0x00, 0xf8, 0x07, 0x01, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x03, 0x07, 0x18, 0x0e, 0x01, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00,
0x00, 0xc0, 0x33, 0x0c, 0x33, 0xc0, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0,
0x00, 0xc0, 0x30, 0x0f, 0x30, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x00, 0x81, 0x86, 0x98, 0xe0, 0x80, 0x00, 0xc0, 0x40, 0x40, 0x40, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x20, 0x00,
0xc0, 0x30, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x20,
0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x20, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10,
0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x1f, 0x10, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0x40, 0xc0, 0x40,
0x00, 0x00, 0xff, 0x10, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0x40, 0xc0, 0x00,
0x00, 0x00, 0x1f, 0x10, 0x10, 0x10, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x40,
0x00, 0x00, 0x1f, 0x10, 0xff, 0x00, 0x00, 0x00, 0xc0, 0x40, 0xc0, 0x00,
0x00, 0x00, 0x1f, 0x12, 0x1e, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x00,
0x00, 0x10, 0xff, 0x90, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x1f, 0x10, 0x1f, 0x00, 0x00, 0x00, 0xc8, 0x48, 0xf8, 0x00,
0x00, 0x00, 0xff, 0x10, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xc0, 0x00,
0x00, 0x00, 0x00, 0x9f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x9f, 0x00, 0x00, 0x00, 0x00, 0x08, 0xf8, 0x00, 0x00,
0x00, 0x00, 0xff, 0x07, 0x08, 0x10, 0x00, 0x00, 0xc0, 0x00, 0xc0, 0x40,
0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x00, 0x00,
0x1f, 0x10, 0x10, 0x1f, 0x10, 0x1f, 0xc0, 0x00, 0x00, 0xc0, 0x00, 0xc0,
0x00, 0x00, 0x1f, 0x10, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xc0, 0x00,
0x00, 0x00, 0x1f, 0x10, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0x40, 0xc0, 0x00,
0x00, 0x00, 0x1f, 0x10, 0x1f, 0x00, 0x00, 0x00, 0xf8, 0x40, 0xc0, 0x00,
0x00, 0x00, 0x1f, 0x10, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0x40, 0xf8, 0x00,
0x00, 0x00, 0x1f, 0x10, 0x10, 0x08, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x1c, 0x12, 0x11, 0x10, 0x00, 0x00, 0x40, 0x40, 0x40, 0x80,
0x00, 0x10, 0xff, 0x10, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0x40, 0xc0, 0x00,
0x00, 0x00, 0x1e, 0x01, 0x0f, 0x10, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x1f, 0x01, 0x0e, 0x07, 0x00, 0x1f, 0x00, 0xc0, 0x00, 0x00, 0xc0, 0x00,
0x00, 0x00, 0x18, 0x07, 0x0d, 0x10, 0x00, 0x00, 0xc0, 0x00, 0x80, 0x40,
0x00, 0x00, 0x1f, 0x00, 0x0f, 0x10, 0x00, 0x00, 0x08, 0xf0, 0x00, 0x00,
0x00, 0x00, 0x11, 0x12, 0x1c, 0x10, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x40,
0x00, 0x00, 0x04, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x20, 0x00,
0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x80, 0xff, 0x04, 0x00, 0x00, 0x00, 0x20, 0xe0, 0x00, 0x00,
0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

tImage MonoFont1_6x13 = {image_data_MonoFont1_6x13,6,13};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */

//�������� �������������� 
static wiz_NetInfo NetConf = {
  {0x0c,0x29,0xab,0x7c,0x04,0x02},  // mac address
  { 192, 168, 0, 113 },             // Local IP address
  { 255, 255, 255, 0 },             // Subnet mask
  { 192, 168, 0, 1 },               // Gateway address
  { 192, 168, 0, 1 },               // DNS server address
  NETINFO_STATIC                    // Use static IP
};


//wiz_NetInfo gWIZNETINFO = { .mac = { 0x4B,0xAA,0xBB,0xCC,0xDD,0xEE},
//								.ip = { 192, 168, 0, 113 },
//								.sn = { 255, 255, 255, 0 },
//					      .gw = { 192, 168, 0, 1 },
//					      //.gw = {46, 191, 230, 1},
//								//.dns = { 89, 189, 156, 5},
//								.dns = { 192, 168, 0, 1 },
//					      .dhcp = NETINFO_STATIC };


volatile wiz_PhyConf phyConf =
{
  PHY_CONFBY_HW,       // PHY_CONFBY_SW
  PHY_MODE_MANUAL,     // PHY_MODE_AUTONEGO
  PHY_SPEED_10,        // PHY_SPEED_100
  PHY_DUPLEX_FULL,     // PHY_DUPLEX_HALF
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void configNet(){
  wiz_NetInfo conf;
  // Configure network address
  ctlnetwork(CN_SET_NETINFO,(void *)&NetConf);
  // read back
  ctlnetwork(CN_GET_NETINFO,(void *)&conf);
  if(memcmp(&conf,&NetConf,sizeof(wiz_NetInfo)) == 0){
    // Successful configuration
  }else{
    // configuration failed
  }
}

void W5500_select() {
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET); //CS LOW
}
 
void W5500_unselect() {
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET); //CS HIGH
}
 
void W5500_ReadBuff(uint8_t* buff, uint16_t len)
{
	HAL_SPI_Receive(&hspi1, buff, len, HAL_MAX_DELAY);
}

 
void W5500_WriteBuff(uint8_t* buff, uint16_t len)
{
	HAL_SPI_Transmit(&hspi1, buff, len, HAL_MAX_DELAY);
}

uint8_t W5500_ReadByte(void)
{
	uint8_t byte;
	W5500_ReadBuff(&byte, sizeof(byte));
	return byte;
}

void W5500_WriteByte(uint8_t byte)
{
	W5500_WriteBuff(&byte, sizeof(byte));
}

void W5500_reset(){
	HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
}

uint8_t stat;
uint8_t reqnr;
char Message[128];
char inputMessage[128];


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	
	W5500_reset();
	reg_wizchip_cs_cbfunc(W5500_select, W5500_unselect);
  reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
	reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);
	
	uint8_t ar[16] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}; // All send and receive buffers are set to 2KB (default)
	ctlwizchip(CW_INIT_WIZCHIP,ar);
	
	uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
	wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
	configNet();
		
	//configuration displays
	SSD1306_RESET();           //���������� �������� ������� 
	SSD1306_INIT();            //������������� ������� 
	setColor(SSD1306_WHITE);   //���������� ���� ����� ������� 
	setFont(MonoFont1_6x13);   //��������� ������ 
	
	
	SSD1306_COMMAND(0xC8);	//(0xC0)	 //��������� ��������� ������� 
	SSD1306_COMMAND(0xA1);  //(0xA0)   //��������� ��������� ������� 
	
	ssd1306_clear_display();
	setCursor(0,0);
	drawRectangle(128,32);
		
	setCursor(1,5);
	printString("Track Test V.1");
	ssd1306_draw_display();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
		printf("Creating socket...\r\n");
		//�������� TCP ������
		stat = socket(HTTP_SOCKET, Sn_MR_TCP, PORT_TCPS, 0); 
		if(stat != HTTP_SOCKET) printf("socket() failed, code = %d\r\n", stat);
			else printf("Socket created, connecting...\r\n");
		//�������� ����������
		
		stat = listen(HTTP_SOCKET);
			if(stat != SOCK_OK) printf("listen() failed, code = %d\r\n", stat);
    else printf("listen() OK\r\n");
		
		while(getSn_SR(HTTP_SOCKET) == SOCK_LISTEN)
    {
			HAL_Delay(10);
    }

		printf("Input connection! socket state = %x\r\n", getSn_SR(HTTP_SOCKET));
		if(getSn_SR(HTTP_SOCKET) != SOCK_ESTABLISHED) printf("Error socket status\r\n");
		uint8_t rIP[4];
		uint16_t remotePort;
		getsockopt(HTTP_SOCKET, SO_DESTIP, rIP);
		getsockopt(HTTP_SOCKET, SO_DESTPORT, (uint8_t*)&remotePort);
		printf("IP:  %d.%d.%d.%d  PORT: %u\r\n", rIP[0], rIP[1], rIP[2], rIP[3], remotePort);

		
		sprintf(Message, "Connected to SOKOL BOX!\r\n");
		send(HTTP_SOCKET, (uint8_t*)Message, strlen(Message));
		printf("sended to client: %s\r\n", Message);
		
		while ((getSn_SR(HTTP_SOCKET) == SOCK_ESTABLISHED)|(getSn_SR(HTTP_SOCKET) == SOCK_SYNRECV)) {
			int32_t nbytes = recv(HTTP_SOCKET, (uint8_t*)inputMessage, sizeof(inputMessage)-1);
		
			sprintf(Message, "echo: %s\r\n", inputMessage);	
			send(HTTP_SOCKET, (uint8_t*)Message, strlen(Message));
		
			printf("cmd from client: %s, b = %x\r\n", inputMessage, nbytes);
		}
		disconnect(HTTP_SOCKET);
		close(HTTP_SOCKET);
		
		
//			
			printf("Closing socket.\r\n");
		
//		
			HAL_Delay(1000);
//		reqnr++;
		
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20404768;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, W5500_INT_Pin|W5500_CS_Pin|W5500_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : W5500_INT_Pin W5500_RST_Pin */
  GPIO_InitStruct.Pin = W5500_INT_Pin|W5500_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : W5500_CS_Pin */
  GPIO_InitStruct.Pin = W5500_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(W5500_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
