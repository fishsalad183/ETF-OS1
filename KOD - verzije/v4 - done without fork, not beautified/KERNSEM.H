#ifndef KERNSEM_H_
#define KERNSEM_H_



#include "QUEUE.H"	// for type definition of Time


class KernelSem;


struct SemaphoreListElem {
	KernelSem* sem;
	SemaphoreListElem* next;

	SemaphoreListElem(KernelSem* s = 0, SemaphoreListElem* n = 0);
};



class KernelSem {
public:
	KernelSem(int init);
	~KernelSem();	// virtual?	-- da li da klasa bude final uopste?

	int wait(Time maxTimeToWait);
	void signal();

	static void timerInform();	// ovim tajmer obavestava da je protekao jos jedan period

	static SemaphoreListElem* SemaphoreListFirst;	// u listu poslednji dodat ide na pocetak - brze je, a drugacije nije potrebno
													// nije volatile!! zbog toga sto je pristup ovome uvek izmedju LOCK i UNLOCK
protected:
	friend class Semaphore;

private:
	int value;
	int valueIncrementedByExpiration;	// Field necessary to enable proper functioning of paired wait and signal calls.

	Queue* myQueue;
};




#endif /* KERNSEM_H_ */
