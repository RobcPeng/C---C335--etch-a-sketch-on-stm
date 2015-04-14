/*Filename: mywc.c *
 *Part of: C335 Lab 2 *
 *Created by: Robert Peng *
 *Created on: 08/29/2013 *
 *Last Modified by: Robert Peng *
 *Last Modified on: 09/05/2013 *
 */
#include <stdio.h>
/*
#include <stdlib.h>
#include <string.h>
#include <math.h>
*/

int wc()
{ //declaring variables to use as counters
  int c; //current character
  int lines=1;
  int wordcount=1;
  int characts=1;

  while((c = getchar()) != 0x1b) { 
    //a while loop that checks every character in the std in
    //to see if they match any of the test cases,
    if (c =='\n'){
      characts++;
      lines=lines++;
      wordcount=wordcount++;
    }if(c=='\t'){
      characts++;
    }if(c==' '&& getchar()+1!=' '){
      characts=characts++;
      wordcount++;
    }  else if (c==' '){
      characts++;
    } else {
      characts=characts++;
    }
  }
  //prints the counters
  printf("%d %d %d\n", lines , wordcount, characts);
  return (0);
  
}
