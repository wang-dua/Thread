/*/*
 * 互斥锁只是保证访问共享数据的安全，但无法高效地等待条件满足
 * 例如只使用互斥锁, 在缓冲区为空的时候, 消费者会不停的加锁-查询-解锁, 进入忙等, 占用大量cpu
 * 如果使用条件变量, 那么当消费者对缓冲区加锁, 如果为空, 则释放锁, 进入阻塞状态, 知道生产者释放不为空的信号
 * 条件变量的第二个参数不能是bool值, 可以是lamda
 #1#
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <queue>

std::queue<int> q;
std::condition_variable cv;
std::mutex mtx;
void producer()
{
	for (int i = 0; i < 5; i++)
	{
		{
			//生产时加锁, 结束解锁
			std::lock_guard<std::mutex> lock(mtx);
			q.push(i);
			std::cout << "Produce " << i << "\n";
		}
		cv.notify_one(); //生完一个通知一个进程
		std::this_thread::sleep_for(std::chrono::seconds(2)); //wait two seconds
	}
}

void consumer()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(mtx);
		//当wait的第二个参数为true(不空), 线程才会继续执行,
		cv.wait(lock, [] {return !q.empty(); });
		int val = q.front();
		q.pop();
		std::cout << "Consume " << val << "\n";

		if (val == 4)
			break;
	}
}

int main()
{
	std::thread p(producer);
	std::thread c(consumer);

	p.join();
	c.join();
	return 0;
}*/