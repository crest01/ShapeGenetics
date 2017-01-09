
#include "Evaluator.h"


namespace PGG
{
	namespace CPU
	{
		Worker::Worker(Evaluator & evaluator, int id) :
			id(id), 
			evaluator(evaluator), 
			should_execute(true), 
			startuplock(tmutex),
			phase(0),
			myThread(&Worker::exec, this)
		{
			cv.wait(startuplock);
			startuplock.unlock();
		}
		void Worker::exec()
		{
			current_evalutator = &evaluator;
			for (auto &&func : evaluator.contextInitalizer)
				func->init();

			std::unique_lock<std::mutex> lk(tmutex);
			cv.notify_one();
			while (true)
			{
				cv.wait(lk);

				if (!should_execute)
				{
					lk.unlock();
					return;
				}
				
				int active = 1;
				while (true)
				{
					// process
					int executed = evaluator.executeNext(phase);
					if (executed == 0)
					{
						if (active)
						{
							evaluator.workerRunning.fetch_sub(1);
							active = 0;
						}
					}

					// end check
					if (evaluator.workerRunning.load() == 0)
					{
						//signal that i am fully done
						evaluator.workerDone.fetch_sub(1);

						//poll
						while (evaluator.workerRunning.load() == 0 && evaluator.workerDone.load() > 0)
							std::this_thread::yield();
						
						if (evaluator.workerDone.load() == 0)
							break;
						evaluator.workerDone.fetch_add(1);
					}
				}
				if (evaluator.workerFinalized.fetch_sub(1) == 1)
				{ 
					// avoid wakeup before sleep
					evaluator.threadWakeupMutex.lock();
					evaluator.threadWakeupMutex.unlock();
					evaluator.threadWakeupCV.notify_one();
				}
			}
		}
		void Worker::work(int phase)
		{
			{
				std::unique_lock<std::mutex> lk(tmutex);
				this->phase = phase;
			}
			cv.notify_one();
		}
		void Worker::terminate()
		{
			{
				std::unique_lock<std::mutex> lk(tmutex);
				should_execute = false;
			}
			cv.notify_one();
		}
		void Worker::join()
		{
			myThread.join();
		}

		bool Evaluator::run(int numThreads, int phase)
		{
			if (phase < 0)
			{
				// search for next phase
				phase = 0;
				for (auto && phaseQ : phaseQueues)
				{
					if (phaseQ.last() != nullptr)
						break;
					++phase;
				}
			}

			if (phase >= phaseQueues.size())
				return false;

			if (numThreads < 1)
				numThreads = 1;

			int running = 0;
			//make sure there is nobody polling the other states anymore
			while(workerFinalized.load() != 0)
				std::this_thread::yield();

			workerRunning.store(numThreads);
			workerDone.store(numThreads);
			workerFinalized.store(numThreads);

			// wake up threads and set current phase
			std::unique_lock<std::mutex> lk(threadWakeupMutex);

			for (auto && worker : workers)
			{
				worker->work(phase);
				++running;
			}
			for (; running < numThreads; ++running)
			{
				workers.emplace_back(new Worker(*this, running));
				workers.back()->work();
			}
			// wait for threads to finish
			threadWakeupCV.wait(lk);
			return true;
		}

		Evaluator::~Evaluator()
		{
			killWorkers();
		}
		void Evaluator::killWorkers()
		{
			for (auto && worker : workers)
				worker->terminate();
			for (auto && worker : workers)
				worker->join();
		}
		int Evaluator::executeNext(int phase)
		{
			auto el = phaseQueues[phase].pull();
			if (el != nullptr)
			{
				el->qexecute();
				LoopyAllocator::free(el);
				return 1;
			}
			return 0;
		}

		thread_local Evaluator* current_evalutator;
	}
}