#ifndef KERNEV_H_
#define KERNEV_H_


typedef unsigned char IVTNo;

class PCB;
class IVTEntry;


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
