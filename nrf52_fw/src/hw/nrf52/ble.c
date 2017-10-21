/*
 *  ble.c
 *
 *  Created on: 2017. 10. 20.
 *      Author: Baram
 */
#include <ble_uart.h>
#include "ble.h"
#include "hw.h"






int bleCmdif(int argc, char **argv);




bool bleInit(void)
{
  bleUartInit();


  cmdifAdd("ble", bleCmdif);
  return true;
}

bool bleIsConnected(void)
{
  return bleUartIsConnected();
}




//-- bleCmdif
//
int bleCmdif(int argc, char **argv)
{
  bool ret = true;


  if(strcmp("status", argv[1]) == 0 && argc == 2)
  {
    while(cmdifRxAvailable() == 0)
    {
      cmdifPrintf("Ble Connect : %d\n", bleIsConnected());
      delay(200);
    }
  }
  else if(strcmp("uart", argv[1]) == 0 && argc == 2)
  {
    uint8_t ch;
    while(1)
    {
      if (cmdifRxAvailable())
      {
        ch = cmdifRxRead();
        if (ch == 0x1B)
        {
          break;
        }

        bleUartWrite(&ch, 1);
      }

      if (bleUartAvailable())
      {
        uint8_t ch;

        ch = bleUartRead();

        cmdifPrintf("BleUart Rx : %c 0x%02X(%d)\n", ch, ch, ch);
      }
    }
    cmdifPrintf("0x%x\n", cmdifRxRead());
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cmdifPrintf( "ble status \n");
  }

  return 0;
}
