#ifndef QUEUE_H_
#define QUEUE_H_


#include "thread.h"	// for definitions of Time, infiniteTimeSlice etc.



class PCB;


struct Elem {
	PCB* pcb;
	Elem* next;
	Time timeLeft;

	Elem(PCB* p = 0, Elem* n = 0, Time t = infiniteTimeSlice);
};


class Queue {
public:
	void put(PCB* p);	// stavlja na kraj reda, cekaju neograniceno
	void priority_put(PCB* p, Time maxTimeToWait);	// stavlja u zavisnosti od prioriteta - preostalog vremena
	PCB* get();

	Elem *first, *last;

	Queue();
	~Queue();

protected:
	friend void timerInform();	// da bi bilo mnogo efikasnije

private:
#ifndef BCC_BLOCK_IGNORE
	Queue(const Queue&) {}	// zabrana kopiranja
#endif
};


#endif /* QUEUE_H_ */
