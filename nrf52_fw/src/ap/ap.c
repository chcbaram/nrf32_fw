/*
 *  ap.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "ap.h"




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
  cmdifPrintf("Start..\n");

  while(1)
  {
    cmdifMain();
  }
}

