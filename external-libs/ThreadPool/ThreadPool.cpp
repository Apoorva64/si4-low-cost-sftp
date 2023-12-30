//
// Created by tombe on 29/12/2023.
//


#include <stdexcept>
#include "ThreadPool.h"
#include "spdlog/spdlog.h"

void ThreadPool::start(uint32_t n) {
    const uint32_t num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
    if(n > num_threads){
        throw std::runtime_error(fmt::format("Out of thread ! (Max {})", num_threads));
    }
    for (uint32_t ii = 0; ii < n; ++ii) {
        threads.emplace_back(&ThreadPool::threadLoop,this);
    }
}

void ThreadPool::threadLoop() {
    while (true) {
        int param;
        std::function<void(int)> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
            });
            if (should_terminate) {
                return;
            }
            job = jobs.front();
            jobs.pop();
            param = jobsParam.front();
            jobsParam.pop();
        }
        job(param);
    }
}

void ThreadPool::queueJob(const std::function<void(int)>& job, int param) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
        jobsParam.push(param);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::busy() {
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = !jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}