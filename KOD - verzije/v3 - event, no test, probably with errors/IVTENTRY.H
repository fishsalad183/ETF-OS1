#ifndef IVTENTRY_H_
#define IVTENTRY_H_


#include "TIMER.H"	// za pInterrupt
#include "EVENT.H"	// za IVTNo


class KernelEv;


class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newRoutine);

	void callOldRoutine();

	void signalLinkedEvent();

	static IVTEntry* getIVTEntryByEntryNo(IVTNo ivtNo);
	static IVTEntry* IVTEntryArray[256];

protected:
	friend class KernelEv;

	void initialize(KernelEv* eventToLink);
	void restore();

private:
	IVTNo entryNo;

	pInterrupt oldRoutine;

	KernelEv* linkedEvent;
};



#endif /* IVTENTRY_H_ */
