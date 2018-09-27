#include "IVTENTRY.H"
#include "KERNEV.H"
#include <dos.h>


IVTEntry* IVTEntry::IVTEntryArray[] = { 0 };


IVTEntry* IVTEntry::getIVTEntryByEntryNo(IVTNo ivtNo) {
	if (ivtNo >= 0 && ivtNo <= 255) {
		return IVTEntryArray[ivtNo];
	}
	else {
		return 0;
	}
}


IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRout) {
	entryNo = ivtNo;
	oldRoutine = 0;
	newRoutine = newRout;
	linkedEvent = 0;
	IVTEntry::IVTEntryArray[entryNo] = this;
}


IVTEntry::~IVTEntry() {
	IVTEntry::IVTEntryArray[entryNo] = 0;
	// No need to call restore() here because it will be called during the destruction of KernelEv
	// which will occur before the destruction of IVTEntry (PREPAREENTRY must be done before creating an event).
}


void IVTEntry::callOldRoutine() {
	if (oldRoutine != 0) {
		(*oldRoutine)();
	}
}


void IVTEntry::signalLinkedEvent() {
	linkedEvent->signal();
}


void IVTEntry::initialize(KernelEv* eventToLink) {
	if (oldRoutine == 0) {	// protects from being called multiple times
		linkedEvent = eventToLink;
#ifndef BCC_BLOCK_IGNORE
		oldRoutine = getvect(entryNo);
		setvect(entryNo, newRoutine);
#endif
	}
}


void IVTEntry::restore() {
	if (oldRoutine != 0) {	// protects from being called multiple times
#ifndef BCC_BLOCK_IGNORE
		setvect(entryNo, oldRoutine);
#endif
		oldRoutine = 0;
	}
}

