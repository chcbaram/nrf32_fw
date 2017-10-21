/*
 * uart.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */
/*
  _HW_DEF_UART_CH_MAX : 2
  _DEF_UART1          : UART0
  _DEF_UART2          :
*/
/*
  USART0
    - RX : 8
    - TX : 6
*/


#include <stdarg.h>
#include <stdbool.h>
#include "hw.h"
#include "nrf52.h"


#include "uart.h"



#define PIN_SERIAL_RX         8
#define PIN_SERIAL_TX         6


#define UART_RX_BUF_LENGTH    64



//-- Internal Variables
//
uart_t  uart_tbl[_HW_DEF_UART_CH_MAX];
volatile uint8_t uart_buffer[_HW_DEF_UART_CH_MAX][UART_RX_BUF_LENGTH];



//-- External Variables
//


//-- Internal Functions
//
static bool uartIsEnable(uint8_t channel);

static uint32_t uartGetNrfBaud(uint32_t baudrate);

//-- External Functions
//


//-- Driver Functions
//




bool uartInit(void)
{
  uint32_t i;


  for(i=0; i<_HW_DEF_UART_CH_MAX; i++)
  {
    uart_tbl[i].is_open           = false;
    uart_tbl[i].hw.rx_buf.ptr_in  = 0;
    uart_tbl[i].hw.rx_buf.ptr_out = 0;
    uart_tbl[i].hw.rx_buf.p_buf   = NULL;
  }


  return true;
}

uint32_t uartOpen(uint8_t channel, uint32_t baud)
{
	uint32_t err_code  = OK;
	uart_t *p_drv_uart = &uart_tbl[channel];


  if (channel >= _HW_DEF_UART_CH_MAX)
  {
    return 1;
  }



  switch(channel)
  {
    case _DEF_UART1:

      p_drv_uart->baud           = baud;
      p_drv_uart->hw.h_uart      = NRF_UART0;

      p_drv_uart->hw.rx_buf.ptr_in  = 0;
      p_drv_uart->hw.rx_buf.ptr_out = 0;
      p_drv_uart->hw.rx_buf.p_buf   = (uint8_t *)uart_buffer[_DEF_UART1];
      p_drv_uart->hw.rx_buf.length  = UART_RX_BUF_LENGTH;


      p_drv_uart->hw.h_uart->PSELTXD = 6;
      p_drv_uart->hw.h_uart->PSELRXD = 8;

      p_drv_uart->hw.h_uart->CONFIG = (UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos) | UART_CONFIG_HWFC_Disabled;

      p_drv_uart->hw.h_uart->BAUDRATE = uartGetNrfBaud(baud);

      p_drv_uart->hw.h_uart->ENABLE = UART_ENABLE_ENABLE_Enabled;

      p_drv_uart->hw.h_uart->EVENTS_RXDRDY = 0x0UL;
      p_drv_uart->hw.h_uart->EVENTS_TXDRDY = 0x0UL;

      p_drv_uart->hw.h_uart->TASKS_STARTRX = 0x1UL;
      p_drv_uart->hw.h_uart->TASKS_STARTTX = 0x1UL;

      p_drv_uart->hw.h_uart->INTENSET = UART_INTENSET_RXDRDY_Msk;

      NVIC_ClearPendingIRQ(UARTE0_UART0_IRQn);
      NVIC_SetPriority(UARTE0_UART0_IRQn, 3);
      NVIC_EnableIRQ(UARTE0_UART0_IRQn);

      p_drv_uart->is_open = true;
      break;
  }


  return err_code;
}

uint32_t uartGetNrfBaud(uint32_t baudrate)
{
  uint32_t nrfBaudRate;


  if (baudrate <= 1200) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud1200;
  } else if (baudrate <= 2400) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud2400;
  } else if (baudrate <= 4800) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud4800;
  } else if (baudrate <= 9600) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud9600;
  } else if (baudrate <= 14400) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud14400;
  } else if (baudrate <= 19200) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud19200;
  } else if (baudrate <= 28800) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud28800;
  } else if (baudrate <= 38400) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud38400;
  } else if (baudrate <= 57600) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud57600;
  } else if (baudrate <= 76800) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud76800;
  } else if (baudrate <= 115200) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud115200;
  } else if (baudrate <= 230400) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud230400;
  } else if (baudrate <= 250000) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud250000;
  } else if (baudrate <= 460800) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud460800;
  } else if (baudrate <= 921600) {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud921600;
  } else {
    nrfBaudRate = UARTE_BAUDRATE_BAUDRATE_Baud1M;
  }


  return nrfBaudRate;
}

