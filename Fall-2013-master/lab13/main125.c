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


void pmenu(void){
  nunchuk_t DataN;
  short ax;
  short ay;
  short az;
  int hold;

  while (1){//change rc  
    f3d_nunchuk_read(&DataN);

    if (DataN.c==1) {
      mode=hold;
      break;
    }

    //screen modes
    //nunchuk up
 if (DataN.jy==0){
      //      f3d_lcd_fillScreen(0xFFFF);
      hold=2;
      f3d_lcd_drawString(20, 10, "Welcome", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 20, "To the Awesome", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 30, "World of the", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 40, "Etchasketch!", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 60, "Play!", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 70, "Saves(?)", 0x000, 0x07FF); 
    }
    //nunchuk middle
    else {
      hold=3;
      f3d_lcd_drawString(20, 10, "Welcome", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 20, "To the Awesome", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 30, "World of the", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 40, "Etchasketch!", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 60, "Play!", 0x000, 0x07FF); 
      f3d_lcd_drawString(20, 70, "Saves(?)", 0x000, 0xFFFF); 
    }
  }
}


int audi(const char *track){ //audio 
  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

 f_mount(0, &Fatfs);
 rc = f_open(&fid, track, FA_READ); 

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
  // while(1);
}


void initfunc(void) {
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
}


void gamestart(void){

  audi("wooshout.wav");
  // gamee();
  //pmenu();
}

void gamee(void){
  f3d_lcd_fillScreen(WHITE);
  


}


int main(void) { 

  initfunc(); 
  
  const char *a[4];
  a[1]="thermo.wav";
  a[2]="wooshout.wav";
  a[3]="eraseout.wav"; 
  f3d_lcd_fillScreen(WHITE);
  gamestart();
  while(1){    
    pmenu();
    if(mode==3){
      gamestart();
      // f3d_lcd_fillScreen(WHITE); 
    } if(mode==2) {
      gamestart();
     // audi(a[1]);
      // f3d_lcd_fillScreen(BLACK); 
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
