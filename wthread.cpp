#include "wthread.h"
vector<wthread_t> threads;
tid_t tid_running_thread;
ucontext_t uc_main;

tid_t wthread_create(wthread_t *ptid, funct_t func, void *arg) {
    return 0;
}
