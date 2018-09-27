#include "thread.h"
#include "PCB.H"
#include "SCHEDULE.H"
#include "QUEUE.H"
#include "DEFS.H"
#include <iostream.h>



extern Thread* starting;
extern IdleThread* idle;

extern volatile int explicit_call;
extern void interrupt timer(...);



ID Thread::prevID = 0;


ID Thread::getID() {
	return id;
}


ID Thread::getRunningID() {
	if (PCB::running != 0) {
		return PCB::running->myThread->getID();
	}
	return 0;
}


Thread* Thread::getThreadByID(ID id) {
	for (PCB* cur = (PCB*) PCB::PCBListFirst; cur != 0; cur = cur->next) {
		if (cur->myThread->id == id) return cur->myThread;
	}
	return 0;
}



void Thread::start() {
	LOCK

	if (myPCB->status == NEW) {
		myPCB->status = READY;
		Scheduler::put(myPCB);
	}
	else {
		cout << "THREAD CANNOT START AGAIN!" << endl;
		asm cli;	// in case cout caused sti
	}

	UNLOCK
}



Thread::Thread (StackSize stackSize, Time timeSlice) {
	LOCK
	id = ++prevID;
	myPCB = new PCB(this, stackSize, timeSlice);
	UNLOCK
}



Thread::~Thread() {
	// waitToComplete() is to be called in derived classes!
	LOCK
	delete myPCB;
	UNLOCK
}



void Thread::waitToComplete() {
	LOCK

	int s = myPCB->status;
	if (s == NEW || s == FINISHED || myPCB == PCB::running || this == starting || this == idle) {
		// This way there are no dependencies on IDLE as a status. (IDLE is a status even when running!)
		// also possible:  this == idle  <=>  s == IDLE  ,  myPCB == PCB::running  <=>  s == RUNNING
		UNLOCK
		return;
	}

	PCB::running->status = BLOCKED;
	myPCB->myWaitQueue->put((PCB*) PCB::running);
	dispatch();

	UNLOCK
}



void dispatch() {
	LOCK
	explicit_call = 1;
	timer();
	//explicit_call = 0;	- done in timer();
	UNLOCK
}
