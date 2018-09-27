#include "MACRO.H"
#include "IDLE.H"
#include "PCB.H"
#include <iostream.h>


//extern volatile int context_change_requested;
//extern volatile unsigned int lockflag;


IdleThread::IdleThread() : Thread(minimumStackSize, 1) {
	LOCK
	done = 0;
	myPCB->status = NEW;
	UNLOCK
}


IdleThread::~IdleThread() {
	LOCK
	done = 1;
	UNLOCK
}


void IdleThread::run() {
	while (!done) {
		//lockflag = 0;
		//lockCout
		//cout << "IDLE. Number of threads is " << PCB::numberOfActiveThreads() << endl;
		//unlockCout
		//lockflag = 1;
		//for (int i = 0; i < 30000; i++)
		//	for (int j = 0; j < 30000; j++);
	}
}


void IdleThread::start() {
	LOCK
	myPCB->status = IDLE;
	UNLOCK
}

