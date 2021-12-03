//==============================================================================
#ifndef _USART1_CONTROL_H_
#define _USART1_CONTROL_H_
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include <xTx.h>
#include "xType.h"
#include "xRx.h"
//==============================================================================
typedef struct{
  OBJECT_ATTACHMENT(xObject);
  
  xRxT Rx;
  xTxT Tx;
  
  volatile UsartReg_T *Reg;
}Usart1_T;
//==============================================================================
extern Usart1_T Usart1;
//==============================================================================
extern inline void Usart1_Handler();
extern inline void Usart1_TransmitionHandler();
extern inline void Usart1_IRQ();
extern inline void Usart1_Init();
//==============================================================================
#endif
