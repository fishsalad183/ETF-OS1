#ifndef THREAD_H_
#define THREAD_H_


// sizes in bytes
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
const StackSize minimumStackSize = 512;
const StackSize maximumStackSize = 65536;	// 65535? 65536 may (or may not) cause an error on 8086

typedef unsigned int Time;	// time, x 55ms
const Time defaultTimeSlice = 2;	// default = 2*55ms
const Time maximumTimeSlice = 65534;	// anything greater will not be accepted
const Time infiniteTimeSlice = 65535;	// means that a thread has no time limit

typedef int ID;


class PCB;


class Thread {
public:

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getID();
	static ID getRunningID();
	static Thread* getThreadByID(ID id);

protected:
	friend class PCB;

	friend class IdleThread;
	friend void interrupt timer(...);
	friend void initialize();
	friend void restore();

	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;

	static ID prevID;
	ID id;
};



void dispatch();



#endif /* THREAD_H_ */
