/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Nov 14 07:57:37 2013
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

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
 
#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_uart.h>
#include <queue.h>

volatile int systick_flag = 0;

int state = 0;
int light = 0;
struct queue txbuf;



void f3d_systick_init(void) {
 SysTick_Config(SystemCoreClock/100);
}

void SysTick_Handler(void) {
  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
    SysTick_Config(SystemCoreClock/10);
  } else 
    SysTick_Config(SystemCoreClock/100);


  if(light>=8){
    light=0;
  } 
  if(state%2){
    f3d_led_on(light);
    light++;
 } 
  else{
    f3d_led_all_off();
  }
  state++;
 
}

/* f3d_systick.c ends here */
