#include <stdio.h>
#include <unistd.h>
#include "wthread.h"

void func1(void * arg)
{
    puts("1");
    puts("11");
    puts("111");
    puts("1111");
}

void func2(void * arg)
{
    puts("22");
    puts("22");
    wthread_yield();
    puts("2222");
    puts("2222");
}

void func3(void *arg)
{
    puts("33");
    puts("33");
    wthread_yield();
    puts("3333");
    wthread_yield();
    puts("3333");
}

void schedule_test()
{
    int id1 = wthread_create(func3, NULL);
    int id2 = wthread_create(func2, NULL);

    wthread_wakeup(id1);
    wthread_wakeup(id2);
    puts("main over");

    wthread_join(id1);
    wthread_join(id2);
}


int main(int argc, const char *argv[]){
    schedule_test();
}
