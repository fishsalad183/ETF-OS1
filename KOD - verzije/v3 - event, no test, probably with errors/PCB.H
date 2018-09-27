#ifndef PCB_H_
#define PCB_H_


#include "THREAD.H"
#include "QUEUE.H"


#ifndef DEF_H_
#define NEW 1
#define READY 2
#define RUNNING 3
#define BLOCKED 4
#define FINISHED 5
#define IDLE 6
#endif



class PCB {
public:
	static volatile PCB* running;	// ovako?

	static volatile PCB *PCBListFirst, *PCBListLast;	// volatile?
	PCB* next;	// za ulancavanje!
	// da li da lista bude dvostruko ulancana?


	volatile int status;	// NEW, READY, RUNNING, BLOCKED, FINISHED, IDLE
	int unblockedBySignal;

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
	static volatile unsigned int num;	// volatile? ima li potrebe uopste za ovim?

#ifndef BCC_BLOCK_IGNORE
	PCB(const PCB&) {}	// zabrana kopiranja
#endif
};



#endif /* PCB_H_ */
