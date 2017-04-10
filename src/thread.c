#include <errno.h>
#include <stdio.h>
#include "queue/o_queue.h"
#include "queue/o_list.h"
#include "thread.h"
#include "context.h"
#include <ucontext.h>
//push_back, pop, get

#define TO_TTHREAD(void_ptr) ((struct tthread_t*)void_ptr)

/*
* Récupère l'identifiant du thread courant.
*/
thread_t thread_self(void) {
    struct tthread_t *current = queue__first();
    return (thread_t) current;
}

/*
 * Créé un nouveau thread qui va exécuter la fonction func avec l'argument funcarg.
 * Renvoie 0 en cas de succès, -1 en cas d'erreur.
 */
int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg) {
    struct watchdog_args args;

    int res = getcontext(args._thread->_context);
    if (res == -1)
        perror("impossible get current context");

    int res = getcontext(&(args._thread->_context.uc_link));
    if (res == -1)
        perror("impossible get current context");

    args._thread->_context.uc_stack.ss_size = STACK_SIZE;
    args._thread->_context.uc_stack.ss_sp = malloc(STACK_SIZE);
    args._func = func;
    args._func_arg = funcargs;

    makecontext(&(args->_thread->_context), cxt_watchdog, args);

    // check if the main has been put in a thread
}

/*
 * Passe la main à un autre thread.
 */
int thread_yield(void) {
    struct tthread_t *actual = queue__pop();
    queue__push_back(actual);
    swapcontext(&TO_TTHREAD(queue__second())->_context, &TO_TTHREAD(queue__first())->_context);
}

/*
 * Attend la fin d'exécution d'un thread.
 * La valeur renvoyée par le thread est placée dans *retval.
 * Si retval est NULL, la valeur de retour est ignorée.
 */
int thread_join(thread_t thread, void **retval) {
    if (thread == NULL) { //doesn't exist --> error, invalid
        perror("Error : thread doesn't exist in thread_join");
        return 0;
    }

    struct tthread_t *tthread = TO_TTHREAD(thread);

    if (tthread->_state == ACTIVE) {
        tthread->_join_wait++; //increment the number of thread that wait the thread
        thread_yield(); //give the hand
    }


    int i = 0;
    while (i < tthread.nb_waiting) {
        if (tthread.waiting[i] == (&TO_TTHREAD(queue__first()))) {
            tthread.waiting[i] = tthread.waiting[nb_waiting - 1];
            tthread.nb_waiting--;
        }
        ++i;
    }

    if (tthread.nb_waiting == 0) {
        retval = tthread._retval;
    }
}

/*
 * Termine le thread courant en renvoyant la valeur de retour retval.
 * Cette fonction ne retourne jamais.
 */
void thread_exit(void *retval) __attribute__ ((__noreturn__)) {
    struct tthread_t *current = TO_TTHREAD(queue__first());
    current->_retval = &retval; //pass function's retval to calling thread
    struct node* current_node = current->_waiting_threads->head;
    while(hasNext(current_node)){
        ((struct tthread_t*)(current_node->data))->_state = ACTIVE;
    }

    exit(0);
}
