#include "MACRO.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include <iostream.h>
#include <dos.h>



volatile PCB* PCB::running = 0;
volatile PCB* PCB::PCBListFirst = 0;
volatile PCB* PCB::PCBListLast = 0;
volatile unsigned int PCB::num = 0;



PCB::PCB(Thread* myThr, StackSize stSz, Time TimeSlice) {
	LOCK

	if (TimeSlice == 0) {
		timeSlice = infiniteTimeSlice;
	}
	else if (TimeSlice > maximumTimeSlice) {
		timeSlice = maximumTimeSlice;
		cout << "TIME SLICE GREATER THAN MAXIMUM! TIME SLICE SET TO MAXIMUM." << endl;	// dijagnosticki?
	}
	else {
		timeSlice = TimeSlice;
	}

	myThread = myThr;
	myWaitQueue = new Queue();
	status = NEW;
	ss = sp = bp = 0;

	if (stSz >= minimumStackSize && stSz <= maximumStackSize) {
		stackSize = stSz;
	}
	else {
		stackSize = maximumStackSize;
		cout << "BAD STACK SIZE! STACK SIZE SET TO MAXIMUM." << endl;	// dijagnosticki?
	}

	stackSize /= sizeof(unsigned);		// jer unsigned = 2B, a max je 64KB
	// TESTIRATI: da li moze da podnese max velicinu?

	//OPASNA ZONA:   --- komentarisati gde je potrebno
	stack = new unsigned[stackSize];
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 1] = FP_SEG(myThr);
	stack[stackSize - 2] = FP_OFF(myThr);
	stack[stackSize - 5] = 0x200;
	stack[stackSize - 6] = FP_SEG(PCB::wrapper);	//?
	stack[stackSize - 7] = FP_OFF(PCB::wrapper);	//?
	ss = FP_SEG(stack + stackSize - 16);
	sp = FP_OFF(stack + stackSize - 16);
	bp = sp;
#endif

	// ulancavanje u listu svih PCB-ova
	if (PCBListFirst == 0) {
		PCBListFirst = PCBListLast = this;
	}
	else {
		PCBListLast->next = this;
		PCBListLast = this;
	}

	PCB::num++;

	UNLOCK
}



PCB::~PCB() {
	LOCK

	if (stack != 0) delete stack;
	if (myWaitQueue != 0) delete myWaitQueue;

	// IZBACIVANJE IZ LISTE
	if (this == PCBListFirst) {
		PCBListFirst = PCBListFirst->next;
		if (PCBListFirst == 0) {
			PCBListLast = 0;
		}
	}
	else {
		PCB* prev = (PCB*) PCBListFirst;
		while (prev->next != this) {	// ne moze se desiti da ne postoji u listi
			prev = prev->next;
		}
		prev->next = this->next;
		if (this == PCBListLast) {
			PCBListLast = prev;
		}
	}

	// ne smanjuje se num jer se to radi kad prestane sa radom, a ne kad se brise

	UNLOCK
}



void PCB::wrapper() {
	PCB::running->myThread->run();

	LOCK

	PCB* pcb;
	while ((pcb = PCB::running->myWaitQueue->get()) != 0) {
		pcb->status = READY;
		Scheduler::put(pcb);
	}

	PCB::running->status = FINISHED;
	PCB::num--;	// smanjuje se broj aktivnih niti!

	dispatch();
	UNLOCK	// verovatno nece trebati zbog dispatch
}



unsigned int PCB::numberOfActiveThreads() {
	return PCB::num;
}

