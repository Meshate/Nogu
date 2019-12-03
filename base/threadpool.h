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
        explicit threadpool(size_t _n);

        ~threadpool();

        template<typename F, typename... Args>
        void add_task(F &&f, Args &&...args);

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

    //TODO 找出当F的参数为引用时无法改变值的原因
    template<typename F, typename... Args>
    void threadpool::add_task(F &&f, Args &&...args) {
        {
            std::unique_lock<std::mutex> lk(mtx_);
            auto func = std::make_shared<std::function<void()>>(std::function<void()>(
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                    ));
            tasks_.emplace([func]{
                (*func)();
            });
        }
        cv_.notify_one();
    }
}

#endif //NOGU_THREADPOOL_H
