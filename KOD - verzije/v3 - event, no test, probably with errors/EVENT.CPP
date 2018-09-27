#include "EVENT.H"
#include "DEFS.H"


Event::Event(IVTNo ivtNo) {
	LOCK
	myImpl = new KernelEv((PCB*) PCB::running, ivtNo);
	UNLOCK
}


Event::~Event() {
	LOCK
	delete myImpl;
	UNLOCK
}


void Event::wait() {
	LOCK
	myImpl->wait();
	UNLOCK
}
