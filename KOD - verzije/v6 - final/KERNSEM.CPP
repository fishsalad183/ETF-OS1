#include "KERNSEM.H"
#include "DEFS.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "QUEUE.H"



SemaphoreListElem* KernelSem::SemaphoreListFirst = 0;

SemaphoreListElem::SemaphoreListElem(KernelSem* s, SemaphoreListElem* n) : sem(s), next(n) {}



// LOCKs and UNLOCKs are not needed in the functions below. The wrapper class Semaphore does that before invoking these.



KernelSem::KernelSem(int init) {
	value = init;

	valueIncrementedByExpiration = 0;

	myQueue = new Queue();

	// The new semaphore is placed at the beginning of the list because it is faster and doing otherwise is unnecessary.
	KernelSem::SemaphoreListFirst = new SemaphoreListElem(this, KernelSem::SemaphoreListFirst);
}



int KernelSem::wait(Time maxTimeToWait) {	// blocking of PCBs is integrated into function
	if (--value < 0) {
		PCB::running->status = BLOCKED;
		myQueue->priority_put((PCB*) PCB::running, maxTimeToWait);	// priority_put !
		dispatch();
	}

	if (PCB::running->unblockedBySignal == 1) {
		PCB::running->unblockedBySignal = 0;
		return 1;
	}
	else {
		return 0;
	}
}



void KernelSem::signal() {	// unblocking of PCBs is integrated into function
	if (valueIncrementedByExpiration == 0) {
		if (value++ < 0) {
			PCB* pcb = myQueue->get();
			pcb->status = READY;
			pcb->unblockedBySignal = 1;
			Scheduler::put(pcb);
		}
	}
	else {
		--valueIncrementedByExpiration;
	}
}



KernelSem::~KernelSem() {
	delete myQueue;	// Queue destructor puts all waiting PCBs into Scheduler.

	// unlinking from list
	// The list has at least 1 element because one is added whenever an object is created and destruction is paired with creation.
	if (this == KernelSem::SemaphoreListFirst->sem) {
		SemaphoreListElem *old = KernelSem::SemaphoreListFirst;
		KernelSem::SemaphoreListFirst = KernelSem::SemaphoreListFirst->next;
		delete old;
	}
	else {
		SemaphoreListElem *prev = KernelSem::SemaphoreListFirst, *cur = KernelSem::SemaphoreListFirst->next;
		while (cur->sem != this) {
			prev = cur;
			cur = cur->next;
		}
		prev->next = cur->next;
		delete cur;
	}
}



void KernelSem::timerInform() {
	// LOCK/UNLOCK not necessary: this function is invoked in void interrupt timer(), an interrupt routine.

	for (SemaphoreListElem* cur = KernelSem::SemaphoreListFirst; cur != 0; cur = cur->next) {
		Queue* q = cur->sem->myQueue;	// This function is a friend function to Queue so it has access rights to private members.
		// Keep in mind that *myQueue is sorted: shortest waiting PCBs are closest to the beginning, those without a time limit are at the end!
		for (Elem* e = q->first; e != 0 && e->timeLeft != infiniteTimeSlice; /*advancing through the queue is done below*/) {
			if (--e->timeLeft == 0) {
				cur->sem->value++;
				cur->sem->valueIncrementedByExpiration++;
				PCB* pcb = q->get();
				pcb->status = READY;	// whereas pcb->unblockedBySignal is 0 by default
				Scheduler::put(pcb);
				e = q->first;	// because q->get() has deleted the previous first element
			}
			else {
				e = e->next;
			}
		}
	}
}
