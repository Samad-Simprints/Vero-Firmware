#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "FreeRTOS.h"
#include "btfifo.h"


/**
  * @brief  BT_FIFO_Create create fifo
  * @param  fifo_size - fifo size
  * @retval pointer on fifo
  */
BT_FIFO_PTR BT_FIFO_Create(uint16_t fifo_size)
{
  BT_FIFO_PTR fifo = (BT_FIFO_PTR)pvPortMalloc(sizeof(BT_FIFO)+fifo_size);
  if(!fifo){
    return NULL;
  }
  memset(fifo,0,(sizeof(BT_FIFO)+fifo_size));
  fifo->buf_size = fifo_size;
  fifo->buffer = (uint8_t*)fifo + sizeof(BT_FIFO);
  fifo->status = BT_FIFO_EMPTY;
  return fifo;
}
/**
  * @brief  BT_FIFO_GetStatus get status
  * @param  fifo - pointer on fifo
  * @retval fifo status is follow:
  *     @arg BT_FIFO_EMPTY
  *     @arg BT_FIFO_FULL
  *     @arg BT_FIFO_FILLED
  */
BT_FIFO_STATUS BT_FIFO_GetStatus(BT_FIFO_PTR fifo)
{
  if(!fifo) return BT_FIFO_ERROR;
  return fifo->status;
}
/**
  * @brief  BT_FIFO_GetUsed get fifo used space
  * @param  fifo - pointer on fifo
  * @retval number of used bytes in fifo
  */
uint16_t BT_FIFO_GetUsed(BT_FIFO_PTR fifo)
{
  if(!fifo) return 0;
  int buf_size = fifo->buf_size;
  if(fifo->status == BT_FIFO_FULL)
    return buf_size;
  int num_bytes = fifo->wr_index - fifo->rd_index;
  if (num_bytes<0)
      num_bytes = buf_size+num_bytes;
  else
    if(num_bytes)
      fifo->status = BT_FIFO_FILLED;
  return num_bytes;
}
/**
  * @brief  BT_FIFO_GetFree get fifo free space
  * @param  fifo - pointer on fifo
  * @retval number of free bytes in fifo
  */
uint16_t BT_FIFO_GetFree(BT_FIFO_PTR fifo)
{
  if(!fifo) return 0;
  return (fifo->buf_size - BT_FIFO_GetUsed(fifo));
}
/**
  * @brief  BT_FIFO_Clear clear fifo
  * @param  fifo - pointer on fifo
  * @retval None
  */
void BT_FIFO_Clear(BT_FIFO_PTR fifo)
{
  if(fifo){
    fifo->rd_index = fifo->wr_index=0;
    fifo->status = BT_FIFO_EMPTY;
  }
}
/**
  * @brief  BT_FIFO_Pop extraction data from fifo
  * @param  fifo - pointer on fifo
  * @param  dst - pointer on destination buffer
  * @param  length - destination length
  * @retval number reading bytes
  */
uint16_t BT_FIFO_Pop(BT_FIFO_PTR fifo, void * dst, uint16_t length)
{
  if(!fifo) return 0;
  uint8_t * buffer = (uint8_t*)dst;
  int len = length;
  int used = BT_FIFO_GetUsed(fifo);
  if(buffer == NULL) return 0;
  if(!used){
    fifo->status = BT_FIFO_EMPTY;
    return 0;
  }
  fifo->status = BT_FIFO_FILLED;
  uint16_t rd_length=0;
  while(len--){
    *buffer++ = fifo->buffer[fifo->rd_index++];
    ++rd_length;
    if(fifo->rd_index == fifo->buf_size)
      fifo->rd_index =0;
        if(used > fifo->buf_size/2 && (used-1) == fifo->buf_size/2)
          fifo->status = BT_FIFO_HALF_FULL;
    if(--used == 0){
      fifo->status = BT_FIFO_EMPTY;
      break;
    }
  }
  return rd_length;
}
/**
  * @brief  BT_FIFO_Push Push data to fifo
  * @param  fifo - pointer on fifo
  * @param  src - pointer on source buffer
  * @param  length - source length
  * @retval number wrote of source buffer space
  */
uint16_t BT_FIFO_Push(BT_FIFO_PTR fifo, void * src, uint16_t length)
{
  if(!fifo) return 0;
  uint8_t * buffer = (uint8_t*)src;
  int len = length;
  int free = BT_FIFO_GetFree(fifo);
  if(buffer == NULL) return 0;
  if(!free){
    fifo->status = BT_FIFO_FULL;
    return 0;
  }
  fifo->status = BT_FIFO_FILLED;
  uint16_t wr_length=0;
  while(len--){
    fifo->buffer[fifo->wr_index++] = *buffer++;
    ++wr_length;
    if(fifo->wr_index == fifo->buf_size)
      fifo->wr_index =0;
        if(free > fifo->buf_size/2 && (free-1) == fifo->buf_size/2)
          fifo->status = BT_FIFO_HALF_FULL;
    if(--free == 0){
      fifo->status = BT_FIFO_FULL;
      break;
    }
  }
  return wr_length;
}
/**
  * @brief  BT_FIFO_Close Close fifo
  * @param  fifo - pointer on fifo
  * @retval None
  */
void BT_FIFO_Close(BT_FIFO_PTR fifo)
{
  if(fifo){
    vPortFree(fifo);
  }
}
