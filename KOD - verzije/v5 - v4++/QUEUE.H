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
	void put(PCB* p);	// puts at the end of the queue, indefinite waiting time
	void priority_put(PCB* p, Time maxTimeToWait);	// positions depending on priority (time remaining)
	PCB* get();	// grabs from the beginning of the list - lowest time remaining

	Elem *first, *last;

	Queue();
	~Queue();

protected:
	friend void timerInform();	// for eficiency
};


#endif /* QUEUE_H_ */
