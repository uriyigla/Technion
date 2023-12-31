//
// Created by chens on 05/06/2023.
//

#ifndef OS3_QUEUE_H
#define OS3_QUEUE_H

#include <sys/time.h>

typedef struct node {
    int fd;
    struct timeval arrival_time;
    struct node* next;
} Node, *pNode;

typedef struct Queue {
    int size;
    pNode front;
    pNode back;
} Queue, *pQueue;

pQueue initQueue();

int getSizeQueue(pQueue que);

void enqueue(pQueue que, int fd, struct timeval arrival);

pNode dequeue(pQueue que); // maybe change to int so the memory manegment will be internal

pNode dequeueByVal(pQueue que, int fd);

pNode dequeueByIDX(pQueue que, int idx);

#endif //OS3_QUEUE_H
