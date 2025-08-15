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
	 * numThreads�߳��������û�ָ��
	 * 
	 */
	ThreadPool(int numThreads): stop(false)
	{
		for (int i = 0; i < numThreads; i++)
		{
			threads.emplace_back //�����ڵ�lamda��һ��std::thread
			(
				[this]
				{
					while (true)
					{
						std::unique_lock<std::mutex> lock(mtx);
						condition.wait(lock, [this] {return !tasks.empty() || stop; });
						// tasks�ǿ�ʱ����stop == true, �߳��ܼ���, ��������
						//�����stop��������������, ���stop == true, ˵��Ҫ���������߳�, ִ�����

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
			); //����߳�, emplace_back()�ڵ�lambda�൱�ڴ����߳�ʱ����ں���, ָ�����߳���������, Ȼ����ӵ�threads��
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
	 * ���������: ...���Դ��������
	 * ��������չ��: ����Ҫչ��ʱ, ʹ�ò�������... ����չ������, ��Ҫ���� ...
	 * bind��ʹ��: �����bind���Խ�������ĺ����󶨵�һ�����������task, ����ͳһ
	 * ����ת��forward: ����ģ���е���������, ���Ա��ִ��������ԭ����(��ֵ -> ��ֵ, ��ֵ -> ��ֵ) 
	 */
	template <typename F, typename ...Args> 
	void enqueue(F &&f, Args &&...args)
	{
		/*
		 *function<void()>��һ�����Դ洢�������������������, Ҳ����task,
		 *bind�������ǽ����� f(args...) �󶨵� ���������task
		 *Ҳ����˵task() ��ͬ f(args...)
		 */
		std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		{
			std::unique_lock<std::mutex> lock(mtx);
			tasks.emplace(std::move(task));
		}
		condition.notify_one();

 	} //��������, �����߳�

private:
	std::vector<std::thread> threads; //�߳�����(������), ����ִ������
	std::queue<std::function<void()>> tasks; //tasks����, �洢void()����, ���û�(������)����ʱ
	std::mutex mtx;
	std::condition_variable condition;
	bool stop; 
};

int main()
{
	ThreadPool threadPool(4); //��ʱ���ĸ��̲߳���, �������񲢷ǰ�˳�� i = 01234...9 ����
	for (int i = 0; i < 10; i++)
	{
		threadPool.enqueue([i]
		{
			std::cout << "task " << i << " is running..." << "\n";
			std::this_thread::sleep_for(std::chrono::seconds(2));
			std::cout << "task " << i << " is done." << "\n";
		}); //����ʹ�����������lambda���� ��Ϊ task() ���
	}
	return 0;
}