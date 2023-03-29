#ifndef SSD1306_
#define SSD1306_

#include "main.h"
#include <stdint.h> 


#define MSbit             0x80
#define DELETE_CODE       0x7F
#define RUS_YO_CODE       0xA8
#define RUS_YO_NUMBER     0xC0
#define RUS_yo_CODE       0xB8
#define RUS_yo_NUMBER     0xC1
#define SUBTRUCT          0x40

typedef struct {
  const uint8_t *data;	// ��� �������, � ������� �������� �������� �������� ������ ������� 
  uint16_t width;				// ������ ������� � �������� 	
  uint16_t hight;				// ������ ������� � �������� 	
} tImage;


void draw_pixel(int16_t x, int16_t y, uint16_t color); //���������� ������ ������� 
void drawRectangle(uint16_t _width, uint16_t _hight);  //���������� �������������� �������  
void fillRectangle(uint16_t _width, uint16_t _hight);  //���������� ������������� ������������ 
void drawHorizontalLine(uint16_t _length);             //���������� �������������� ����� 
void SSD1306_COMMAND(unsigned int com);							   //�������� ������� 
void drawVerticalLine(uint16_t _hight);                //���������� ������������ ����� 
void setCursor(uint16_t x, uint16_t y);                //��������� ������� 
void SSD1306_DATA(unsigned int data);                  //�������� ������ 
void drawIcon(tImage structureName);                   //���������� �������� 
void ssd1306_clear_display(void);                      //������� ������� 
void printChar(uint8_t BlockNum);                      //���������� ������� 
void printString(char *myString);                      //���������� �������  
void ssd1306_draw_display(void);                       //���������� ����� ������� 
void setColor(uint16_t color);                         //��������� ����� ������� 
void setFont(tImage FontName);                         //��������� ������ 
void drawByte(uint8_t _byte);                          //���������� ����� 
void SSD1306_RESET(void);                              //������������ ������� 
void SSD1306_INIT(void);                               //������������� �������


#endif /*SSD1306_*/

