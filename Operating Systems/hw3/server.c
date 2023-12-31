#include "segel.h"
#include "request.h"
#include "Queue.h"

//
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//


pthread_mutex_t mutex;
pthread_cond_t cond_que_empty;
pthread_cond_t cond_que_full;

int waiting_size;
int threads_size;

pQueue requests;


void * workerThread(void *arg) {
    pThread_stats statistics = (pThread_stats) malloc(sizeof (Thread_stats));

    statistics->handler_thread_id = *(int*)arg;
    statistics->handler_thread_dynamic_req_count = 0;
    statistics->handler_thread_req_count = 0;
    statistics->handler_thread_static_req_count = 0;
    free(arg);

    int fd;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (waiting_size == 0) {
            pthread_cond_wait(&cond_que_empty, &mutex);
        }
        pNode curr = dequeue(requests);
        waiting_size--;
        threads_size++;
        pthread_cond_signal(&cond_que_full);
        pthread_mutex_unlock(&mutex);
        pReq_stats reqStats = (pReq_stats) malloc(sizeof (Req_stats));
        struct timeval curr_time ;
        gettimeofday(&curr_time,NULL);
        reqStats->arrival_time = curr->arrival_time;
        timersub(&curr_time,&reqStats->arrival_time,&reqStats->dispatch_time);
        reqStats->handler_thread_stats = statistics;
        fd = curr->fd;
        requestHandle(fd, reqStats);
        Close(fd);
        free(curr);
        free(reqStats);
        pthread_mutex_lock(&mutex);
        threads_size--;
        pthread_cond_signal(&cond_que_full);
        pthread_mutex_unlock(&mutex);
    }
}

static int getSizeBothQueues() {
    return waiting_size + threads_size;
}

void getargs(int *port, int argc, char *argv[])
{
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    *port = atoi(argv[1]);

}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    getargs(&port, argc, argv);
    int num_threds = atoi(argv[2]);
    int q_size = atoi(argv[3]);
    char overload_handling[10];
    strcpy(overload_handling, argv[4]);
    int max_size = -1;
    if (strcmp(overload_handling, "dynamic") == 0) {
        max_size = atoi(argv[5]);
    }

    pthread_cond_init(&cond_que_empty, NULL);
    pthread_cond_init(&cond_que_full, NULL);

    requests = initQueue();
    threads_size = 0;
    waiting_size = 0;

    pthread_t thread_arr[num_threds];
    for(int i = 0; i < num_threds; i++) {
        int* idx = (int*) malloc(sizeof (int));
        *idx = i;
        if(pthread_create(&thread_arr[i], NULL, workerThread, (void *)idx) != 0) {
            fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        }
    }


    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
        struct timeval arrival;
        gettimeofday(&arrival,NULL);

        if (getSizeBothQueues() >= q_size) {
            pthread_mutex_lock(&mutex);

            if(strcmp(overload_handling,"block")==0){
                while(getSizeBothQueues() >= q_size) {
                    pthread_cond_wait(&cond_que_full,&mutex);
                }
                enqueue(requests, connfd, arrival);
                waiting_size++;
                pthread_cond_signal(&cond_que_empty);
                pthread_mutex_unlock(&mutex);

            }

            if(strcmp(overload_handling,"dt")==0){
                if(getSizeBothQueues() >= q_size){
                    Close(connfd);
                    pthread_mutex_unlock(&mutex);
                } else {
                    enqueue(requests, connfd, arrival);
                    waiting_size++;
                    pthread_cond_signal(&cond_que_empty);
                    pthread_mutex_unlock(&mutex);
                }
            }

            if(strcmp(overload_handling, "dh") == 0) {
                if(getSizeBothQueues() >= q_size && waiting_size > 0){
                    pNode temp = dequeue(requests);
                    if(temp != NULL) {
                        close(temp->fd);
                        waiting_size--;
                    }
                    free(temp);
                }
                enqueue(requests, connfd, arrival);
                waiting_size++;
                pthread_cond_signal(&cond_que_empty);
                pthread_mutex_unlock(&mutex);
            }
            if(strcmp(overload_handling,"bf")==0){
                while(getSizeBothQueues() > 0) {
                    pthread_cond_wait(&cond_que_full,&mutex);
                }
                close(connfd);
                pthread_mutex_unlock(&mutex);
            }
            if(strcmp(overload_handling,"dynamic")==0){
                if(q_size < max_size) {
                    q_size++;
                }
                Close(connfd);
                pthread_mutex_unlock(&mutex);
            }
            if(strcmp(overload_handling,"random")==0) {
                int half;
                if(waiting_size % 2 != 0){
                    half = (waiting_size + 1) / 2;
                } else {
                    half = waiting_size / 2;
                }
                int idx;
                for(int i=  0 ; i < half ; i++){
                    idx = rand() % (waiting_size - 1);
                    pNode temp = dequeueByIDX(requests, idx);
                    close(temp->fd);
                    free(temp);
                    waiting_size--;
                }
                enqueue(requests, connfd, arrival);
                waiting_size++;
                pthread_cond_signal(&cond_que_empty);
                pthread_mutex_unlock(&mutex);
            }
        } else {
            pthread_mutex_lock(&mutex);
            enqueue(requests, connfd, arrival);
            waiting_size++;
            pthread_cond_signal(&cond_que_empty);
            pthread_mutex_unlock(&mutex);
        }
    }

}





