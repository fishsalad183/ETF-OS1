#include "PCB.H"
#include "SCHEDULE.H"
#include <dos.h>
#include "QUEUE.H"
#include "DEFS.H"
#include <iostream.h>



volatile PCB* PCB::running = 0;
PCB* PCB::PCBListFirst = 0;
PCB* PCB::PCBListLast = 0;



PCB::PCB(Thread* myThr, StackSize stSz, Time tSlice) {
	LOCK

	if (tSlice == 0) {
		timeSlice = infiniteTimeSlice;
	}
	else if (tSlice > maximumTimeSlice) {
		timeSlice = maximumTimeSlice;
		cout << "TIME SLICE GREATER THAN MAXIMUM! TIME SLICE SET TO MAXIMUM." << endl;
		asm cli;	// in case cout caused sti
	}
	else {
		timeSlice = tSlice;
	}


	status = NEW;
	myThread = myThr;
	myWaitQueue = new Queue();
	ss = sp = bp = 0;
	unblockedBySignal = 0;


	if (stSz >= minimumStackSize && stSz <= maximumStackSize) {
		stackSize = stSz;
	}
	else {
		stackSize = maximumStackSize;
		cout << "BAD STACK SIZE! STACK SIZE SET TO MAXIMUM." << endl;
		asm cli;	// in case cout caused sti
	}

	stackSize /= sizeof(unsigned);	// unsigned is 2B and stackSize is in bytes

	stack = new unsigned[stackSize];
	// SLIPPERY ZONE
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 3] = 0x200;
	stack[stackSize - 4] = FP_SEG(PCB::wrapper);
	stack[stackSize - 5] = FP_OFF(PCB::wrapper);
	ss = FP_SEG(stack + stackSize - 14);
	sp = FP_OFF(stack + stackSize - 14);
	bp = sp;
#endif


	// linking into list
	if (PCBListFirst == 0) {
		PCBListFirst = PCBListLast = this;
	}
	else {
		PCBListLast->next = this;
		PCBListLast = this;
	}

	UNLOCK
}



PCB::~PCB() {
	LOCK

	delete stack;

	delete myWaitQueue;	// Queue destructor "sets free" remaining PCBs (in case something unexpected occured)

	// unlinking from list
	if (this == PCBListFirst) {
		PCBListFirst = PCBListFirst->next;
		if (PCBListFirst == 0) {
			PCBListLast = 0;
		}
	}
	else {
		PCB* prev = PCBListFirst;
		while (prev->next != this) {
			prev = prev->next;
		}
		prev->next = this->next;
		if (this == PCBListLast) {
			PCBListLast = prev;
		}
	}

	UNLOCK
}



void PCB::wrapper() {
	PCB::running->myThread->run();

	LOCK

	// since this process is finishing, no process may wait for it anymore
	PCB* pcb;
	while ((pcb = PCB::running->myWaitQueue->get()) != 0) {
		pcb->status = READY;
		Scheduler::put(pcb);
	}

	PCB::running->status = FINISHED;

	dispatch();

	UNLOCK	// although unnecessary due to dispatch() and FINISHED
}

