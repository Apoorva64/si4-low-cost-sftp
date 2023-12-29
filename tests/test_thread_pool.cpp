//
// Created by tombe on 29/12/2023.
//
#define CONFIG_CATCH_MAIN
#include <catch2/catch_all.hpp>
#include "ThreadPool.h"

void simple_add(int b){
    int a = 9 + b;
    printf("%d\n", a);
}

TEST_CASE("Test Simple Function", "[thread_pool_simple]"){
    ThreadPool threadPool = ThreadPool();
    threadPool.start(5);
    threadPool.queueJob(simple_add,12);
    threadPool.queueJob(simple_add,19);
    threadPool.queueJob(simple_add,11);
    threadPool.queueJob(simple_add,10);
    while(threadPool.busy());
    threadPool.stop();
}