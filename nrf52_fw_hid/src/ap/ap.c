/*
 *  ap.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "ap.h"

extern void bleMouseLoop(void);


void test(void)
{
  ledToggle(0);
}


void apInit(void)
{
  cmdifBegin(_DEF_UART1, 115200);
}

void apMain(void)
{
  uint32_t pre_time;


  cmdifPrintf("Start..\n");

  pre_time = millis();
  while(1)
  {
    cmdifMain();

    if (millis()-pre_time >= 1000)
    {
      pre_time = millis();
      ledToggle(0);
    }

    bleMouseLoop();
  }
}

