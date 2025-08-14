/*可以.lock() / .unlock() 多次。

支持延迟加锁（std::defer_lock）。

可以和 std::condition_variable 配合使用（因为 lock_guard 不支持条件变量等待的释放 / 重新加锁逻辑）。*/
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

/*
std::mutex mtx;

void task()
{
	std::unique_lock<std::mutex> lock(mtx);
	std::cout << "Thread" << std::this_thread::get_id() << " got the lock.\n";

	lock.unlock();
	std::cout << "Thread" << std::this_thread::get_id() << " unlock.\n";
	std::this_thread::sleep_for(std::chrono::seconds(3));

	lock.lock();
	std::cout << "Thread" << std::this_thread::get_id() << " re-lock.\n";


}
*/

/*int main()
{
	std::thread t1(task);
	std::thread t2(task);

	t1.join();
	t2.join();
	return 0;
}*/