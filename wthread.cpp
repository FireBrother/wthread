#include "wthread.h"
// 消除clang对于ucontext的函数的警告
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

// 全局变量
vector<wthread_t> threads;
tid_t tid_running_thread;
ucontext_t uc_main;
queue<tid_t> waiting_tids;

tid_t wthread_create(func_t func, void *arg) {
    int tid = -1;
    // 查找是否有空闲的线程
    for (int i = 0; i < threads.size(); i++)
        if (threads[i].state == STOPPED) {
            tid = i;
            break;
        }
    // 如果没找到，需要添加一个线程
    if (tid == -1) {
        wthread_t thread;
        threads.push_back(thread);
        tid = threads.size() - 1;
    }

    LogDebug("Thread %d created.", tid);
    threads[tid].func = func;
    threads[tid].arg = arg;
    threads[tid].state = READY;

    return tid;
}

void wthread_yield() {
    tid_t tid = tid_running_thread;
    if (tid == -1)
        return;
    LogDebug("Thread %d yielded.", tid);
    threads[tid].state = RUNNABLE;
    tid_running_thread = -1;
    waiting_tids.push(tid);
    if (!waiting_tids.empty()) {
        tid_t tid_cond = waiting_tids.front();
        waiting_tids.pop();
        swapcontext(&(threads[tid].uc), &(threads[tid_cond].uc));
    }
    else
        swapcontext(&(threads[tid].uc), &uc_main);
}

void wthread_entry() {
    tid_t tid = tid_running_thread;
    if (tid == -1)
        return;

    LogDebug("Thread %d started.", tid);
    threads[tid].func(threads[tid].arg);
    LogDebug("Thread %d finished.", tid);
    threads[tid].state = STOPPED;
    if (!waiting_tids.empty()) {
        tid_t tid_cond = waiting_tids.front();
        waiting_tids.pop();
        swapcontext(&(threads[tid].uc), &(threads[tid_cond].uc));
    }
    else
        tid_running_thread = -1;
}

void wthread_wakeup(tid_t tid) {
    if(tid >= threads.size()) {
        return;
    }

    LogDebug("Thread %d woke up.", tid);
    switch(threads[tid].state){
        case READY:
            getcontext(&(threads[tid].uc));

            threads[tid].uc.uc_stack.ss_sp = threads[tid].stack;
            threads[tid].uc.uc_stack.ss_size = STACK_SIZE;
            threads[tid].uc.uc_stack.ss_flags = 0;
            threads[tid].uc.uc_link = &(uc_main);
            threads[tid].state = RUNNING;

            tid_running_thread = tid;

            makecontext(&(threads[tid].uc), (void (*)(void))(wthread_entry), 0);

        case RUNNABLE:
            swapcontext(&(uc_main),&(threads[tid].uc));
            break;
        default: ;
    }
}

void wthread_join(tid_t tid) {
    if(tid >= threads.size()) {
        return;
    }

    LogDebug("Joining thread %d.", tid);
    while (threads[tid].state != STOPPED)
        ;
}

void wthread_exit() {
    tid_t tid = tid_running_thread;
    if (tid == -1)
        return;

    threads[tid].state = STOPPED;
}
