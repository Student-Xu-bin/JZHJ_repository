#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#include <stdint.h>

typedef struct {
    uint32_t    head;
    uint32_t    tail;
    uint32_t    size;
    uint8_t     *buffer;
}queue_t;

typedef enum {
    QUEUE_OK = 0,
    QUEUE_ERROR,
    QUEUE_OVERLOAD,
    QUEUE_EMPTY
}queue_status_t;

void queue_init(queue_t *queue, uint8_t *buffer, uint32_t size);
queue_status_t queue_push(queue_t *queue, uint8_t data);
queue_status_t queue_pop(queue_t *queue, uint8_t *pdata);
uint32_t queue_push_array(queue_t *queue, uint8_t *parray, uint32_t len);
uint32_t queue_pop_array(queue_t *queue, uint8_t *parray, uint32_t len);
uint32_t queue_count(queue_t *queue);
queue_status_t queue_is_full(queue_t *queue);
queue_status_t queue_is_empty(queue_t *queue);

#endif
