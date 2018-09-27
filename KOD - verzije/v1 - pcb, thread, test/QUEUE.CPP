#include "MACRO.H"
#include "QUEUE.H"
#include "PCB.H"



Queue::Queue() {
	first = last = 0;
}


Queue::~Queue() {
	LOCK
	while (first) {
		Elem* old = first;
		first = first->next;
		delete old;
	}
	first = last = 0;
	UNLOCK
}


void Queue::put(PCB* p) {		// ZASTITA OD VISESTRUKOG UBACIVANJA?
	LOCK

	Elem* newElem = new Elem();
	newElem->pcb = p;
	newElem->next = 0;

	if (last == 0) {
		first = last = newElem;
	}
	else {
		last->next = newElem;
		last = newElem;
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
		delete old;		// BRISE SE STARI CLAN
		return ret;
	}
	return 0;
}



