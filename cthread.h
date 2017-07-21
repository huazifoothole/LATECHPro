#ifndef CTHREAD_H
#define CTHREAD_H
#include <pthread.h>
extern pthread_mutex_t printerMutex;// = PTHREAD_MUTEX_INITIALIZER;
extern pthread_mutex_t scannerMutex;// = PTHREAD_MUTEX_INITIALIZER;
extern pthread_mutex_t HscannerMutex;// = PTHREAD_MUTEX_INITIALIZER;
#endif // CTHREAD_H
