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
        //std::cout << "走到这里" << std::endl;
        stop = false;
    }
    cond.notify_all();
    for(auto& thread : threads) {
        if(thread.joinable()) {
            thread.join();
            std::cout << "走到这里" << std::endl;
        }
    }
}

void ThreadPool::add(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(tasksMtx);
        if(stop) {
            throw std::runtime_error("ThreadPool already stop, can't add task any more!");
        }
        tasks.emplace(task);
    }
    cond.notify_all();
}
