#include <iostream.h>
#include "DEFS.H"
#include "TIMER.H"
#include "IDLE.H"
#include "thread.h"



IdleThread* idle;
Thread* starting;



int return_value;

extern int userMain(int argc, char* argv[]);


class UserMainThread : public Thread {
public:
	~UserMainThread();
	UserMainThread(int argc, char* argv[], StackSize ss = maximumStackSize, Time ts = 0);	// has no time limit by default!
protected:
	void run();
private:
	int arg1;
	char** arg2;
};

void UserMainThread::run()  {
	return_value = userMain(arg1, arg2);
}

UserMainThread::UserMainThread(int a1, char** a2, StackSize ss, Time ts) : Thread(ss, ts) {
	arg1 = a1; arg2 = a2;
}

UserMainThread::~UserMainThread() {
	waitToComplete();
}




int main(int argc, char* argv[]) {
	initialize();

	UserMainThread* umt = new UserMainThread(argc, argv);
	umt->start();
	delete umt;	// will waitToComplete()

	restore();

	cout << "\nmain has returned " << return_value << endl;
	return return_value;
}
