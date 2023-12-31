#ifndef __REQUEST_H__


typedef struct thread_stats{
    int handler_thread_id;
    int handler_thread_req_count;
    int handler_thread_static_req_count;
    int handler_thread_dynamic_req_count;
} Thread_stats, *pThread_stats;

typedef struct req_stats {
    struct timeval arrival_time;
    struct timeval dispatch_time;
    pThread_stats handler_thread_stats;
} Req_stats, *pReq_stats;



void requestHandle(int fd, pReq_stats stats);

#endif
