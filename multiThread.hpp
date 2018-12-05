#ifndef MESSAGE_QUEUE
#define MESSAGE_QUEUE

#include <mutex>
#include <queue>
#include "interpreter.hpp"

template <typename T>
class MessageQueue
{
public:
	void push(const T & value);
	bool empty() const;
	bool try_pop(T& popped_value);
	void wait_and_pop(T& popped_value);
private:
	std::queue<T> the_queue;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;

};

#include "multiThread.tpp"
#endif



