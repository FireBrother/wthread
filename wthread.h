#ifndef _WTHREAD_H_
#define _WTHREAD_H_

#define _XOPEN_SOURCE 600

// 头文件
#include <ucontext.h>
#include <vector>
#include <queue>
#include "Logger.hpp"
using namespace std;

// 参数设置
#define STACK_SIZE 1024*128

// 类型定义
typedef void (*func_t)(void*);
typedef uint32_t tid_t;
enum state_t {READY, RUNNABLE, RUNNING, BLOCKED, STOPPED};

struct wthread_t {
    ucontext_t uc;
    func_t func;
    void *arg;
    state_t state;
    char stack[STACK_SIZE];
};

// 外部变量
extern vector<wthread_t> threads;
extern tid_t tid_running_thread;
extern ucontext_t uc_main;
extern queue<tid_t> waiting_tids;

// 接口定义
tid_t wthread_create(func_t func, void *arg);
void wthread_yield();
void wthread_join(tid_t tid);
void wthread_entry();
void wthread_wakeup(tid_t tid);
void wthread_join(tid_t tid);
void wthread_exit();

#endif
