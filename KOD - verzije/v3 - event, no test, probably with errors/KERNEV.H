#ifndef KERNEV_H_
#define KERNEV_H_


#include "EVENT.H"


class PCB;


class KernelEv {
public:
	KernelEv(PCB* pcb, IVTNo ivtNo);
	~KernelEv();

	void wait();

protected:
	friend class IVTEntry;

	void signal();

private:
	int val;

	IVTEntry* myEntry;

	PCB* creator;
};



#endif /* KERNEV_H_ */
