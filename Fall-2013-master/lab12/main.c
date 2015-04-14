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
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <f3d_dac.h>
#include <f3d_timer2.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <diskio.h>
#include <ff.h>
#include <queue.h>

#define TIMER 20000
#define AUDIOBUFSIZE 128

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */
BYTE Buff[512];		/* File read buffer */
int ret;

struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}


const char *b[1];
int mode=0;
void pmenu(){
  nunchuk_t DataN;
  short ax;
  short ay;
  short az;
  int button_state = 0;
  int hold;
  while (1){//change rc 
    
    if (mode==0){ //menu
      f3d_lcd_drawString(20, 10, "thermo.wav", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 30, "eraseout.wav", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 50, "wooshout.wav", 0x000, 0xFFFF); 
    }
    
    button_state = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
    f3d_nunchuk_read(&DataN);

    if (DataN.c==1) {
      mode=hold;
     
    }

    //screen modes
    //nunchuk up
    if (DataN.jy>=255){
      //      f3d_lcd_fillScreen(0xFFFF);
      hold=1;
      f3d_lcd_drawString(20, 10, "thermo.wav", 0x000, 0x07FF); 
      f3d_lcd_drawString(20, 30, "eraseout.wav", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 50, "wooshout.wav", 0x000, 0xFFFF); 
      printf("mode is %d", mode);
      b[0]="thermo.wav";
    }
    //nunchuk down
    else if (DataN.jy==0){
      //      f3d_lcd_fillScreen(0xFFFF);
      hold=2;
      f3d_lcd_drawString(20, 10, "thermo.wav", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 30, "eraseout.wav", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 50, "wooshout.wav", 0x000, 0x07FF); 
      printf("mode is %d", mode);
      b[0]="wooshout.wav";
    }
    //nunchuk middle
    else {
      hold=3;
      f3d_lcd_drawString(20, 10, "thermo.wav", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 30, "eraseout.wav", 0x000, 0x07FF); 
      f3d_lcd_drawString(20, 50, "wooshout.wav", 0x000, 0xFFFF);
      printf("mode is %d", mode);
      b[0]="eraseout.wav";
    }
  }
}





int main(void) { 
 
  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;

  
 

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
 
  f3d_i2c1_init();
  delay(10);
  f3d_uart_init();
  delay(10); 
  f3d_timer2_init();
  delay(10);
  f3d_dac_init();
  delay(10); 
  f3d_delay_init();
  delay(10); 
  f3d_rtc_init();
  delay(10);
  f3d_systick_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  while(1){
    
    pmenu(mode);
    printf("%d", mode);
 

  
  
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_fillScreen(BLUE);
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_fillScreen(BLUE);
  f3d_lcd_fillScreen(WHITE);
  
  printf("Reset\n");
  const char *a[4];
  a[1]="thermo.wav";
  a[2]="wooshout.wav";
  a[3]="eraseout.wav";


  
  /*
  */
  f_mount(0, &Fatfs);/* Register volume work area */
  /*switch(mode)
    {
    case 1:
      printf("\nOpen thermo.wav\n");
      //char = thermo.wav // array[0]
      //  rc = f_open(&fid, "thermo.wav", FA_READ);
    case 2:
      printf("\nOpen eraseout.wav\n");
	//char = eraseout.wav //arrow [1]
     
    case 3:
      printf("\nOpen wooshout.wav\n");
      
    }
  */
  rc = f_open(&fid, a[mode], FA_READ); 

  if (!rc) {
    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return -1;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    
    // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    
    //      audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  
  if (rc) die(rc);
  
  mode=0;
  
  }

  while (1);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
