/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
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
#include <f3d_gyro.h>
#include <stdio.h>

#define TIMER 20000

void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) { 
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  int button_state;
  int i=1;    
  f3d_gyro_init();
  f3d_led_init();
  f3d_uart_init();
  printf("Reset\n");
  float Data[3] = {0};
  float cData[3] = {0};
  int c=0;
  
  
  //initializing button
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  while (1){
    button_state = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
    if (button_state) {
      c++;
      printf("%d\n", c);
      delay();
    }
    while (button_state == 0){
      
      
      f3d_gyro_getdata(Data);    
      delay();
      printf("\n %f %f %f\n", Data[0], Data[1], Data[2]);
      button_state = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
      // if (Data[0] > 1000000000) {
      //	printf("sup");
   
    }
    while (c%4 == 1) {
      printf("this is for x\n");
      f3d_gyro_getdata(Data);    
      printf("%f\n", Data[0]);
    
      delay();
      if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) {
      c++;
      delay();
      
      f3d_gyro_getdata(Data);    
     
      }}

    while (c%4 == 2) {
      printf("this is for y\n");
      f3d_gyro_getdata(Data);    
      printf("%f\n", Data[1]);
      delay();
      if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) {
      c++;
      delay();
    }
    }

    while (c%4 == 3) {
      printf("this is for z\n");
      f3d_gyro_getdata(Data);    
      printf("%f\n", Data[2]);
      delay();
      if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) {
       c++;
      delay();
    }
    }
    
    
    if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0){  
      
      f3d_led_on(i);
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

