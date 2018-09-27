#ifndef EVENT_H_
#define EVENT_H_



#ifndef DEFS_H_
#define PREPAREENTRY(entryNo, callOldRout)\
		void interrupt newRoutine##entryNo(...);\
		IVTEntry ivtEntry##entryNo(entryNo, newRoutine);\
		void interrupt newRoutine##entryNo(...) {\
			ivtEntry##entryNo.signalLinkedEvent();\
			if (callOldRout == 1)\
				ivtEntry##entryNo.callOldRoutine();\
		}
#endif



typedef unsigned char IVTNo;
class KernelEv;


class Event {
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	//void signal();	// can call KernelEv

private:
	KernelEv* myImpl;
};



#endif /* EVENT_H_ */
