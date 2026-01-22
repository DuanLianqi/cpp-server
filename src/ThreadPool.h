#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(int size = 10);
    ~ThreadPool();

    void add(std::function<void()> task);

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasksMtx;
    std::condition_variable cond;
    bool stop;
};

#endif
