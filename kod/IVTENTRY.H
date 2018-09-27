#ifndef IVTENTRY_H_
#define IVTENTRY_H_



typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);


class KernelEv;


class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newRoutine);
	~IVTEntry();

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
	pInterrupt newRoutine;

	KernelEv* linkedEvent;
};



#endif /* IVTENTRY_H_ */
