#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <queue>
#include <vector>

class ThreadPool
{
public:
	/*
	 * numThreads线程数量由用户指定
	 * 
	 */
	ThreadPool(int numThreads): stop(false)
	{
		for (int i = 0; i < numThreads; i++)
		{
			threads.emplace_back //括号内的lamda是一个std::thread
			(
				[this]
				{
					while (true)
					{
						std::unique_lock<std::mutex> lock(mtx);
						condition.wait(lock, [this] {return !tasks.empty() || stop; });
						// tasks非空时或者stop == true, 线程能继续, 不会阻塞
						//这里的stop是在析构函数中, 如果stop == true, 说明要唤醒所有线程, 执行完毕

						if (stop && tasks.empty())
						{
							return;
						}

						std::function<void()> task(std::move(tasks.front()));
						tasks.pop();
						lock.unlock();
						task();
					}
				}
			); //添加线程, emplace_back()内的lambda相当于创建线程时的入口函数, 指明该线程做的事情, 然后添加到threads中
		}
	}
	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(mtx);
			stop = true;
		}
		condition.notify_all();
		for (auto &t : threads)
		{
			t.join();
		}
	}

	/*
	 * 打包参数包: ...可以打包参数包
	 * 参数包的展开: 当需要展开时, 使用参数包名... 可以展开参数, 重要的是 ...
	 * bind的使用: 这里的bind可以将多参数的函数绑定到一个零参数函数task, 进行统一
	 * 完美转发forward: 搭配模板中的万能引用, 可以保持传入参数的原类型(右值 -> 右值, 左值 -> 左值) 
	 */
	template <typename F, typename ...Args> 
	void enqueue(F &&f, Args &&...args)
	{
		/*
		 *function<void()>是一个可以存储“零参数函数”的容器, 也就是task,
		 *bind的作用是将函数 f(args...) 绑定到 零参数函数task
		 *也就是说task() 等同 f(args...)
		 */
		std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		{
			std::unique_lock<std::mutex> lock(mtx);
			tasks.emplace(std::move(task));
		}
		condition.notify_one();

 	} //生产任务, 提醒线程

private:
	std::vector<std::thread> threads; //线程数组(消费者), 用于执行任务
	std::queue<std::function<void()>> tasks; //tasks队列, 存储void()函数, 由用户(生产者)给出时
	std::mutex mtx;
	std::condition_variable condition;
	bool stop; 
};

int main()
{
	ThreadPool threadPool(4); //此时有四个线程并发, 处理任务并非按顺序 i = 01234...9 处理
	for (int i = 0; i < 10; i++)
	{
		threadPool.enqueue([i]
		{
			std::cout << "task " << i << " is running..." << "\n";
			std::this_thread::sleep_for(std::chrono::seconds(2));
			std::cout << "task " << i << " is done." << "\n";
		}); //这里使用了零参数的lambda函数 作为 task() 入队
	}
	return 0;
}