bool uartIsEnable(uint8_t channel)
{
  if (channel >= _HW_DEF_UART_CH_MAX)
  {
    return false;
  }

  return uart_tbl[channel].is_open;
}

void uartWaitForEnable(uint8_t channel, uint32_t timeout)
{
  uint32_t t_time;


  t_time = millis();
  while(1)
  {
    if (uartIsEnable(channel) == true)
    {
      break;
    }
    if ((millis()-t_time) >= timeout)
    {
      break;
    }
  }
}

uint32_t uartAvailable(uint8_t channel)
{
  uint32_t length = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];


  length = (   p_drv_uart->hw.rx_buf.length
             + p_drv_uart->hw.rx_buf.ptr_in
             - p_drv_uart->hw.rx_buf.ptr_out ) % p_drv_uart->hw.rx_buf.length;

  return length;
}

void uartPutch(uint8_t channel, uint8_t ch)
{
  uartWrite(channel, &ch, 1 );
}

uint8_t uartGetch(uint8_t channel)
{
  if (uartIsEnable(channel) == false ) return 0;


  while(1)
  {
    if( uartAvailable(channel) ) break;
  }


  return uartRead(channel);
}

int32_t uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length)
{
  int32_t  ret = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];
  uint32_t i;

  if (uartIsEnable(channel) == false ) return 0;


  for (i=0; i<length; i++)
  {
    p_drv_uart->hw.h_uart->TXD = p_data[i];

    while(!p_drv_uart->hw.h_uart->EVENTS_TXDRDY);

    p_drv_uart->hw.h_uart->EVENTS_TXDRDY = 0x0UL;
  }

  ret = length;

  return ret;
}

uint8_t uartRead(uint8_t channel)
{
  uint8_t ret = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];
  uint32_t index;
  volatile uint32_t next_index;


  index      = p_drv_uart->hw.rx_buf.ptr_out;
  next_index = p_drv_uart->hw.rx_buf.ptr_out + 1;

  if (next_index == p_drv_uart->hw.rx_buf.length)
  {
    next_index = 0;;
  }

  if (index != p_drv_uart->hw.rx_buf.ptr_in)
  {
    ret = p_drv_uart->hw.rx_buf.p_buf[index];
    p_drv_uart->hw.rx_buf.ptr_out = next_index;
  }

  return ret;
}

int32_t uartPrintf(uint8_t channel, const char *fmt, ...)
{
  int32_t ret = 0;
  va_list arg;
  va_start (arg, fmt);
  //int32_t len;
  static char print_buffer[255];


  if (uartIsEnable(channel) == false ) return 0;

  vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  ret = uartPrint(channel, (uint8_t *)print_buffer);

  return ret;
}

int32_t uartPrint(uint8_t channel, uint8_t *p_str)
{
  int32_t index = 0;

  if (uartIsEnable(channel) == false ) return 0;

  while(1)
  {
    if (p_str[index] == '\n')
    {
      uartPutch(channel, '\r');
    }
    uartPutch(channel, p_str[index]);

    if (p_str[index] == 0)
    {
      break;
    }

    index++;

    if (index > 255)
    {
      break;
    }
  }


  return index;
}




//--------------------------------------- Hardware Functions
//
void UARTE0_UART0_IRQHandler()
{
  uint8_t ch;
  uint32_t index;


  if (uart_tbl[_DEF_UART1].hw.h_uart->EVENTS_RXDRDY)
  {
    ch = (uint8_t)uart_tbl[_DEF_UART1].hw.h_uart->RXD;

    index = uart_tbl[_DEF_UART1].hw.rx_buf.ptr_in;

    uart_tbl[_DEF_UART1].hw.rx_buf.p_buf[index] = ch;
    uart_tbl[_DEF_UART1].hw.rx_buf.ptr_in = (uart_tbl[_DEF_UART1].hw.rx_buf.ptr_in + 1) % uart_tbl[_DEF_UART1].hw.rx_buf.length;

    uart_tbl[_DEF_UART1].hw.h_uart->EVENTS_RXDRDY = 0x0UL;
  }
}




