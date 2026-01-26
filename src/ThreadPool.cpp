#include "ThreadPool.h"
#include <iostream>
#include <stdexcept>

ThreadPool::ThreadPool(int size) : stop(false) {
    for(int i = 0; i < size; i++) {
        threads.emplace_back(std::thread([this]() {
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasksMtx);
                    cond.wait(lock, [this]() {
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) {
                        return;
                    }
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasksMtx);
        stop = true;
    }
    cond.notify_all();
    for(auto& thread : threads) {
        if(thread.joinable()) {
            thread.join();
        }
    }
}
