#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>

//pthread_create()// create a new thread: on success returns 0, on error returns an error number.
//pthread_detach()// detach a thread: on success returns 0, on error returns an error number.
//pthread_join()// join with a terminated thread: on success returns 0, on error returns an error number.
//pthread_mutex_init()// destroy and initialize a mutex: on success returns 0, on error returns an error number.
//pthread_mutex_destroy() // destroy and initialize a mutex: on success returns 0, on error returns an error number.
//pthread_mutex_lock()// lock and unlock a mutex: on success returns 0, on error returns an error number.
//pthread_mutex_unlock()// unlock and lock a mutex: on success returns 0, on error returns an error number.
//gettimeofday()// get / set time: return 0 for success, or -1 for failure.

typedef struct s_table{
    int forks;
} t_table;

typedef struct s_philo{
    pthread_t th; // thread
    struct s_philo  *next;
} t_philo;

#endif