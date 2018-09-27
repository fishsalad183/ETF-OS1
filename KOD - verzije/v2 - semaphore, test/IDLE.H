#ifndef IDLE_H_
#define IDLE_H_


#include "THREAD.H"



class IdleThread : public Thread {
public:
	IdleThread();
	~IdleThread();
	void start();	// start nije virtuelna metoda - ova deklaracija je samo sakriva!

protected:
	friend void interrupt timer(...);

	void run();

private:
	int done;

	//void Thread::start(); - onemoguciti da se pozove?!
};


#endif /* IDLE_H_ */
