#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <queue>
#include <condition_variable>

class ThreadPool {

public:
    ThreadPool() = default;
    ThreadPool(ThreadPool&&) = default;
    ThreadPool(int numThreads = 8) {
        for(int i = 0; i < numThreads; i ++ ) {
            threads.emplace_back([this] {
                while(true) {
                std::unique_lock<std::mutex> lock(mtx_);
                condition.wait(lock, [this] {
                    return stop_ || !tasks.empty();
                });
                if(stop_ && tasks.empty()) return;
                std::function<void()> task(move(tasks.front()));
                tasks.pop();
                lock.unlock();
                task();
            }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            stop_ = true;
        }
        condition.notify_all();
        for(auto& t : threads) {
            t.join();
        }
    }
    template<class F, class ...Args>
    void enqueue(F f, Args ...args) {
        std::function<void()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        {
            std::unique_lock<std::mutex> lock(mtx_);
            tasks.emplace(move(task));
        }
        condition.notify_one();
    }

private:
    std::mutex mtx_;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    bool stop_;
    std::condition_variable condition;

};


#endif