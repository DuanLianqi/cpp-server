#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>

class ThreadPool {
public:
    ThreadPool(int size = 10);
    ~ThreadPool();

    template<class Func, class... Args>
    auto add(Func&& func, Args... args) -> std::future<typename std::result_of<Func(Args...)>::type> {
        using returnType = typename std::result_of<Func(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<returnType()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );

        std::future<returnType> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(tasksMtx);
            if(stop) {
                throw std::runtime_error("ThreadPool already stop, can't add task any more!");
            }
            tasks.emplace([task](){ (*task)(); });
        }
        cond.notify_all();
        return res;
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasksMtx;
    std::condition_variable cond;
    bool stop;
};

#endif
