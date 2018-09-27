#include "KERNEV.H"
#include "DEFS.H"
#include "THREAD.H"



// With the exception of signal() (because it is not wrapped in Event), LOCKs and UNLOCKs are not needed in the functions below. The wrapper class Event does that before invoking these.



KernelEv::KernelEv(PCB* pcb, IVTNo ivtNo) {
	creator = pcb;
	val = 1;
	myEntry = IVTEntry::IVTEntryArray[ivtNo];
	myEntry->initialize(this);	// WILL CAUSE AN ERROR IF PREPAREENTRY HAD NOT BEEN DONE
}


KernelEv::~KernelEv() {
	KernelEv::signal();	// so that no threads remain blocked if this event is deleted
	myEntry->restore();
}


void KernelEv::wait() {
	if (PCB::running == creator && val == 1) {
		val = 0;
		PCB::running->status = BLOCKED;	// thread being blocked (PCB::running) saved as this->creator
		dispatch();
	}
}


void KernelEv::signal() {
	LOCK
	if (val == 0) {
		val = 1;
		creator->status = READY;
		Scheduler::put((PCB*) creator);
		dispatch();
	}
	UNLOCK
}
