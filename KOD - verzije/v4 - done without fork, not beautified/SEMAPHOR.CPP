#include "DEFS.H"
#include "KERNSEM.H"
#include "semaphor.h"



Semaphore::Semaphore(int init) {
	LOCK
	myImpl = new KernelSem(init);
	UNLOCK
}


Semaphore::~Semaphore() {
	LOCK
	delete myImpl;
	myImpl = 0;
	UNLOCK
}


void Semaphore::signal() {
	LOCK
	myImpl->signal();
	UNLOCK
}


int Semaphore::wait(Time maxTimeToWait) {
	LOCK
	int ret = myImpl->wait(maxTimeToWait);
	UNLOCK
	return ret;
}


int Semaphore::val() const {
	LOCK
	int ret = myImpl->value;	// Semaphore is friend to KernelSem
	UNLOCK
	return ret;
}

