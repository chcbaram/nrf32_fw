/*
 *  ble_mouse.h
 *
 *  Created on: 2017. 10. 20.
 *      Author: Baram
 */

#ifndef BLE_MOUSE_H
#define BLE_MOUSE_H


#ifdef __cplusplus
 extern "C" {
#endif



#include "def.h"
#include "bsp.h"








bool bleMouseInit(void);
bool bleMouseIsConnected(void);



#ifdef __cplusplus
}
#endif


#endif
