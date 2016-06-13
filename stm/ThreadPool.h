#pragma once

#include <functional>

struct ThreadPoolPrivate;

class ThreadPool
{
public:
	using Func = std::function<void()>;
	enum State { RUNNING, PAUSED, STOPPED };

	ThreadPool(const std::size_t threadCount, const State state = RUNNING);
	~ThreadPool();

	void enqueue(Func f);

	//! Block until queue is empty
	void finishAll();
	bool nothingToDo() const;

	void pause();
	void run();

	State state() const;

private:
	ThreadPoolPrivate *m_detail;

	void startThread();

	//! No new threads will be fetched from queue. Cannot be undone
	void stop();
};

