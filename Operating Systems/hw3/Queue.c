//
// Created by chens on 05/06/2023.
//

#include "Queue.h"
#include "segel.h"
#include "sys/time.h"

pQueue initQueue() {
    pQueue ret = (pQueue) malloc(sizeof(Queue));
    if (ret != NULL) {
        ret->size = 0;
        ret->front = NULL;
        ret->back = NULL;
    }
    return ret;
}

void enqueue(pQueue queue, int data, struct timeval arrival) {
    pNode newNode = (pNode)malloc(sizeof(Node));
    newNode->fd = data;
    newNode->next = NULL;
    newNode->arrival_time = arrival;


    if (queue->back == NULL) {
        queue->front = newNode;
        queue->back = newNode;
    } else {
        queue->back->next = newNode;
        queue->back = newNode;
    }
    queue->size++;
}

pNode dequeue(pQueue que) {
    if (que == NULL) {
        return NULL;
    }
    if (que->front == NULL) {
        return NULL;
    }
    if (que->front == que->back) {
        que->back = NULL;
        pNode temp = que->front;
        que->size = 0;
        que->front = NULL;
        return temp;
    }
    pNode temp = que->front;
    que->front = temp->next;
    temp->next= NULL;
    que->size--;
    return temp;
}

pNode dequeueByVal(pQueue que, int fd) {
    if (que == NULL) {
        return NULL;
    }
    if (que->front == NULL) {
        return NULL;
    }
    pNode it = que->front;
    int idx = 0;

    while (it != NULL) {
        if (it->fd == fd) {
            break;
        }
        it = it->next;
        idx++;
    }

    return dequeueByIDX(que, idx);

}

pNode dequeueByIDX(pQueue queue, int idx) {
    if (idx < 0 || idx >= queue->size) {
        return NULL;
    }

    if(idx == 0) {
        return dequeue(queue);
    }

    Node* currentNode = queue->front;
    Node* previousNode = NULL;

    // Traverse the linked list to find the node at the given index
    for (int i = 0; i < idx; i++) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }
    pNode temp = currentNode;
    // Update the pointers to remove the node
    if (previousNode == NULL) {
        // The node to remove is the first node
        queue->front = currentNode->next;
    } else {
        // The node to remove is in the middle or at the end
        previousNode->next = currentNode->next;

        if (currentNode == queue->back) {
            // The node to remove is the last node
            queue->back = previousNode;
        }
    }

    queue->size--;
    return temp;
}