/*
 * RingBuffer.h
 *
 *  Created on: Oct 21, 2020
 *      Author: duanlc
 */

#ifndef GATEWAYMANAGER_RINGBUFFER_H_
#define GATEWAYMANAGER_RINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

#define RINGBUFFER_LEN 		(64)
#define GET_TICK		HAL_GetTick()


extern volatile unsigned char ringbuffere[RINGBUFFER_LEN];

typedef struct ringbuffer_t
{
	void *buffer;   // data buffer
	void *buffer_end; // end of data buffer
	size_t capacity; // maximum number of items in the buffer
	size_t count;   // number of items in the buffer
	size_t sz;    // size of each item in the buffer
	void *head;    // pointer to head
	void *tail;    // pointer to tail
	unsigned long ring_push_time_out;
	unsigned char ring_push_flag;
} ringbuffer_t;


typedef struct{
	unsigned char message_flag;
	unsigned long message_time_out;
}modbusMessageCheck_t;
extern ringbuffer_t 		vrts_ringbuffer_Data;
bool ring_init(ringbuffer_t *cb, size_t capacity, size_t sz);

bool ring_free(ringbuffer_t *cb);

bool ring_push_head(ringbuffer_t *cb, const void *item);

bool ring_pop_tail(ringbuffer_t *cb, void *item);

#ifdef __cplusplus
}
#endif



#endif /* GATEWAYMANAGER_RINGBUFFER_H_ */
