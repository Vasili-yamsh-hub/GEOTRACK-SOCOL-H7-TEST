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
  const uint8_t *data;	// имя массива, в котором хранятся числовые значения нашего рисунка 
  uint16_t width;				// ширина рисунка в пикселях 	
  uint16_t hight;				// высота рисунка в пикселях 	
} tImage;


void draw_pixel(int16_t x, int16_t y, uint16_t color); //прорисовка одного пикселя 
void drawRectangle(uint16_t _width, uint16_t _hight);  //прорисовка прямоугольника пустого  
void fillRectangle(uint16_t _width, uint16_t _hight);  //прорисовка рямоугольника заполненного 
void drawHorizontalLine(uint16_t _length);             //прорисовка горизонтальной линии 
void SSD1306_COMMAND(unsigned int com);							   //отправка команды 
void drawVerticalLine(uint16_t _hight);                //прорисовка вертикальной линии 
void setCursor(uint16_t x, uint16_t y);                //установка курсора 
void SSD1306_DATA(unsigned int data);                  //отправка данных 
void drawIcon(tImage structureName);                   //прорисовка картинки 
void ssd1306_clear_display(void);                      //очистка дисплея 
void printChar(uint8_t BlockNum);                      //прорисовка символа 
void printString(char *myString);                      //прорисовка строчки  
void ssd1306_draw_display(void);                       //прорисовка всего дисплея 
void setColor(uint16_t color);                         //установка цвета пикселя 
void setFont(tImage FontName);                         //установка шрифта 
void drawByte(uint8_t _byte);                          //прорисовка байта 
void SSD1306_RESET(void);                              //перезагрузка дисплея 
void SSD1306_INIT(void);                               //инициализация дисплея


#endif /*SSD1306_*/

