/* led.c --- 
 * 
 * Filename: led.c
 * Description: 
 * Author: Kevin Kapoun & Robert Peng
 * Maintainer: 
 * Created: 9/19/2013
 * Last-Updated: 9/21/2013
 *           By: Kevin Kapoun & Robert Peng
 *     Update #: 1
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
#include <stm32f30x.h>
#include <f3d_led.h>

void f3d_led_init(void) {
  // initialize the port and pins for the 8 leds
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;


  

}

void f3d_led_on(int led) {
  // enable the led specified by led parameter
  // led = 0-7 representing the 8 leds
  //GPIOE->BSRR = led;
  /* 
     
  GPIOE->BSRR = GPIO_Pin_15;
  GPIOE->BSRR = GPIO_Pin_14;
  GPIOE->BSRR = GPIO_Pin_12;
  GPIOE->BSRR = GPIO_Pin_11;
  GPIOE->BSRR = GPIO_Pin_10;
  GPIOE->BSRR = GPIO_Pin_9;
  GPIOE->BSRR = GPIO_Pin_8;
  */
  switch(led)
    {
    case 7:
      GPIOE->BSRR=GPIO_Pin_15;
      break;
    case 6:
      GPIOE->BSRR=GPIO_Pin_14;
      break;
    case 5:
      GPIOE->BSRR=GPIO_Pin_13;
      break;
    case 4:
      GPIOE->BSRR=GPIO_Pin_12;
      break;
    case 3:
      GPIOE->BSRR=GPIO_Pin_11;
      break;
    case 2:
      GPIOE->BSRR=GPIO_Pin_10;
      break;
    case 1:
      GPIOE->BSRR=GPIO_Pin_9;
      break;
    case 0:
      GPIOE->BSRR=GPIO_Pin_8;
      break;
      
    }
}
 
void f3d_led_off(int led) {
  // disable the led specified by led parameter
  // led = 0-7 representing the 8 leds

  GPIOE->BRR = led; //GPIO_Pin_15
  switch(led)
    {
    case 7:
      GPIOE->BRR=GPIO_Pin_15;
      break;
    case 6:
      GPIOE->BRR=GPIO_Pin_14;
      break;
    case 5:
      GPIOE->BRR=GPIO_Pin_13;
      break;
    case 4:
      GPIOE->BRR=GPIO_Pin_12;
      break;
    case 3:
      GPIOE->BRR=GPIO_Pin_11;
      break;
    case 2:
      GPIOE->BRR=GPIO_Pin_10;
      break;
    case 1:
      GPIOE->BRR=GPIO_Pin_9;
      break;
    case 0:
      GPIOE->BRR=GPIO_Pin_8;
      break;
      
    }
 
} 

void f3d_led_all_on(void) {
  // turn on all 8 leds 
  GPIOE->BSRR=GPIO_Pin_8;
  GPIOE->BSRR=GPIO_Pin_9;
  GPIOE->BSRR=GPIO_Pin_10;
  GPIOE->BSRR=GPIO_Pin_11;
  GPIOE->BSRR=GPIO_Pin_12;
  GPIOE->BSRR=GPIO_Pin_13;
  GPIOE->BSRR=GPIO_Pin_14;
  GPIOE->BSRR=GPIO_Pin_15;
} 

void f3d_led_all_off(void) {
  // turn off all 8 leds
  GPIOE->BRR=GPIO_Pin_8;
  GPIOE->BRR=GPIO_Pin_9;
  GPIOE->BRR=GPIO_Pin_10;
  GPIOE->BRR=GPIO_Pin_11;
  GPIOE->BRR=GPIO_Pin_12;
  GPIOE->BRR=GPIO_Pin_13;
  GPIOE->BRR=GPIO_Pin_14;
  GPIOE->BRR=GPIO_Pin_15;
} 

/* led.c ends here */

