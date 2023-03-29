
#include "SSD1306.h"

static uint8_t	buffer[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT/8];
extern I2C_HandleTypeDef hi2c1;
uint16_t currentX, currentY, currentColor;
tImage currentFont;


void SSD1306_COMMAND(unsigned int com){
	uint8_t mode [2] = {0,com};
	HAL_I2C_Master_Transmit(&hi2c1,SSD1306_Addr,mode,2,100);
}
void SSD1306_DATA(unsigned int data){
	uint8_t mode [2] = {0x40,data};
	HAL_I2C_Master_Transmit(&hi2c1,SSD1306_Addr,mode,2,100);
}
void SSD1306_INIT(void){
	SSD1306_COMMAND(SSD1306_DISPLAYOFF);   // Turn display off
	SSD1306_COMMAND(SSD1306_SETDISPLAYCLOCKDIV);
	SSD1306_COMMAND(0x80);
	SSD1306_COMMAND(SSD1306_SETMULTIPLEX);
	//size of display
	SSD1306_COMMAND(0x1F);//128x32
	//SSD1306_COMMAND(0x3F);//128X64
	SSD1306_COMMAND(SSD1306_SETDISPLAYOFFSET);
	SSD1306_COMMAND(0x00);
	SSD1306_COMMAND(SSD1306_SETSTARTLINE|0x00);
	
	// We use internal charge pump
	SSD1306_COMMAND(SSD1306_CHARGEPUMP);
	SSD1306_COMMAND(SSD1306_CHARGEPUMP_ENABLE);
	
	// Horizontal memory mode
	SSD1306_COMMAND(SSD1306_MEMORYMODE);
	SSD1306_COMMAND(SSD1306_HORIZONTAL_ADDRESSING);
	SSD1306_COMMAND(SSD1306_SEGREMAP | SSD1306_SEGREMAP_ON);
	SSD1306_COMMAND(SSD1306_COMSCANDEC);
	SSD1306_COMMAND(SSD1306_SETCOMPINS);
	//size of display
	SSD1306_COMMAND(0x02);//128x32
	//SSD1306_COMMAND(0x12);//128x64
	
	// Max contrast
	SSD1306_COMMAND(SSD1306_SETCONTRAST);
	SSD1306_COMMAND(200);//0xCF
	SSD1306_COMMAND(SSD1306_SETPRECHARGE);
	SSD1306_COMMAND(0xF1);
	SSD1306_COMMAND(SSD1306_SETVCOMDETECT);
	SSD1306_COMMAND(0x40);//0x40
	SSD1306_COMMAND(SSD1306_DISPLAYALLON_RESUME);
	// Non-inverted display
	SSD1306_COMMAND(SSD1306_NORMALDISPLAY);
	
	// Turn display back on
	SSD1306_COMMAND(SSD1306_DISPLAYON);
	
	
	HAL_Delay(100);
}	
void SSD1306_RESET(void){
  /*Fundamental settings*/
  SSD1306_COMMAND(SSD1306_SETCONTRAST);                   
  SSD1306_COMMAND(SSD1306_RESET_CONTRAST);  
  SSD1306_COMMAND(SSD1306_DISPLAYALLON_RESUME);   
  SSD1306_COMMAND(SSD1306_NORMALDISPLAY);                       
  SSD1306_COMMAND(SSD1306_DISPLAYOFF);                       
  
  /*Scroll settings*/
  SSD1306_COMMAND(SSD1306_SET_VERTICAL_SCROLL_AREA);                       
  SSD1306_COMMAND(SSD1306_RESET_TOP_ROW);                       
  SSD1306_COMMAND(SSD1306_RESET_ROWS_NUMBER);    
  
  /*Addressing settings*/                       
  SSD1306_COMMAND(SSD1306_SETLOWCOLUMN | SSD1306_RESET_LOWER_NIBBLE);                       
  SSD1306_COMMAND(SSD1306_SETHIGHCOLUMN | SSD1306_RESET_HIGHER_NIBBLE);                       
  SSD1306_COMMAND(SSD1306_MEMORYMODE);                       
  SSD1306_COMMAND(SSD1306_PAGE_ADDRESSING);                       
  SSD1306_COMMAND(SSD1306_COLUMNADDR);                       
  SSD1306_COMMAND(SSD1306_RESET_COLUMNADDR);                       
  SSD1306_COMMAND(SSD1306_PAGEADDR);                       
  SSD1306_COMMAND(SSD1306_RESET_PAGEADDR);                       
  SSD1306_COMMAND(SSD1306_PAGESTARTADDR | SSD1306_PAGE0);                           

  /*Hardware settings*/      
  SSD1306_COMMAND(SSD1306_SETSTARTLINE | SSD1306_RESET_STARTLINE);
  SSD1306_COMMAND(SSD1306_SEGREMAP | SSD1306_SEGREMAP_OFF);
  SSD1306_COMMAND(SSD1306_SETMULTIPLEX);                
  SSD1306_COMMAND(SSD1306_RESET_MUX_RATIO);  
  SSD1306_COMMAND(SSD1306_COMSCANINC);                  
  SSD1306_COMMAND(SSD1306_SETDISPLAYOFFSET);              
  SSD1306_COMMAND(SSD1306_RESET_OFFSET);  
  SSD1306_COMMAND(SSD1306_SETCOMPINS);                    
  SSD1306_COMMAND(SSD1306_RESET_SETCOMPINS);     
  SSD1306_COMMAND(SSD1306_SETDISPLAYCLOCKDIV);            
  SSD1306_COMMAND(SSD1306_RESET_CLOCKDIV);       
  SSD1306_COMMAND(SSD1306_SETPRECHARGE);
  SSD1306_COMMAND(SSD1306_RESET_PRECHARGE);  
  SSD1306_COMMAND(SSD1306_SETVCOMDETECT);                 
  SSD1306_COMMAND(SSD1306_RESET_SETVCOMDETECT);                          
  SSD1306_COMMAND(SSD1306_CHARGEPUMP);                    
  SSD1306_COMMAND(SSD1306_CHARGEPUMP_DISABLE); 
	
	
}
	
