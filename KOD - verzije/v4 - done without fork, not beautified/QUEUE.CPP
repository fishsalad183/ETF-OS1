#include "QUEUE.H"
#include "DEFS.H"
#include "PCB.H"
#include "SCHEDULE.H"



Elem::Elem(PCB* p, Elem* n, Time t) : pcb(p), next(n), timeLeft(t) {}


Queue::Queue() {
	first = last = 0;
}


Queue::~Queue() {
	LOCK
	while (first) {

		// oslobadjanje cekajucih procesa -- da li da bude realizovano na drugim mestima (unutar destruktora)?
		first->pcb->status = READY;
		Scheduler::put(first->pcb);

		Elem* old = first;
		first = first->next;
		delete old;
	}
	first = last = 0;
	UNLOCK
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

	// podesava koliko dugo moze da ceka
	if (maxTimeToWait == 0) {
		maxTimeToWait = infiniteTimeSlice;
	}
	else if (maxTimeToWait > maximumTimeSlice) {
		maxTimeToWait = maximumTimeSlice;
	}

	if (first == 0) {	// umetanje u prazan prioritetni red
		first = last = new Elem(p, 0, maxTimeToWait);
	}
	else {
		Elem *prev = 0, *cur = first;
		while (cur != 0 && maxTimeToWait > cur->timeLeft) {		// koristi se > umesto >= jer je brze, a nema potrebe za razlikom
			prev = cur;
			cur = cur->next;
		}
		if (prev == 0) {	// umetanje na pocetak prioritetnog reda
			first = new Elem(p, first, maxTimeToWait);
		}
		else if (cur == 0) {	// umetanje na kraj prioritetnog reda
			Elem* newElem = new Elem(p, 0, maxTimeToWait);
			last->next = newElem;
			last = newElem;
		}
		else {		// umetanje u sredinu prioritetnog reda
			prev->next = new Elem(p, cur, maxTimeToWait);
		}
	}

	UNLOCK
}


PCB* Queue::get() {
	if (first) {
		LOCK
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
	return 0;
}



