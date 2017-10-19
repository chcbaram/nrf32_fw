/*
 *  delay.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram, PBHP
 */

#include "delay.h"
#include "nrf_drv_systick.h"


#define USE_LFXO




static volatile uint32_t overflows = 0;




void delayInit(void)
{
  NVIC_SetPriority(RTC1_IRQn, 15);
  NVIC_ClearPendingIRQ(RTC1_IRQn);
  NVIC_EnableIRQ(RTC1_IRQn);

  #if defined( USE_LFXO )
    // 32Khz XTAL
    NRF_CLOCK->LFCLKSRC = (uint32_t)((CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos) & CLOCK_LFCLKSRC_SRC_Msk);
  #elif defined(USE_LFRC)
    // Internal OSC
    NRF_CLOCK->LFCLKSRC = (uint32_t)((CLOCK_LFCLKSRC_SRC_RC << CLOCK_LFCLKSRC_SRC_Pos) & CLOCK_LFCLKSRC_SRC_Msk);
  #else
    #error Clock Source is not configured, define USE_LFXO or USE_LFRC according to your board
  #endif
  NRF_CLOCK->TASKS_LFCLKSTART = 1UL;

  NRF_RTC1->PRESCALER = 0;
  NRF_RTC1->INTENSET = RTC_INTENSET_OVRFLW_Msk;
  NRF_RTC1->EVTENSET = RTC_EVTEN_OVRFLW_Msk;
  NRF_RTC1->TASKS_START = 1;
}

void delay(uint32_t ms)
{
  nrf_drv_systick_delay_ms(ms);
}

uint32_t millis()
{
  uint64_t ticks = (uint64_t)((uint64_t)overflows << (uint64_t)24) | (uint64_t)(NRF_RTC1->COUNTER);

  return (uint32_t)((ticks * 1000) / 32768);
}

uint32_t micros()
{
  uint64_t ticks = (uint64_t)((uint64_t)overflows << (uint64_t)24) | (uint64_t)(NRF_RTC1->COUNTER);

  return (uint32_t)((ticks * 1000000) / 32768);
}


void delay_ns(uint32_t ns)
{
  for (volatile uint32_t i = 0; i < ns/10; i++) { }
}

void delay_us(uint32_t us)
{
  uint32_t t_time;


  t_time = micros();

  while(1)
  {
    if ((micros()-t_time) >= us)
    {
      break;
    }
  }
}

void delay_ms(uint32_t ms)
{
  nrf_drv_systick_delay_ms(ms);
}


void RTC1_IRQHandler(void)
{
  NRF_RTC1->EVENTS_OVRFLW = 0;

#if __CORTEX_M == 0x04
    volatile uint32_t dummy = NRF_RTC1->EVENTS_OVRFLW;
    (void)dummy;
#endif

  overflows = (overflows + 1) & 0xff;
}
