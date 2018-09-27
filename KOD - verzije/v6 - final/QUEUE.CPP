#include "QUEUE.H"
#include "DEFS.H"
#include "PCB.H"
#include "SCHEDULE.H"



Elem::Elem(PCB* p, Elem* n, Time t) : pcb(p), next(n), timeLeft(t) {}


Queue::Queue() {
	// LOCK/UNLOCK probably unnecessary, as Queue is always constructed during the construction of some other class, which is locked
	first = last = 0;
}


Queue::~Queue() {
	LOCK
	while (first) {

		// "setting free" waiting processes
		first->pcb->status = READY;
		Scheduler::put(first->pcb);

		Elem* old = first;
		first = first->next;
		delete old;
	}
	UNLOCK
	first = last = 0;
}


void Queue::put(PCB* p) {
	LOCK

	Elem* newElem = new Elem(p, 0);

	if (last == 0) {
		first = last = newElem;
	}
	else {
		last->next = newElem;
		last = newElem;
	}

	UNLOCK
}


void Queue::priority_put(PCB* p, Time maxTimeToWait) {
	LOCK

	if (maxTimeToWait == 0) {
		maxTimeToWait = infiniteTimeSlice;
	}
	else if (maxTimeToWait > maximumTimeSlice) {
		maxTimeToWait = maximumTimeSlice;
	}

	if (first == 0) {	// inserting into an empty priority queue
		first = last = new Elem(p, 0, maxTimeToWait);
	}
	else {
		Elem *prev = 0, *cur = first;
		while (cur != 0 && maxTimeToWait > cur->timeLeft) {		// > used instead of >= (faster, >= is not necessary)
			prev = cur;
			cur = cur->next;
		}
		if (prev == 0) {	// inserting into a priority queue at the beginning
			first = new Elem(p, first, maxTimeToWait);
		}
		else if (cur == 0) {	// inserting into a priority queue at the end
			// last = last->next = new Elem(p, 0, maxTimeToWait);	// more efficient, but compiler bugs?
			Elem* newElem = new Elem(p, 0, maxTimeToWait);
			last->next = newElem;
			last = newElem;
		}
		else {		// inserting into the middle of a priority queue
			prev->next = new Elem(p, cur, maxTimeToWait);
		}
	}

	UNLOCK
}


PCB* Queue::get() {
	LOCK
	if (first) {
		PCB* ret = first->pcb;
		Elem* old = first;
		first = first->next;
		if (first == 0) {
			last = 0;
		}
		UNLOCK
		delete old;
		return ret;
	}
	UNLOCK
	return 0;
}



