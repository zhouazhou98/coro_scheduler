#pragma once

#include "scheduler.h"
#include <coroutine>

class Task;

class TaskPromise {
public:
    TaskPromise() : parent(nullptr) {}

    std::suspend_always initial_suspend() noexcept { return {}; }

    std::suspend_always final_suspend() noexcept {
        // if (parent) {
        //     Scheduler::getInstance().push(parent);
        // }
        return {};
    }

    void set_parent(std::coroutine_handle<> parentTask) {
        parent = parentTask;
    }

    Task get_return_object();

    void unhandled_exception() { std::terminate(); }



private:
    std::coroutine_handle<> parent;
};


class Task {
public:
    using promise_type = TaskPromise;
    using CoroutineHandle = std::coroutine_handle<promise_type>;


    Task(std::coroutine_handle<promise_type> handle)
        : handle_(handle) {
            std::cout << "[Task] " << std:: endl;
            uint64_t ptr = (uint64_t)&handle_;
            std::cout << "[handle_] : " << ptr << std:: endl;
        }

    // ~Task() {
    //     if (handle_ && handle_.done()) {
    //         handle_.destroy();
    //     }
    // }

    ~Task() {
        // if (handle_ && handle_.done()) {
        //     handle_.destroy();
        // }
        std::cout << "[~Task] begin" << std:: endl;
        // if (handle_ && handle_.done()) {
        if (handle_ && handle_.done()) {
            uint64_t ptr = (uint64_t)&handle_;
            std::cout << "[~handle_] begin: " << ptr << std:: endl;
            handle_.destroy();
            std::cout << "[~handle_] end: " << ptr << std:: endl;
        }
        std::cout << "[~Task] end" << std:: endl;
    }

    void resume() {
        if (handle_ && !handle_.done()) {
            handle_.resume();
        }
    }

    bool await_ready() noexcept { return false; }

    void await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept {
        handle_.promise().set_parent(awaitingCoroutine);
        Scheduler::getInstance().push(handle_);
        Scheduler::getInstance().resume();
    }

    void await_resume() noexcept {}

    // 新增：显式获取内部 coroutine_handle
    std::coroutine_handle<> get_handle() const noexcept {
        return handle_;
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

inline Task TaskPromise::get_return_object() {
    return Task(std::coroutine_handle<TaskPromise>::from_promise(*this));
}

