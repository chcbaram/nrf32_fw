/*
 * hwtimer.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */



#include <stdarg.h>
#include <stdbool.h>
#include "hw.h"
#include "hwtimer.h"
#include "timer/nrf_drv_timer.h"



const nrf_drv_timer_t TIMER_1 = NRF_DRV_TIMER_INSTANCE(0);
const nrf_drv_timer_t TIMER_2 = NRF_DRV_TIMER_INSTANCE(1);

nrf_drv_timer_config_t timer_cfg_1 = NRF_DRV_TIMER_DEFAULT_CONFIG;
nrf_drv_timer_config_t timer_cfg_2 = NRF_DRV_TIMER_DEFAULT_CONFIG;

typedef struct
{
  nrf_drv_timer_t        *p_timer;
  nrf_drv_timer_config_t *p_timer_cfg;
  uint8_t  enable;
  uint32_t freq;
  voidFuncPtr handler;
} hwtimer_t;



//-- Internal Variables
//
static hwtimer_t timer_tbl[_HW_DEF_TIMER_CH_MAX];



//-- External Variables
//


//-- Internal Functions
//
void timer_1_event_handler(nrf_timer_event_t event_type, void* p_context);
void timer_2_event_handler(nrf_timer_event_t event_type, void* p_context);


//-- External Functions
//




bool hwtimerInit(void)
{
  uint8_t i;


  //-- TIM1
  //
  timer_tbl[_DEF_TIMER1].p_timer     = (nrf_drv_timer_t *)&TIMER_1;
  timer_tbl[_DEF_TIMER1].p_timer_cfg = &timer_cfg_1;

  nrf_drv_timer_init(timer_tbl[_DEF_TIMER1].p_timer, timer_tbl[_DEF_TIMER1].p_timer_cfg, timer_1_event_handler);


  //-- TIM2
  //
  timer_tbl[_DEF_TIMER2].p_timer     = (nrf_drv_timer_t *)&TIMER_2;
  timer_tbl[_DEF_TIMER2].p_timer_cfg = &timer_cfg_2;

  nrf_drv_timer_init(timer_tbl[_DEF_TIMER2].p_timer, timer_tbl[_DEF_TIMER2].p_timer_cfg, timer_2_event_handler);


  for( i=0; i<_HW_DEF_TIMER_CH_MAX; i++ )
  {
    timer_tbl[i].handler = NULL;
    timer_tbl[i].enable  = 0;
  }

  return true;
}

void hwtimerStop(uint8_t channel)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;

  nrf_drv_timer_disable(timer_tbl[channel].p_timer);

  timer_tbl[channel].enable = 0;
}

void hwtimerSetPeriod(uint8_t channel, uint32_t period_us)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;

  uint32_t time_ticks;

  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;

  time_ticks = nrf_drv_timer_us_to_ticks(timer_tbl[channel].p_timer, period_us);
  nrf_drv_timer_extended_compare(timer_tbl[channel].p_timer, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
}

void hwtimerAttachInterrupt(uint8_t channel, voidFuncPtr handler)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;

  hwtimerStop(channel);

  timer_tbl[channel].handler = handler;

}


void hwtimerDetachInterrupt(uint8_t channel)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;


  timer_tbl[channel].handler = NULL;
}

void hwtimerStart(uint8_t channel)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;

  nrf_drv_timer_enable(timer_tbl[channel].p_timer);

  timer_tbl[channel].enable = 1;
}






void timer_1_event_handler(nrf_timer_event_t event_type, void* p_context)
{

  switch (event_type)
  {
    case NRF_TIMER_EVENT_COMPARE0:
      if (timer_tbl[_DEF_TIMER1].handler != NULL)
      {
         (*timer_tbl[_DEF_TIMER1].handler)();
      }
      break;

    default:
      //Do nothing.
      break;
  }
}

void timer_2_event_handler(nrf_timer_event_t event_type, void* p_context)
{

  switch (event_type)
  {
    case NRF_TIMER_EVENT_COMPARE0:
      if (timer_tbl[_DEF_TIMER2].handler != NULL)
      {
         (*timer_tbl[_DEF_TIMER2].handler)();
      }
      break;

    default:
      //Do nothing.
      break;
  }
}


