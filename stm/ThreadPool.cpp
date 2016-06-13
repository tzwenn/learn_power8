#include "ThreadPool.h"

#include <vector>
#include <deque>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <iostream>

struct ThreadPoolPrivate
{
	std::vector<std::thread> threads;
	std::deque<ThreadPool::Func> tasks;
	std::condition_variable cv;
	std::mutex mutex;

	ThreadPool::State state;
};

ThreadPool::ThreadPool(const std::size_t threadCount, const State state) :
	m_detail(new ThreadPoolPrivate)
{
	m_detail->state = state;
	m_detail->threads.reserve(threadCount);

	for (std::size_t i = 0; i < threadCount; i++) {
		m_detail->threads.emplace_back(&ThreadPool::startThread, this);
	}
}

ThreadPool::~ThreadPool()
{
	stop();
	for (auto & t: m_detail->threads)
		t.join();
	delete m_detail;
}

void ThreadPool::enqueue(Func f)
{
	{
		std::unique_lock<std::mutex> lock(m_detail->mutex);
		m_detail->tasks.push_back(f);
	}
	m_detail->cv.notify_one();
}

void ThreadPool::finishAll()
{
	run();
	std::unique_lock<std::mutex> lock(m_detail->mutex);
	m_detail->cv.wait(lock, [&]{
		return m_detail->tasks.empty();
	});
}

void ThreadPool::pause()
{
	std::unique_lock<std::mutex> lock(m_detail->mutex);
	m_detail->state = PAUSED;
	m_detail->cv.notify_all();
}

void ThreadPool::run()
{
	std::unique_lock<std::mutex> lock(m_detail->mutex);
	m_detail->state = RUNNING;
	m_detail->cv.notify_all();
}

void ThreadPool::stop()
{
	std::unique_lock<std::mutex> lock(m_detail->mutex);
	m_detail->state = STOPPED;
	m_detail->cv.notify_all();
}

ThreadPool::State ThreadPool::state() const
{
	return m_detail->state;
}

bool ThreadPool::nothingToDo() const
{
	return m_detail->tasks.empty();
}

void ThreadPool::startThread()
{
	Func nextTask;
	while (true) {
		{
			std::unique_lock<std::mutex> lock(m_detail->mutex);
			m_detail->cv.wait(lock, [&]{
				return (m_detail->state == STOPPED ||
						  (m_detail->state == RUNNING && !m_detail->tasks.empty()));
			});
			if (m_detail->state == STOPPED)
				return;

			nextTask = m_detail->tasks.front();
			m_detail->tasks.pop_front();
		}
		if (nextTask)
			nextTask();
	}
}
