/*
 *  hw.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef HW_H
#define HW_H


#ifdef __cplusplus
 extern "C" {
#endif


#include "def.h"
#include "bsp.h"

#include "cmdif.h"



#include "core/delay.h"
#include "core/led.h"
#include "core/uart.h"
#include "core/hwtimer.h"
#include "core/swtimer.h"
#include "core/button.h"
#include "core/flash.h"
//#include "core/spi.h"
//#include "lcd/lcd.h"
//#include "core/dac.h"
#include "nrf52/ble.h"

void hwInit(void);



#ifdef __cplusplus
}
#endif


#endif
