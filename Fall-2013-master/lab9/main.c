/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: Robert Peng & Kevin Kapoun
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 11/10/13
 *           By: 
 *     Update #: 4
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
#include <f3d_i2c.h>
#include <f3d_nunchuk.h>
#include <math.h>

int main(void) { 
  
  f3d_lcd_init();
  f3d_gyro_init();
  f3d_led_init();
  f3d_uart_init();
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);
  f3d_lcd_fillScreen(0xFFFF);
  int charz=0; 

  char xout[32];
  char yout[32];
  char zout[32];

  short ax;
  short ay;
  short az;
  int c;
  int i=0;
  int h=0;
  int colorc=0;
  int f=0;
  int button_state = 0;
  uint16_t Colors[8]={0x000,0x001F,0x07E0,0x07FF,0xF800,0xF81F,0xFFE0};
  float DataB[3]={0};
  nunchuk_t DataN;
  float DataM[3];
  int mode=4;
  
  while (1){
    button_state = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
    f3d_nunchuk_read(&DataN);

    az = DataN.az;
    ay = DataN.ay;
    ax = DataN.ax;

    //screen modes
    //nunchuk left
    if((DataN.jx == 0) && (DataN.jy>=123) && (DataN.c==1)){
      f3d_lcd_fillScreen(0xFFFF);
      mode=1;    
    }
    //nunchuk up
    else if ((DataN.jx==125) && (DataN.jy>=255) && (DataN.c==1)){
      f3d_lcd_fillScreen(0xFFFF);
      mode=2;
    }
    //nunchuk right
    else if ((DataN.jx==255) && (DataN.jy<=123) && (DataN.c==1)){
      f3d_lcd_fillScreen(0xFFFF);
      mode=3; 
    } 
    else if ((DataN.jx==125) && (DataN.jy==0) && (DataN.c==1)){
      f3d_lcd_fillScreen(0xFFFF);
      mode=4;
    }

  
    //endof screen modes

    //compass 
    if(mode==1){
      f3d_lcd_drawString(10, 10, "Compass!", 0x000,0x07FF);

      f3d_mag_read(DataM);
      sprintf(xout,"x %f", DataM[0]);
      sprintf(yout,"y %f", DataM[1]);
      sprintf(zout,"z %f", DataM[2]);
      f3d_lcd_drawString( 20, 20, xout, 0x000,0x07FF);
      f3d_lcd_drawString( 20, 30, yout, 0x000,0x07FF);
      f3d_lcd_drawString( 20, 40, zout, 0x000,0x07FF);


      if((DataM[0] <= -.02) && (DataM[1] <= -1.3) && (DataM[2] <= -3.3)){
	f3d_lcd_drawString(20, 50,  "direction: East", 0x000,0x07FF);
      }
    
      else if((DataM[0] >= .07) && (DataM[1] >= .08) && (DataM[2] <= -.35)){
	f3d_lcd_drawString(20, 50,  "direction: South", 0x000,0x07FF);
      }
	
      else if((DataM[0] <= -.08) && (DataM[1] >= .16) && (DataM[2] <=-.3)){
	f3d_lcd_drawString(20, 50,  "direction: West", 0x000,0x07FF);
      }
      
      else if((DataM[0] <= -.2) && (DataM[1] >= .03) && (DataM[2] <=-.3)){
	f3d_lcd_drawString(20, 50,  "direction: North", 0x000,0x07FF);
      }
      
    }	

 

     

    //board   
    if(mode==2){ 
      f3d_lcd_drawChar(20,10,'X',0x000,0x07FF);
      f3d_lcd_drawChar(60,20,'Y',0x000,0x07FF);
      f3d_lcd_drawChar(100,10,'Z',0x000,0x07FF);
      f3d_lcd_drawChar(60,9,'G', 0x000, 0x07FF);
      f3d_gyro_getdata(DataB);
      if(f<100){
	for(i=0; i<=7; i++){
	  
	  c=c%3;
	  //draws lines for x axis,y and z
	  f3d_lcd_drawPixel(11+i,26+DataB[0]+0,Colors[c]);
	  f3d_lcd_drawPixel(26+i,26+DataB[0]+0,Colors[c]);
	  f3d_lcd_drawPixel(51+i,26+DataB[1],Colors[(i+1)]);
	  f3d_lcd_drawPixel(66+i,26+DataB[1],Colors[(i+1)]);
	  f3d_lcd_drawPixel(91+i,26+DataB[2],Colors[(i+2)]);
	  f3d_lcd_drawPixel(106+i,26+DataB[2],Colors[(i+2)]);
	}
	
	colorc++;
	f++;
      } else {
	
	//resets the bars for changes in the axes
	for(i=0;i<=7;i++){
	  for(colorc=0;colorc<180;colorc++){  
	    f3d_lcd_drawPixel(11+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(26+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(51+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(66+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(91+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(106+i,colorc,0xFFFF);
	  } 
	}
	colorc=0;
	f=0;
      }
    }//end board
      
    
      //for nunchuck
     
      if(mode==3){
	f3d_lcd_drawChar(20,10,'X',0x000,0x07FF);
	f3d_lcd_drawChar(60,20,'Y',0x000,0x07FF);
	f3d_lcd_drawChar(100,10,'Z',0x000,0x07FF);
	f3d_lcd_drawChar(60,9,'N', 0x000,0x07FF);
	f3d_nunchuk_read(&DataN);
      if(f<50){
	for(i=0; i<=7; i++){
	  
	  c=c%3;
	  //draws lines for x axis
	  f3d_lcd_drawPixel(11+i,26+DataN.ax,Colors[c]);
	  f3d_lcd_drawPixel(26+i,26+DataN.ax,Colors[c]);
	  f3d_lcd_drawPixel(51+i,26+DataN.ay,Colors[(i+1)]);
	  f3d_lcd_drawPixel(66+i,26+DataN.ay,Colors[(i+1)]);
	  f3d_lcd_drawPixel(91+i,26+DataN.az,Colors[(i+2)]);
	  f3d_lcd_drawPixel(106+i,26+DataN.az,Colors[(i+2)]);
	}
	
	colorc++;
	f++;
      } else { 
	//resets the bars for changes in the axes
	for(i=0;i<=7;i++){
	  for(colorc=0;colorc<180;colorc++){  
	    f3d_lcd_drawPixel(11+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(26+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(51+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(66+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(91+i,colorc,0xFFFF);
	    f3d_lcd_drawPixel(106+i,colorc,0xFFFF);	 
	  } 
	}
	colorc=0;
	f=0;
      }
      }
      	//end nunchuck

      if (mode==4){
	f3d_lcd_drawString(20, 10, "Make selection", 0x000, 0x07FF);
	f3d_lcd_drawChar(10, 20, '<', 0x000, 0x07FF); 
	f3d_lcd_drawString(20, 20, "Compass", 0x000, 0x07FF);
	f3d_lcd_drawChar(10, 30, '^', 0x000, 0x07FF);
	f3d_lcd_drawString(20, 30, "Gyro", 0x000, 0x07FF);
	f3d_lcd_drawChar(10, 40, '>', 0x000, 0x07FF);
	f3d_lcd_drawString(20, 40, "Accelerometer", 0x000, 0x07FF);
	f3d_lcd_drawString(10,60, "Pick using", 0x000, 0x07FF);
	f3d_lcd_drawString(10,70, "Nunchuk stick", 0x000, 0x07FF);
	f3d_lcd_drawString(10,80, "and C button", 0x000, 0x07FF);
	
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
