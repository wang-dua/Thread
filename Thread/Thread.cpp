#include <iostream>
#include <thread>
#include <mutex>

std::once_flag initFlag; // 用来记录某个任务是否已经执行
std::mutex mtx;
void initResource() {
    std::cout << "初始化资源，线程ID: " << std::this_thread::get_id() << "\n";
}

void worker(int id) {
    // 多线程同时调用，但 initResource 只会被执行一次
    std::call_once(initFlag, initResource);
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "线程 " << id << " 工作中...\n";
}

/*
int main() {
    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    std::thread t3(worker, 3);

    t1.join();
    t2.join();
    t3.join();
}
*/
