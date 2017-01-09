/*
 * SharedMutex.h
 *
 *  Created on: Dec 8, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_UTILS_READWRITELOCK_H_
#define GPUPROCGENETICS_SRC_UTILS_READWRITELOCK_H_

#include <mutex>
#include <atomic>

namespace PGA {

// i am using this because shared_mutex isn't in the c++11 standard
class ReadWriteLock {
private:

	std::atomic_uint _readers;
	std::mutex		 _writer;

public:
	ReadWriteLock():
		_readers(0), _writer()
	{}

	void lock_read() {
		_writer.lock();
		_readers ++;
		_writer.unlock();
	}

	void unlock_read() {
		_writer.lock();
		_readers --;
		_writer.unlock();
	}

	void lock_write() {
		// spin until all the readers are gone
		do {
			_writer.lock();
			if (_readers > 0) {
				_writer.unlock();
				continue;
			}
			else {
				break;
			}
		} while(1);
	}

	void unlock_write() {
		_writer.unlock();
	}
};

class ReadLocker {
	ReadWriteLock& _lock;
public:
	ReadLocker(ReadWriteLock& lock): _lock(lock)
	{
		_lock.lock_read();
	}
	~ReadLocker() {
		_lock.unlock_read();
	}
};

class WriteLocker {
	ReadWriteLock& _lock;
public:
	WriteLocker(ReadWriteLock& lock): _lock(lock)
	{
		_lock.lock_write();
	}
	~WriteLocker() {
		_lock.unlock_write();
	}
};


}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_UTILS_READWRITELOCK_H_ */
