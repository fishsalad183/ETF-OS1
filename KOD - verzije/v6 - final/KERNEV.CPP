#include "KERNEV.H"
#include "IVTENTRY.H"
#include "DEFS.H"
#include "thread.h"
#include "PCB.H"
#include "SCHEDULE.H"



// LOCKs and UNLOCKs are not needed in the functions below.
// The wrapper class Event does that before invoking all but signal().
// signal() is invoked in an interrupt routine.



KernelEv::KernelEv(PCB* pcb, IVTNo ivtNo) {
	creator = pcb;
	val = 1;
	myEntry = IVTEntry::IVTEntryArray[ivtNo];
	myEntry->initialize(this);	// WILL CAUSE AN ERROR IF PREPAREENTRY HAS NOT BEEN DONE
}


KernelEv::~KernelEv() {
	KernelEv::signal();	// so that no thread remains blocked if this event is deleted
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
	if (val == 0) {
		val = 1;
		creator->status = READY;
		Scheduler::put((PCB*) creator);
		dispatch();
	}
}
