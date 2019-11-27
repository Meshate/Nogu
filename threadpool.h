//
// Created by meshate on 2019/11/26.
//

#ifndef NOGU_THREADPOOL_H
#define NOGU_THREADPOOL_H

#include <future>
#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>

namespace nogu {
    class threadpool {
    public:
        threadpool(size_t _n);

        ~threadpool();

        template<typename F>
        void add_task(F &&f);

    private:
        std::queue<std::function<void()>> tasks_;
        std::vector<std::thread> workers_;
        std::mutex mtx_;
        std::condition_variable cv_;
        bool shutdown = false;
    };

    threadpool::threadpool(size_t _n) {
        for (int i = 0; i < _n; ++i) {
            workers_.emplace_back([this]() {
                std::unique_lock<std::mutex> lk(mtx_);
                for (;;) {
                    if (!tasks_.empty()) {
                        auto task = std::move(tasks_.front());
                        tasks_.pop();
                        lk.unlock();
                        task();
                        lk.lock();
                    } else if (shutdown) {
                        break;
                    } else {
                        cv_.wait(lk);
                    }
                }
            });
        }
    }

    threadpool::~threadpool() {
        {
            std::unique_lock<std::mutex> lk(mtx_);
            shutdown = true;
        }
        cv_.notify_all();
        for (auto &i:workers_) {
            i.join();
        }
    }

    //TODO 添加函数参数支持 template<typename F, typename... Args>
    template<typename F>
    void threadpool::add_task(F &&f) {
        {
            std::unique_lock<std::mutex> lk(mtx_);
            tasks_.emplace(std::forward<F>(f));
        }
        cv_.notify_one();
    }
}

#endif //NOGU_THREADPOOL_H
