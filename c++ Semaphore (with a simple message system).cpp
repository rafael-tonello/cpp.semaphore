#ifndef _SEMAPHORE_
#define _SEMAPHORE_

/* A sempahore class that allows to send messages to waiters. A middle ground 
 * between a commmon Semaphore and a channel system */
Class Semaphore{
	private:
		std::mutex counterMutex;
		std::mutex m;
		int count;
		queue<void *> messageToNext;
	public:
		Semaphore(int start): count(start){}
		
		//await for continue. If there is a message in internal message vector,
		//one of them will be returned. Otherwise, NULL will be returned.
		void* wait()
		{
			void* ret = NULL;
			m.lock()
			
			counterMutex.lock();
			count--;
			if (count > 0)
				m.unlock();	
			
			if (!messageToNext.empty())
				ret = messageToNext.pop();
			counterMutex.unlock();
			
			return ret;
		}
		void *get(){ return wait(); }
		void *receive(){ return wait(); }
		
		//release one waiter. You can pass a message for the next waiter.
		release(void *MessageToNext = NULL)
		{
			counterMutex.lock();
			count++;
			if (messageToNext != NULL)
				messageToNext.push(messageToNext);
			
			if (count >=1)
				m.unlock();
			counterMutex.unlock();
			
		}
		set(void *MessageToNext = NULL){release(MessageToNext); }
		send(void *MessageToNext = NULL){release(MessageToNext); }
		
		//a simple thread helper
		
};

//use example
/*
Semaphore sm* = new Semaphore(0)
ThreadPool tp* = new ThreadPool();
for (auto &curr: list){
	//enqueue the 'curr' in the 'sm'
	sm->release(curr);
	
	//create a task that will run parallel
	tp->enqueue([sm](){
		//get next 'curr' from 'sm'
		var currp = sm->wait();
	});
}*/
#endif