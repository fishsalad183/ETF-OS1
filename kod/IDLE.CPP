#include "IDLE.H"
#include "DEFS.H"
#include "PCB.H"



IdleThread::IdleThread() : Thread(minimumStackSize, 1) {
	LOCK
	done = 0;
	myPCB->status = NEW;
	UNLOCK
}


IdleThread::~IdleThread() {
	done = 1;
}


void IdleThread::run() {
	while (done <= 0) {
		--done;
		++done;
	}
}


void IdleThread::start() {
	LOCK
	myPCB->status = IDLE;
	UNLOCK
}