void ssd1306_clear_display(void) {
  memset(buffer, 0, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
}
	
void ssd1306_draw_display(void) {
	uint16_t size = (SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT/8);  
  
	SSD1306_COMMAND(SSD1306_COLUMNADDR);
  SSD1306_COMMAND(0);   
  SSD1306_COMMAND(SSD1306_LCDWIDTH-1); 
  SSD1306_COMMAND(SSD1306_PAGEADDR);
  SSD1306_COMMAND(0); 
  SSD1306_COMMAND(7); 
	
	//возможно есть ошибка !!!!!
	
	buffer[0] = 0x40;
	HAL_I2C_Master_Transmit(&hi2c1,SSD1306_Addr,buffer,size,100);
}

void draw_pixel(int16_t x, int16_t y, uint16_t color) {
  switch (color) 
    {
      case SSD1306_WHITE:   buffer[(x+1)+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
      case SSD1306_BLACK:   buffer[(x+1)+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break; 
    }
}	
	
void setCursor(uint16_t x, uint16_t y) {
  currentX = x; 
  currentY = y;
}

void setColor(uint16_t color) {
  currentColor = color; 
}

void drawVerticalLine(uint16_t _hight) {
  while(_hight--) {
    draw_pixel(currentX, currentY, currentColor);  
    currentY++;
  }
}

void drawHorizontalLine(uint16_t _length) {
  while(_length--) {
    draw_pixel(currentX, currentY, currentColor);  
    currentX++;
  }
}

void drawRectangle(uint16_t _width, uint16_t _hight) {
  uint16_t startX = currentX, startY = currentY;   
  
  drawVerticalLine(_hight);
  currentY--;  
  drawHorizontalLine(_width);

  currentX = startX; currentY = startY; 
  
  drawHorizontalLine(_width); 
  currentX--;  
  drawVerticalLine(_hight);    
}

void fillRectangle(uint16_t _width, uint16_t _hight) {
  uint16_t startY = currentY;  

  while(_width--) {
    drawVerticalLine(_hight);    
    currentX++;    
    currentY = startY;
  }
}

void drawByte(uint8_t _byte) {
  uint8_t bitNumber = 8;
  while(bitNumber--) 
		{
    if((_byte & MSbit) != 0) 
			{
				draw_pixel(currentX, currentY, currentColor);   
      }
    _byte <<= 1;
    currentY++;
    }
}

void drawIcon(tImage structureName) {
  uint16_t startX = currentX, startY = currentY;

  uint16_t imageByteNum = 0;
  
  uint16_t bytesPerHight = (uint16_t)(structureName.hight / 8); 
  if ((bytesPerHight * 8) != structureName.hight) bytesPerHight += 1;
  
  for(uint16_t imageHigt = 0; imageHigt < bytesPerHight; imageHigt++) {
    for(uint16_t imageWidth = 0; imageWidth < structureName.width; imageWidth++) {
      drawByte(structureName.data[imageByteNum]);
      currentX++;
      currentY = startY;
      imageByteNum++;      
    }
      
      startY += 8;
      currentY = startY;  
      currentX = startX;
  }      
}

void setFont(tImage FontName){
  currentFont.data = FontName.data; 
  currentFont.width = FontName.width; 
  currentFont.hight = FontName.hight;  
}

void printChar(uint8_t BlockNum) {
  uint16_t startX = currentX, startY = currentY;
 
  uint16_t bytesPerHight = (uint16_t)(currentFont.hight / 8); 
  if ((bytesPerHight * 8) != currentFont.hight) bytesPerHight += 1;  

  uint16_t imageByteNum = BlockNum*bytesPerHight*currentFont.width;
  
  for(uint16_t imageHigt = 0; imageHigt < bytesPerHight; imageHigt++) {
    for(uint16_t imageWidth = 0; imageWidth < currentFont.width; imageWidth++) {
      drawByte(currentFont.data[imageByteNum]);
      currentX++;
      currentY = startY;
      imageByteNum++;      
    }
      
      startY += 8;
      currentY = startY;  
      currentX = startX;
  }          
}

void printString(char *myString){
  uint16_t startY = currentY, startX = currentX;
  uint16_t BlockNum = 0;
  while(*myString != '\0')
  	{
		if(*myString <= DELETE_CODE)
			{
				BlockNum = *myString;
			}
				printChar(BlockNum);  
      	currentY = startY;
      	startX += (currentFont.width + 1);
      	currentX = startX;
      	myString++;
	}	
}






