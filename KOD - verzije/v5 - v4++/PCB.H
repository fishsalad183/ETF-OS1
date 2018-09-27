#ifndef PCB_H_
#define PCB_H_


#include "thread.h"	// for typedefs (Time, StackSize)
class Queue;


#ifndef DEFS_H_
#define NEW 1
#define READY 2
#define RUNNING 3
#define BLOCKED 4
#define FINISHED 5
#define IDLE 6
#endif



class PCB {
public:
	static volatile PCB* running;

	static PCB *PCBListFirst, *PCBListLast;	// volatile?
	PCB* next;


	volatile int status;	// NEW, READY, RUNNING, BLOCKED, FINISHED, IDLE
	int unblockedBySignal;	// necessary for int Semaphore::wait(Time maxTimeToWait)

	unsigned* stack;
	StackSize stackSize;

	unsigned sp;
	unsigned ss;
	unsigned bp;

	Time timeSlice;


	Thread* myThread;
	Queue* myWaitQueue;


	PCB(Thread* myThr, StackSize stackSize, Time timeSlice);
	~PCB();


	static void wrapper();


protected:
	friend class Thread;
};



#endif /* PCB_H_ */
