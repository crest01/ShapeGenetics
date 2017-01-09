#ifndef EVALUATOR_INCLUDED
#define EVALUATOR_INCLUDED

#pragma once
#include <vector>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <list>

#include "Queue.h"
#include "Allocator.h"

#if defined(_MSC_VER) && _MSC_VER < 1900
#define thread_local __declspec( thread )
#endif

namespace PGG
{
	namespace CPU
	{
		class Evaluator;
		class Worker
		{
			int id;
			volatile bool should_execute;
			std::mutex tmutex;
			std::unique_lock<std::mutex> startuplock;
			std::condition_variable cv;
			Evaluator & evaluator;
			volatile int phase;

			std::thread myThread;
			void exec();

		public:

			Worker(const Worker& other) = delete;
			Worker & operator=(const Worker&) = delete;

			Worker(Evaluator & evaluator, int id);
			void work(int phase = 0);
			void terminate();
			void join();
		};

		class Evaluator
		{
			friend class Worker;
			//every phase gets its own queue
			std::vector< Queue<> > phaseQueues;

			std::list<std::unique_ptr<Worker> > workers;
			std::mutex threadWakeupMutex;
			std::condition_variable threadWakeupCV;
			std::atomic<int> workerRunning;
			std::atomic<int> workerDone;
			std::atomic<int> workerFinalized;

			struct EvaluatorContextInitializer
			{
				virtual void init() = 0;
			};

			template<typename F>
			struct EvaluatorContextInitializerImpl : EvaluatorContextInitializer
			{
				F f;
				EvaluatorContextInitializerImpl(F f) : f(f) {}
				void init()
				{
					f();
				}
			};

			std::list<std::unique_ptr<EvaluatorContextInitializer> > contextInitalizer;

			template<class T, class Procedure>
			struct ProcEvalEntry
			{
				struct NoContext
				{ };

				T shape;
				ProcEvalEntry(const T& s) : shape(s) { }
				void execute()
				{
					Procedure :: template execute<NoContext>(shape);
				}
			};

			template<class T, class F>
			struct FuncEvalEntry
			{
				T shape;
				F func;
				FuncEvalEntry(const T& s, F& f) : shape(s), func(f) {}
				void execute()
				{
					func(shape);
				}
			};

			int executeNext(int phase);
			void killWorkers();
		public:
/*
			Evaluator(int MaxNumPhases = 128)
			{
				workerFinalized.store(0);
				phaseQueues.swap( decltype(phaseQueues)(MaxNumPhases) );
	//			phaseQueues.resize(MaxNumPhases);
			}
			~Evaluator();

			void setNumPhases(int maxNumPhases)
			{
				phaseQueues.resize(maxNumPhases);
			}
			*/

			Evaluator(int MaxNumPhases = 128) :
				phaseQueues(MaxNumPhases)
			{
			    workerFinalized.store(0);
			}
		   ~Evaluator();

		   void setNumPhases(int maxNumPhases)
		   {
			   decltype(phaseQueues) temp(maxNumPhases);
			   phaseQueues.swap(temp);
		   }

			template<class T, class Procedure>
			void enqueue(const T& data, int phase = 0)
			{
				// allocate a wrapped queue element and also
				typedef QueueElementElementWrapper<ProcEvalEntry<T, Procedure> > QEntry;
				void* space = LoopyAllocator::alloc(sizeof(QEntry));
				QEntry* newElement = new(space)QEntry(data);

				phaseQueues[phase].push(newElement);
			}

			template<class T, class F>
			void enqueue(const T& data, F f, int phase = 0)
			{
				// allocate a wrapped queue element and also
				typedef QueueElementElementWrapper<FuncEvalEntry<T, F> > QEntry;
				void* space = LoopyAllocator::alloc(sizeof(QEntry));
				QEntry* newElement = new(space)QEntry(data, f);

				phaseQueues[phase].push(newElement);
			}

			template<class F>
			void addContextInitializer(F f)
			{
				contextInitalizer.emplace_back(new EvaluatorContextInitializerImpl<F>(f));
				killWorkers();
				workers.clear();
			}

			bool run(int numThreads = 1, int phase = -1);
		};

		extern thread_local Evaluator* current_evalutator;
	}
}

#endif
