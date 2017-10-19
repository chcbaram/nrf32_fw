/*
 * bsp.h
 *
 *  Created on: 2017. 3. 10.
 *      Author: Baram
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include <stdio.h>
#include <stdbool.h>

#include "nrf.h"
#include "nrf52.h"
#include "nrf_drv_systick.h"



#ifdef __cplusplus
 extern "C" {
#endif



void bspInit(void);
void bspDeinit();


#ifdef __cplusplus
 }
#endif

#endif /* SRC_BSP_BSP_H_ */
