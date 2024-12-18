#include "task.h"
#include "awaiter.h"
#include "scheduler.h"
#include <iostream>

Task childTask() {
    std::cout << "Child Task: Start\n";
    co_await Awaiter(1000);
    std::cout << "Child Task: End\n";
    co_await Awaiter(1000);
    std::cout << "Child Task: End\n";
}

Task parentTask() {
    std::cout << "Parent Task: Start\n";
    co_await childTask();
    std::cout << "Parent Task: Middle\n";
    co_await childTask();
    std::cout << "Parent Task: End\n";
}

Task topTask() {
    std::cout << "Top Task: Start\n";
    co_await parentTask();
    std::cout << "Top Task: End\n";
}

int main() {
    Scheduler& scheduler = Scheduler::getInstance();

    // auto task = parentTask();
    auto task = topTask();
    scheduler.push(task.get_handle());

    // 启动调度器
    scheduler.resume();
    // scheduler.resume();

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // scheduler.resume();
    // scheduler.resume();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    return 0;
}

