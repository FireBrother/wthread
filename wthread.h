#ifndef _WTHREAD_H_
#define _WTHREAD_H_

#define _XOPEN_SOURCE 600

// 头文件
#include <ucontext.h>
#include <vector>
using namespace std;

// 参数设置
#define STACK_SIZE 1024*128

// 类型定义
typedef void (*funct_t)(void*);
typedef uint32_t tid_t;
enum state_t {READY, RUNNING, BLOCKED, STOPPED};

struct wthread_t {
    ucontext_t uc;
    funct_t func;
    void *arg;
    state_t state;
    char stack[STACK_SIZE];
};

// 变量声明
extern vector<wthread_t> threads;
extern tid_t tid_running_thread;
extern ucontext_t uc_main;

// 接口定义
tid_t wthread_create(wthread_t *ptid, funct_t func, void *arg);
void wthread_yield();
void wthread_join(tid_t tid);


#endif
