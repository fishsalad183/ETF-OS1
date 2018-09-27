#ifndef QUEUE_H_
#define QUEUE_H_


class PCB;


struct Elem {
	PCB* pcb;
	Elem* next;
};


class Queue {
public:
	void put(PCB* p);
	PCB* get();

	Elem *first, *last;

	Queue();
	~Queue();

private:
#ifndef BCC_BLOCK_IGNORE
	Queue(const Queue&) {}	// zabrana kopiranja
#endif
};


#endif /* QUEUE_H_ */
