/*
 * Bootloader.c
 *
 *  Created on: Dec 17, 2020
 *      Author: rekuts
 */
//=================================================================================================================================
#include <string.h>
#include <stdlib.h>
#include "Bootloader.h"
#include "xRequest.h"
//=================================================================================================================================
uint16_t ActionWrite(xObject context, xObject object, uint16_t object_size)
{
  
  
  return ACCEPT;
}

BootloaderT Bootloader = { 0
};
//=================================================================================================================================