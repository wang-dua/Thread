/*����.lock() / .unlock() ��Ρ�

֧���ӳټ�����std::defer_lock����

���Ժ� std::condition_variable ���ʹ�ã���Ϊ lock_guard ��֧�����������ȴ����ͷ� / ���¼����߼�����*/
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