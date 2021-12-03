/*
 * CK_Responses.h
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */

#ifndef _BL_RESPONSES_H_
#define _BL_RESPONSES_H_
//==============================================================================
#include "xType.h"
#include "xTransaction.h"
//==============================================================================
enum TERMINAL_STM32F103_REQUESTS
{
  TERMINAL_STM32F103_REQUESTS_START = 0,
  
  TERMINAL_STM32F103_GET_REQUESTS_START = 100,
  TERMINAL_STM32F103_GET_INFO,
  TERMINAL_STM32F103_GET_HANDLER,  
  TERMINAL_STM32F103_GET_REQUESTS_END,
  
  TERMINAL_STM32F103_SET_REQUESTS_START = 200,
  TERMINAL_STM32F103_SET_OPTIONS,
  TERMINAL_STM32F103_SET_HANDLER,
  TERMINAL_STM32F103_SET_REQUESTS_END,
  
  TERMINAL_STM32F103_TRY_REQUESTS_START = 300,
  TERMINAL_STM32F103_TRY_ERASE,
  TERMINAL_STM32F103_TRY_WRITE,
  TERMINAL_STM32F103_TRY_READ,
  TERMINAL_STM32F103_TRY_STOP,
  TERMINAL_STM32F103_TRY_JUMP_TO_MAIN,
  TERMINAL_STM32F103_TRY_JUMP_TO_BOOT,
  TERMINAL_STM32F103_TRY_REQUESTS_END,
  
  TERMINAL_STM32F103_EVT_REQUESTS_START = 400,
  TERMINAL_STM32F103_EVT_WRITE_COMPLITE,
  TERMINAL_STM32F103_EVT_READ_COMPLITE,
  TERMINAL_STM32F103_EVT_REQUESTS_END,
  
  TERMINAL_STM32F103_REQUESTS_END = 500  
};
//==============================================================================
extern const xCommandT Commands[];
extern const xTransactionT Requests[];
//==============================================================================
int rx_endline(xObject context, uint8_t *obj, uint16_t size);
//==============================================================================
#endif /* _BL_RESPONSES_H_ */
