#ifndef MESSAGE_QUEUE
#define MESSAGE_QUEUE

#include <mutex>
#include <queue>
#include <condition_variable>
#include "interpreter.hpp"

template <typename T>
class MessageQueue
{
public:
	bool empty() const {
		std::lock_guard<std::mutex> lock(the_mutex);
		return the_queue.empty();
	}
	void push(const T& value) {
		std::unique_lock<std::mutex> lock(the_mutex);
		the_queue.push(value);
		lock.unlock();
		the_condition_variable.notify_one();
	}
	bool try_pop(T &popped_value) {
		std::lock_guard<std::mutex> lock(the_mutex);
		if (the_queue.empty()) {
			return false;
		}
		popped_value = the_queue.front();
		the_queue.pop();
		return true;
	}
	void wait_and_pop(T &popped_value) {
		std::unique_lock<std::mutex> lock(the_mutex);
		while (the_queue.empty()) {
			the_condition_variable.wait(lock);
		}
		popped_value = the_queue.front();
		the_queue.pop();
	}
private:
	std::queue<T> the_queue;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;

};

#include "multiThread.tpp"
#endif



