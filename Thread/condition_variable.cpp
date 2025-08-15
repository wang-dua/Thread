/*/*
 * ������ֻ�Ǳ�֤���ʹ������ݵİ�ȫ�����޷���Ч�صȴ���������
 * ����ֻʹ�û�����, �ڻ�����Ϊ�յ�ʱ��, �����߻᲻ͣ�ļ���-��ѯ-����, ����æ��, ռ�ô���cpu
 * ���ʹ����������, ��ô�������߶Ի���������, ���Ϊ��, ���ͷ���, ��������״̬, ֪���������ͷŲ�Ϊ�յ��ź�
 * ���������ĵڶ�������������boolֵ, ������lamda
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
			//����ʱ����, ��������
			std::lock_guard<std::mutex> lock(mtx);
			q.push(i);
			std::cout << "Produce " << i << "\n";
		}
		cv.notify_one(); //����һ��֪ͨһ������
		std::this_thread::sleep_for(std::chrono::seconds(2)); //wait two seconds
	}
}

void consumer()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(mtx);
		//��wait�ĵڶ�������Ϊtrue(����), �̲߳Ż����ִ��,
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