#ifndef AWAITER_H
#define AWAITER_H

#include "scheduler.h"
#include <coroutine>
#include <thread>
#include <iostream>

class Awaiter {
public:
    explicit Awaiter(int delayMs) : delayMs_(delayMs) {}
    ~Awaiter() { std::cout << "[~Awaiter]\n"; }

    bool await_ready() noexcept { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
        std::thread([handle, this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs_));
            // handle.resume();
            // Scheduler::getInstance().push(handle);
            Scheduler::getInstance().resume();
        }).detach();
    }

    void await_resume() noexcept {}

private:
    int delayMs_;
};

#endif // AWAITER_H

