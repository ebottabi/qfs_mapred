/*
 * shared_queue.hpp
 *
 *  Created on: Nov 23, 2012
 *      Author: https://www.quantnet.com/threads/c-multithreading-in-boost.10028/
 */

#ifndef SHARED_QUEUE_H_
#define SHARED_QUEUE_H_

#include <queue>
#include <boost/thread.hpp>

// Queue class that has thread synchronisation
template<typename T>
class shared_queue {
private:
	std::queue<T> m_queue; // Use STL queue to store data
	boost::mutex m_mutex; // The mutex to synchronise on
	boost::condition_variable m_cond; // The condition to wait for

public:

	// Add data to the queue and notify others
	void Enqueue(const T& data) {
		// Acquire lock on the queue
		boost::unique_lock<boost::mutex> lock(m_mutex);

		// Add the data to the queue
		m_queue.push(data);

		// Notify others that data is ready
		m_cond.notify_one();

	} // Lock is automatically released here

	// Get data from the queue. Wait for data if not available
	T Dequeue() {

		// Acquire lock on the queue
		boost::unique_lock<boost::mutex> lock(m_mutex);

		// When there is no data, wait till someone fills it.
		// Lock is automatically released in the wait and obtained
		// again after the wait
		while (m_queue.size() == 0)
			m_cond.wait(lock);

		// Retrieve the data from the queue
		T result = m_queue.front();
		m_queue.pop();
		return result;

	} // Lock is automatically released here
};

#endif /*SHARED_QUEUE_H_*/
