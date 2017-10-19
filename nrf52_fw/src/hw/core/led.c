/*
 *  led.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */
#include "hw.h"
#include "led.h"
#include "nrf_gpio.h"


typedef struct
{
  uint16_t      pin_number;
} led_port_t;


led_port_t led_port_tbl[LED_CH_MAX] =
{
    {17}
};



int ledCmdif(int argc, char **argv);




void ledInit(void)
{
  uint32_t i;


  for (i=0; i<LED_CH_MAX; i++)
  {
    nrf_gpio_cfg_output(led_port_tbl[i].pin_number);
    ledOff(i);
  }


  cmdifAdd("led", ledCmdif);
}

void ledOn(uint8_t ch)
{
  if (ch >= LED_CH_MAX) return;

  nrf_gpio_pin_write(led_port_tbl[ch].pin_number, 0);
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_CH_MAX) return;
  nrf_gpio_pin_write(led_port_tbl[ch].pin_number, 1);
}

void ledToggle(uint8_t ch)
{
  if (ch >= LED_CH_MAX) return;

  nrf_gpio_pin_toggle(led_port_tbl[ch].pin_number);
}



//-- ledCmdif
//
int ledCmdif(int argc, char **argv)
{
  bool ret = true;
  uint8_t number;


  if (argc == 3)
  {
    number = (uint8_t) strtoul((const char * ) argv[2], (char **)NULL, (int) 0);

    if(strcmp("on", argv[1]) == 0)
    {
      ledOn(number);
    }
    else if(strcmp("off", argv[1])==0)
    {
      ledOff(number);
    }
    else if(strcmp("toggle", argv[1])==0)
    {
      ledToggle(number);
    }
    else if(strcmp("demo", argv[1])==0)
    {
      while(cmdifRxAvailable() == 0)
      {
        ledToggle(number);
        delay(200);
        cmdifPrintf("led toggle %d\n", number);
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cmdifPrintf( "led on/off/toggle/demo number ...\n");
  }

  return 0;
}
