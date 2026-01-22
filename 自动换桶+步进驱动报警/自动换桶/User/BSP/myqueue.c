#include "myqueue.h"

void queue_init(queue_t *queue, uint8_t *buffer, uint32_t size)
{
    queue->buffer = buffer;
    queue->size = size;
    queue->head = queue->tail = 0;
}

queue_status_t queue_push(queue_t *queue, uint8_t data)
{
    uint32_t index = (queue->tail + 1) % queue->size;

    if(index == queue->head)
        return QUEUE_OVERLOAD;

    queue->buffer[queue->tail] = data;
    queue->tail = index;

    return QUEUE_OK;
}

queue_status_t queue_pop(queue_t *queue, uint8_t *pdata)
{
    if(queue->head == queue->tail)
        return QUEUE_EMPTY;

    *pdata = queue->buffer[queue->head];
    queue->head = (queue->head + 1) % queue->size;

    return QUEUE_OK;
}

uint32_t queue_push_array(queue_t *queue, uint8_t *parray, uint32_t len)
{
    uint32_t i = 0;

    for(; i < len; i ++)
    {
        if(queue_push(queue, parray[i]) == QUEUE_OVERLOAD)
            break;
    }

    return i;
}

uint32_t queue_pop_array(queue_t *queue, uint8_t *parray, uint32_t len)
{
    uint32_t i = 0;

    for(; i < len; i ++)
    {
        if(queue_pop(queue, &parray[i]) == QUEUE_EMPTY)
            break;
    }

    return i;
}

uint32_t queue_count(queue_t *queue)
{
    if(queue->head <= queue->tail)
        return queue->tail - queue->head;

    return queue->size + queue->tail - queue->head;
}

queue_status_t queue_is_empty(queue_t *queue)
{
    if(queue->head == queue->tail)
        return QUEUE_EMPTY;

    return QUEUE_ERROR;
}

queue_status_t queue_is_full(queue_t *queue)
{
    if(((queue->tail + 1) % queue->size) == queue->head)
        return QUEUE_OVERLOAD;

    return QUEUE_ERROR;
}
