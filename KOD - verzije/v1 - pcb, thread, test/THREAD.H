#ifndef THREAD_H_
#define THREAD_H_


// SVE JE U BAJTOVIMA
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
const StackSize minimumStackSize = 512;
const StackSize maximumStackSize = 65535;	// maksimalna velicina steka je 65535 B

typedef unsigned int Time;	// time, x 55ms
const Time defaultTimeSlice = 2;	// default = 2*55ms
const Time maximumTimeSlice = 65534;	// ne moze vise od ovoga da se prosledi kao parametar
const Time infiniteTimeSlice = 65535;	// koristi se kao vrednost za neograniceno
typedef int ID;


class PCB;
class IdleThread;	//?


class Thread {
public:

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getID() const;
	static ID getRunningID();
	static Thread* getThreadByID(ID id);

protected:
	friend class PCB;
	friend class IdleThread;	//?
	friend class UserMainThread;	//?
	friend void interrupt timer(...);
	friend void initialize();
	friend void restore();

	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;

	static ID prevID;
	ID id;


#ifndef BCC_BLOCK_IGNORE
	Thread(const Thread&) {}	// zabrana kopiranja, moze li ovako?
#endif
};



void dispatch();



#endif /* THREAD_H_ */
