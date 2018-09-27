#include <iostream.h>
#include "TIMER.H"
#include "THREAD.H"
#include "IDLE.H"
#include "PCB.H"
#include "MACRO.H"



IdleThread* idle;
Thread* starting;


int return_value;

extern int userMain(int argc, char* argv[]);


class UserMainThread : public Thread {
public:
	~UserMainThread();
	UserMainThread(int argc, char* argv[], StackSize ss = maximumStackSize, Time ts = 0);	// po difoltu nema vremensko ogranicenje!
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




extern volatile unsigned int context_change_requested;
extern volatile int lockflag;


int main(int argc, char* argv[]) {


	initialize();


	// podesiti parametre po potrebi prilikom kreiranja UserMainThread
	UserMainThread* umt = new UserMainThread(argc, argv);
	umt->start();


	lockCout
	cout << "about to delete umt" << endl;
	unlockCout


	delete umt;


	restore();


	// privremeno
		for (int k = 0; k < 15; k++) {
			lockflag = 0;
			cout << "MAIN" << k << endl;
			lockflag = 1;
			for (int i = 0; i < 10000; ++i)
				for (int j = 0; j < 30000; ++j);
		}


	cout << "system main has returned " << return_value << endl;

	return return_value;
}
