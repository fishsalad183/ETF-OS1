#include "MACRO.H"
#include "THREAD.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include <iostream.h>	// radi dijagnostickog ispisa



extern void interrupt timer(...);
//extern volatile int context_change_requested;
extern volatile int explicit_call;

extern Thread* starting;
extern IdleThread* idle;


ID Thread::prevID = 0;



ID Thread::getID() const {
	return id;
}

ID Thread::getRunningID() {
	if (PCB::running) {
		return PCB::running->myThread->getID();
	}
	return 0;
}

Thread* Thread::getThreadByID(ID id) {	// neefikasno?
	for (PCB* cur = (PCB*) PCB::PCBListFirst; cur != 0; cur = cur->next) {
		if (cur->myThread->getID() == id) return cur->myThread;
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
		asm cli;	// za slucaj omogucavanja
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
	//waitToComplete();		// POZIVA SE SAMO U IZVEDENIM KLASAMA, KAD JE POTREBNO
	LOCK
	delete myPCB;
	UNLOCK
}



void Thread::waitToComplete() {
	LOCK

	int s = myPCB->status;
	if (s == NEW || s == FINISHED || myPCB == PCB::running || this == starting || this == idle) {
		// ostalo?    ---    this == idle moze kao s == IDLE, myPCB == PCB::running moze kao s == RUNNING
		// mada je ovako dobro jer ne zavisi od toga da li IDLE postoji kao status
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
	timer();		// timer kao prekid? ovde je ipak osiguran od preuzimanja
	explicit_call = 0;
	UNLOCK
}
