/*
 *  ble.h
 *
 *  Created on: 2017. 10. 20.
 *      Author: Baram
 */

#ifndef BLE_H
#define BLE_H


#ifdef __cplusplus
 extern "C" {
#endif



#include "def.h"
#include "bsp.h"








bool bleInit(void);
bool bleIsConnected(void);

#ifdef __cplusplus
}
#endif


#endif
