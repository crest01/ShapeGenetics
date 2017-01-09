#pragma once

#include <thread>
#include <atomic>
#include <mutex>

namespace PGG
{
	namespace CPU
	{

		struct QueueElement
		{
			virtual void qexecute() = 0;
			std::atomic<QueueElement*> volatile next;
		};


		template<bool LOCKFREE = true>
		class Queue
		{
		public:
			void push(QueueElement* element);
			QueueElement* pull();
			QueueElement* first();
			QueueElement* last();
			void setFirst(QueueElement* element);
			void clear();
		};

		template<class Payload>
		struct QueueElementElementWrapper : public Payload, public QueueElement
		{
			template<typename ... PayloadIntializers>
			QueueElementElementWrapper(PayloadIntializers... payload) : Payload(payload...)
			{
				QueueElement::next.store(nullptr);
			}
			void qexecute()
			{
				Payload::execute();
			}
		};


		template<>
		class Queue< true>
		{
			struct FirstElement : public QueueElement
			{
				void qexecute() {}
			};

			FirstElement front;
			std::atomic<QueueElement*> volatile back;

		public:
			Queue() : front(), back(&front)
			{
			}

			void push(QueueElement* element)
			{
				element->next.exchange(nullptr);
				QueueElement* prev = back.exchange(element);
				prev->next.exchange(element);
			}
			QueueElement* pull()
			{
				while (true)
				{
					QueueElement* next = front.next.load();
					if (next != nullptr)
					{
						QueueElement* nextnext = next->next.load();
						if (front.next.compare_exchange_strong(next, nextnext))
						{
							//we have got something
							if (nextnext == nullptr)
							{
								//need to ajdust back
								QueueElement* exp_backnext = next;
								if (!back.compare_exchange_strong(exp_backnext, &front))
								{
									//did not work, someone else already came in
									//need to wait for this one to write so we can fix the front->next
									while ((nextnext = next->next.load()) == nullptr)
										std::this_thread::yield();
									front.next.exchange(nextnext);
								}
							}
							return next;
						}
					}
					else
						return next;
				}
			}

			QueueElement* first()
			{
				return front.next.load();
			}
			QueueElement* last()
			{
				QueueElement* loadedback = back.load();
				if (back.load() != &front)
					return loadedback;
				else
					return front.next.load();
			}
			void setFirst(QueueElement* element)
			{
				front.next.exchange(element);
			}
			void clear()
			{
				front.next.store(nullptr);
				back.store(&front);
			}
		};

		template<>
		class Queue<false>
		{
			QueueElement * volatile front, *volatile back;
			std::mutex mutex;
		public:
			Queue() : front(nullptr), back(nullptr)
			{
			}

			void push(QueueElement* element)
			{
				element->next.store(nullptr);
				std::lock_guard<std::mutex> g(mutex);
				QueueElement *oldback = back;
				back = element;
				if (oldback == nullptr)
					front = element;
				else
					oldback->next.store(element);
			}
			QueueElement* pull()
			{
				QueueElement* outEntry;
				std::lock_guard<std::mutex> g(mutex);
				outEntry = front;
				if (outEntry)
				{
					front = outEntry->next.load();
					if (back == outEntry)
						back = nullptr;
				}
				return outEntry;
			}

			QueueElement* first()
			{
				return front;
			}
			QueueElement* last()
			{
				return back;
			}
			void setFirst(QueueElement* element)
			{
				std::lock_guard<std::mutex> g(mutex);
				front = element;
			}
			void clear()
			{
				std::lock_guard<std::mutex> g(mutex);
				front = back = nullptr;
			}
		};
	}

}