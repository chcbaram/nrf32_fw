/*
 *  ble_uart.h
 *
 *  Created on: 2017. 10. 20.
 *      Author: Baram
 */

#ifndef BLE_UART_H
#define BLE_UART_H


#ifdef __cplusplus
 extern "C" {
#endif



#include "def.h"
#include "bsp.h"








bool bleUartInit(void);
bool bleUartIsConnected(void);

uint32_t bleUartAvailable(void);
uint8_t  bleUartRead(void);
int32_t  bleUartWrite(uint8_t *p_data, uint32_t length);


#ifdef __cplusplus
}
#endif


#endif
