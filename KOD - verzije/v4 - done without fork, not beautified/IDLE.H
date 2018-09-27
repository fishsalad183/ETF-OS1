#ifndef IDLE_H_
#define IDLE_H_


#include "thread.h"



class IdleThread : public Thread {
public:
	IdleThread();
	~IdleThread();
	void start();	// start() in Thread is not virtual - this method only shadows it!

protected:
	friend void interrupt timer(...);

	void run();

private:
	unsigned int done;

};


#endif /* IDLE_H_ */
