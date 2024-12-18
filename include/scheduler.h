#pragma once

#include <stack>
#include <coroutine>
#include <iostream>
#include <memory>

class Scheduler {
public:
    static Scheduler& getInstance() {
        static Scheduler instance;
        return instance;
    }

    void push(std::coroutine_handle<> handle) {
        std::cout << "push: " << (uint64_t)&handle << std::endl;
        call_stack_.push(handle);
    }

    void pop() {
        if (!call_stack_.empty()) {
            call_stack_.pop();
            // std::coroutine_handle<> handle = (std::coroutine_handle<>)call_stack_.pop();
            // std::cout << "pop: " << (uint64_t)&handle << std::endl;
        }
    }

    void resume() {
        while (!call_stack_.empty()) {
            auto current = call_stack_.top();
            current.resume();
            if (current.done()) {
                call_stack_.pop();
            } else {
                break;  // 当前协程还未完成，等待下一次 resume
            }
        }
    }

    bool empty() const {
        return call_stack_.empty();
    }

private:
    std::stack<std::coroutine_handle<>> call_stack_;
};

