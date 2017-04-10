#ifndef FRED_CIE_CONTEXT_H
#define FRED_CIE_CONTEXT_H

#include <ucontext.h>
#include <signal.h>
#include <valgrind/valgrind.h>

#include "queue/o_list.h"

enum TTHREAD_STATE{
    ACTIVE, SLEEPING
};

#define STACK_SIZE 64 * 1024


struct tthread_t{
    ucontext_t _context;
    void** _retval;

    enum TTHREAD_STATE _state;
    struct list* _waiting_threads;
    int _waiting_thread_nbr;

    //For memory purposes
    int _valgrind_stackid;
};


struct watchdog_args {
  struct tthread_t * _thread;
  //tthread_t * _calling;
  void * (*_func)(void *);
  void * _func_arg;
};


struct tthread_t* tthread_init();
void tthread_destroy(struct tthread_t * tthread);
int cxt_watchdog(struct watchdog_args * args);

#endif //FRED_CIE_CONTEXT_H
