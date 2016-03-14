#ifndef _MYTHREAD_
#define _MYTHREAD_

#include<pthread.h>

class MyThreadClass
{
public:
   MyThreadClass() {/* empty */}
   virtual ~MyThreadClass() {/* empty */}

   /** Returns true if the thread was successfully started, false if there was an error starting the thread */
public:
   bool StartInternalThread()
   {
      return (pthread_create(&_thread, NULL, InternalThreadEntryFunc, this) == 0);
   }

   /** Will not return until the internal thread has exited. */
   void WaitForInternalThreadToExit()
   {
      (void) pthread_join(_thread, NULL);
   }
   
   void CancelInternalThread()
   {
	//cout<<"Entered Cancel Thread\n";
	(void) pthread_cancel(_thread);
	//cout<<"Cancelled thread\n";
   }
	
protected:
   /** Implement this method in your subclass with the code you want your thread to run. */
   virtual void ThreadFunc() = 0;

private:
   static void * InternalThreadEntryFunc(void * This) {((MyThreadClass *)This)->ThreadFunc(); return NULL;}

   pthread_t _thread;
};
#endif
