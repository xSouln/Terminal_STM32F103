//==============================================================================
#include <string.h>
#include "main.h"
#include "usart.h"
#include "xRx.h"
#include "xTx.h"
#include "Usart1.h"
#include "Responses.h"
//==============================================================================
#define USART1_RX_CIRCLE_BUF_SIZE_MASK 0x1ff
#define USART1_RX_OBJECT_BUF_SIZE 0x1ff
#define USART1_TX_CIRCLE_BUF_SIZE_MASK 0x3ff
//==============================================================================
TX_BUF_INIT(USART1);
RX_BUF_INIT(USART1);
//==============================================================================
//==============================================================================
inline void Usart1_Handler()
{
  xRxUpdate(&Usart1, &Usart1.Rx);
}
//==============================================================================
inline void Usart1_Init()
{
  Usart1.Rx.Tx = &Usart1.Tx;
  Usart1.Tx.Rx = &Usart1.Rx;
  
  Usart1.Rx.State.EndLineControl = true;

  Usart1.Reg->CR1.ReceiverEnable = true;
  Usart1.Reg->CR1.RxNotEmptyInterruptEnable = true;

  //Ports.B.Out->USART1_DE = false;
  Usart1.Reg->CR1.TransmitterEnable = true;
  Usart1.Reg->CR1.TxCompleteInterruptEnable = false;
  Usart1.Reg->CR1.TxEmptyInterruptEnable = false;
}
//==============================================================================
bool usart1_transmit_action(xObject context, uint8_t* ptr, uint16_t size)
{
  //Ports.B.Out->USART1_DE = true;
  //UsartX.Tx.Packet = *packet;
  Usart1.Reg->CR1.TxEmptyInterruptEnable = true;
  return true;
}
//==============================================================================
inline void Usart1_TransmitionHandler()
{
  Usart1.Reg->CR1.TxEmptyInterruptEnable = Usart1.Tx.State.HandlerIndex != Usart1.Tx.State.TotalIndex;
}
//==============================================================================
inline void Usart1_IRQ()
{
  if(Usart1.Reg->SR.RxNotEmpty)
  {
    Usart1.Reg->SR.RxNotEmpty = false;
    xRxCircleAdd(Usart1.Rx, Usart1.Reg->DR);
    return;
  }

  if(Usart1.Reg->CR1.TxEmptyInterruptEnable && Usart1.Reg->SR.TxEmpty)
  {
    if(Usart1.Tx.State.HandlerIndex != Usart1.Tx.State.TotalIndex){ xTxSend(Usart1.Tx, Usart1.Reg->DR); }
    else { Usart1.Reg->CR1.TxEmptyInterruptEnable = false; }
  }
}
//==============================================================================
Usart1_T Usart1 =
{
  .Rx =
  {
    RX_OBJECT_RECEIVER_INIT(USART1, rx_endline),
    RX_CIRCLE_RECEIVER_INIT(USART1)
  },

  .Tx =
  {
    TX_BINDING(USART1, usart1_transmit_action)
  },

  .Reg = (UsartReg_T*)USART1
};
//==============================================================================
