/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Robert Peng and Kevin Kapoun
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 10/31/2013
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_delay.h>
#include <f3d_gyro.h>
#include <stdio.h>
#include <f3d_lcd_sd.h>

int main(void) { 
  f3d_lcd_init();
  f3d_gyro_init();
 f3d_lcd_fillScreen(0xFFFF);
 int charz=0; 
 //draws x y and z on top of the screen
 f3d_lcd_drawChar(20,10,'X',0x000,0x07FF);
 f3d_lcd_drawChar(60,10,'Y',0x000,0x07FF);
 f3d_lcd_drawChar(100,10,'Z',0x000,0x07FF);
 for(charz=2;charz<13;charz++){

 f3d_lcd_drawChar(20,charz*10,charz,0x000,0x07FF);
 f3d_lcd_drawChar(60,charz*10,charz+10,0x000,0x07FF);
 f3d_lcd_drawChar(100,charz*10,charz+20,0x000,0x07FF);
 }
 float Data[3]={0};
 int i=0;
 int colorc=0;
 int f=0;

uint16_t Colors[8]={0x000,0x001F,0x07E0,0x07FF,0xF800,0xF81F,0xFFE0};
 int curx=Data[0];
 while (1){
   f3d_gyro_getdata(Data);

   if(f<1000){
   
  
       for(i=0; i<=7; i++){
	 int c=i;
	 c=c%3;
	 //draws lines for x axis
	 f3d_lcd_drawPixel(11+i,Data[0]+26,Colors[c%7]);
	 f3d_lcd_drawPixel(26+i,Data[0]+26,Colors[c%7]);
	 
       }
       //draws lines for y axis
       for(i=0; i<=7; i++){
	 f3d_lcd_drawPixel(51+i,26+Data[1],Colors[(i+1)%7]);
	 f3d_lcd_drawPixel(66+i,26+Data[1],Colors[(i+1)%7]);
	 
       }
       //draws lines for z axis
       for(i=0; i<=7; i++){
	 f3d_lcd_drawPixel(91+i,26+Data[2],Colors[(i+2)%7]);
	 f3d_lcd_drawPixel(106+i,26+Data[2],Colors[(i+2)%7]);
	 
       }
       colorc++;
       f++;
   } else {
     
 //resets the bars for changes in the axes
     for(i=0;i<=7;i++){
       for(colorc=0;colorc<180;colorc++){  
	 f3d_lcd_drawPixel(11+i,colorc,0xFFFF);
	 f3d_lcd_drawPixel(26+i,colorc,0xFFFF);
       } 
     }
     for(i=0;i<=7;i++){
       for(colorc=0;colorc<180;colorc++){  
	 f3d_lcd_drawPixel(51+i,colorc,0xFFFF);
	 f3d_lcd_drawPixel(66+i,colorc,0xFFFF);
       } 
     }
     for(i=0;i<=7;i++){
       
       for(colorc=0;colorc<180;colorc++){  
	 f3d_lcd_drawPixel(91+i,colorc,0xFFFF);
	 f3d_lcd_drawPixel(106+i,colorc,0xFFFF);
       } 
     }
     
     f=0;
   }
   
 }
 
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
