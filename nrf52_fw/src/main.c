/*
 * main.c
 *
 *  Created on: 2017. 3. 19.
 *      Author: baram
 */
#include "main.h"



void mainInit(void);







int main(void)
{
  uint32_t t_time;
  uint32_t cnt = 0;


  mainInit();



  while(1)
  {
    ledToggle(0);
    delay(500);
    ledToggle(0);
    delay(500);
  }

  return 0;
}

void mainInit(void)
{
  bspInit();
  hwInit();
  apInit();
}
