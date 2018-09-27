#ifndef PCB_H_
#define PCB_H_


#include "THREAD.H"
#include "QUEUE.H"



class PCB {
public:
	static volatile PCB* running;	// ovako?

	static volatile PCB *PCBListFirst, *PCBListLast;	// volatile?
	PCB* next;	// za ulancavanje!
	// da li da lista bude dvostruko ulancana?


	volatile int status;	// NEW, READY, RUNNING, BLOCKED, FINISHED, IDLE - definisano u zaglavlju za definicije


	unsigned* stack;
	StackSize stackSize;

	unsigned sp;
	unsigned ss;
	unsigned bp;

	Time timeSlice;


	Thread* myThread;
	Queue* myWaitQueue;		// red niti koje cekaju da se ova zavrsi


	PCB(Thread* myThr, StackSize stackSize, Time timeSlice);
	~PCB();


	static void wrapper();

	static unsigned int numberOfActiveThreads();

protected:
	friend class Thread;

private:
	static volatile unsigned int num;	// volatile?

};



#endif /* PCB_H_ */
