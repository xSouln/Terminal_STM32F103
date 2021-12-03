/*
 * Responses.c
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */
//==============================================================================
#include "Responses.h"
#include "xTx.h"
#include "Usart1.h"
//==============================================================================
EVENT_PATTERN(Response,
               xObject* Parent,
               xRxT* Rx,
               xTransactionT* Transaction);
               
EVENT_PATTERN(Action,
               xObject* Parent,
               xRxT* Rx,
               RequestT* Header);

EVENT_PATTERN(Command,
               xObject* Parent,
               xRxT* Rx,
               xCommandT* Command);
//==============================================================================
void Response_REQUEST_GET(xEventResponseT* Event, xObject obj, uint16_t obj_size, int16_t error)
{
  ResponseInfoT info =
  {
    .Key = Event->Transaction->Id,
    .Size = Event->Transaction->Content.size
  };
  
  xTxAdd(Event->Rx->Tx, &info, sizeof(info));
  xTxAdd(Event->Rx->Tx, Event->Transaction->Content.obj, Event->Transaction->Content.size);
}
//==============================================================================
void Response_REQUEST_DEFAULT(xEventResponseT* Event, xObject obj, uint16_t obj_size, int16_t error)
{
  ResponseInfoT info =
  {
    .Key = Event->Transaction->Id,
    .Size = Event->Transaction->Content.size + sizeof(error)
  };
  
  xTxAdd(Event->Rx->Tx, &info, sizeof(info));
  xTxAdd(Event->Rx->Tx, &error, sizeof(error));
  xTxAdd(Event->Rx->Tx, Event->Transaction->Content.obj, Event->Transaction->Content.size);
}
//==============================================================================
void request_action(xEventActionT* Event, uint8_t* object, uint16_t size)
{
  int16_t action_error = ACCEPT;
  uint8_t i = 0;
  
  while(Requests[i].Id != (uint16_t)-1)
  {
    if(((RequestInfoT*)object)->Key == Requests[i].Id)
    {
      ((xEventBaseT*)Event)->Context = (xObject)&Requests[i];
      object += sizeof(RequestInfoT);
      size -= sizeof(RequestInfoT);
      
      if(Requests[i].Action)
      {
        action_error = Requests[i].Action(Event, object, size);
      }
      
      if(Requests[i].Response)
      {
        xTxAdd(Event->Rx->Tx, RESPONSE_HEADER, sizeof_str(RESPONSE_HEADER));
        Requests[i].Response(Event, object, size, action_error);
        xTxAdd(Event->Rx->Tx, RESPONSE_END, sizeof_str(RESPONSE_END));
      }
      
      break;
    }
    i++;
  }
}
//==============================================================================
void request_qwerty0(xEventCommandT* Event, xObject object, uint16_t size)
{
  xTxAdd(Event->Rx->Tx, "request: ", sizeof_str("request: "));
  xTxAdd(Event->Rx->Tx, Event->Command->Header, Event->Command->HeaderLength);
  
  xTxAdd(Event->Rx->Tx, " accept: ", sizeof_str(" accept: "));
  xTxAdd(Event->Rx->Tx, object, size);
  
  xTxAdd(Event->Rx->Tx, RESPONSE_END, sizeof_str(RESPONSE_END));
}
//==============================================================================
int rx_endline(xObject context, uint8_t *obj, uint16_t size)
{ 
  if(size >= sizeof(RequestHeaderT) && obj[0] == REQUEST_START_CHARACTER && obj[5] == RESPONSE_END_CHARACTER)
  {
    if(size < sizeof(RequestT)) { return RX_STORAGE; }
    RequestT *request = (RequestT*)obj;
    uint16_t action_size = size - sizeof(RequestT);

    if(action_size < request->Info.Size) { return RX_STORAGE; }
    if(action_size > request->Info.Size) { return RX_RESET; }
  }
  
  xCommandIdentify(context, (xCommandT*)Commands, obj, size);
  
  return RX_RESET;
}
//==============================================================================
const xCommandT Commands[] =
{
  NEW_COMMAND1("qwerty0", request_qwerty0),
  NEW_COMMAND1("qwerty1", request_qwerty0),
  NEW_COMMAND1("qwerty2", request_qwerty0),
  { 0 }
};

const xTransactionT Requests[] =
{
  //NEW_REQUEST(GET_INFO, Response_REQUEST_GET, 0, Bootloader.Info),
  { .Id = -1 }
};
//==============================================================================
