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
	// no need to call restore() here because:
	// 1) it will be called in the destructor of KernelEv
	// 2) PREPAREENTRY should be written in the beginning of the code, so KernEv destructors get called before IVTEntry ones
}


void IVTEntry::callOldRoutine() {
	if (oldRoutine != 0) {
#ifndef BCC_BLOCK_IGNORE
		(*oldRoutine)();
#endif
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

