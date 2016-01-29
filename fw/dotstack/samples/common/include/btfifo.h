#ifndef BTFIFO_H
#define BTFIFO_H

typedef enum{
  BT_FIFO_EMPTY=0,
  BT_FIFO_FULL,
  BT_FIFO_HALF_FULL,
  BT_FIFO_FILLED,
  BT_FIFO_ERROR
}BT_FIFO_STATUS;

typedef struct _BT_FIFO{
  uint8_t * buffer;
  uint16_t  buf_size;
  uint16_t  rd_index;
  uint16_t  wr_index;
  BT_FIFO_STATUS status;
}BT_FIFO, *BT_FIFO_PTR;

BT_FIFO_PTR 	BT_FIFO_Create(uint16_t fifo_size);
BT_FIFO_STATUS 	BT_FIFO_GetStatus(BT_FIFO_PTR fifo);
uint16_t 		BT_FIFO_GetUsed(BT_FIFO_PTR fifo);
uint16_t 		BT_FIFO_GetFree(BT_FIFO_PTR fifo);
void 			BT_FIFO_Clear(BT_FIFO_PTR fifo);
uint16_t 		BT_FIFO_Pop(BT_FIFO_PTR fifo, void * dst, uint16_t length);
uint16_t 		BT_FIFO_Push(BT_FIFO_PTR fifo, void * src, uint16_t length);
void 			BT_FIFO_Close(BT_FIFO_PTR fifo);

#endif
