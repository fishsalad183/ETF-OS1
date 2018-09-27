#ifndef KERNSEM_H_
#define KERNSEM_H_



class KernelSem;


struct SemaphoreListElem {
	KernelSem* sem;
	SemaphoreListElem* next;

	SemaphoreListElem(KernelSem* s = 0, SemaphoreListElem* n = 0);
};



class Queue;
typedef unsigned int Time;



class KernelSem {
public:
	KernelSem(int init);
	~KernelSem();

	int wait(Time maxTimeToWait);
	void signal();

	static void timerInform();	// timer uses this method to inform about time passed

	static SemaphoreListElem* SemaphoreListFirst;	// new list member is placed at the beginning of the list

protected:
	friend class Semaphore;

private:
	int value;
	int valueIncrementedByExpiration;	// for proper functioning of paired wait and signal calls

	Queue* myQueue;
};




#endif /* KERNSEM_H_ */
