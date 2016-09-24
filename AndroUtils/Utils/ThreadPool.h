#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

using namespace std;

namespace andro
{
template<typename TaskContext>
class ThreadPool
{
public:

	ThreadPool(int threads) :
		terminate(false),
		busy(ATOMIC_VAR_INIT(0U)),
		stopped(false)
	{
		for (int i = 0; i < threads; i++)
		{
			threadPool.emplace_back(thread(&ThreadPool::Invoke, this));
		}
	}

	void Enqueue(TaskContext& task)
	{
		{
			unique_lock<mutex> lock(tasksMutex);
			tasks.push(task);
		}
		// Wake up one thread.
		condition.notify_one();
	}

	void Invoke() {

		TaskContext task;
		while (true)
		{
			{
				unique_lock<mutex> lock(tasksMutex);

				// Wait until queue is not empty or terminate
				condition.wait(lock, [this]{ return !tasks.empty() || terminate; });

				//exit if terminate and queue is empty
				if (terminate && tasks.empty())
				{
					return;
				}

				busy++;

				// Get next task 
				task = tasks.front();
				tasks.pop();
			}

			// Execute 
			task();

			{
				unique_lock<mutex> lock(tasksMutex);
				busy--;
			}
			// notify task finished
			cv_finished.notify_one();
		}
	}

	void ShutDown()
	{
		if (!stopped)
		{
			{
				unique_lock<mutex> lock(tasksMutex);
				terminate = true;
			}

			// Wake up all threads.
			condition.notify_all();

			// Join all threads.
			for (thread &thread : threadPool)
			{
				thread.join();
			}

			threadPool.empty();
			stopped = true;
		}
	}
	~ThreadPool()
	{
		ShutDown();
	}

	void FlushQueue()
	{
		std::unique_lock<std::mutex> lock(tasksMutex);
		// wait until all jobs ar finished
		cv_finished.wait(lock, [this](){ return  (tasks.empty() && busy == 0); });
	}

private:
	vector<thread> threadPool;

	queue<TaskContext> tasks;

	// Task queue mutex.
	std::mutex tasksMutex;
	std::condition_variable condition;
	std::condition_variable cv_finished;
	std::atomic<unsigned int> busy;

	// Indicates that pool needs to be shut down.
	bool terminate;

	// Indicates that pool has been terminated.
	bool stopped;

};
}