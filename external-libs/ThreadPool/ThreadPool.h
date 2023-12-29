//
// Created by tombe on 29/12/2023.
//

#ifndef SECTRANS_THREADPOOL_H
#define SECTRANS_THREADPOOL_H


#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

class ThreadPool {
public:
    void start(uint32_t n);
    void queueJob(const std::function<void(int)>& job, int param);
    void stop();
    bool busy();
private:
    void threadLoop();

    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
    std::vector<std::thread> threads;
    std::queue<std::function<void(int)>> jobs;
    std::queue<int> jobsParam;
};


#endif //SECTRANS_THREADPOOL_H
