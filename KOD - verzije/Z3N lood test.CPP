#include <iostream.h>
#include "thread.h"
#include "DEFS.H"
#include "TIMER.H"
#include "semaphor.h"
#include "PCB.H"
#include <stdlib.h>	// for rand()
#include "event.h"


// SET userMain AS FRIEND TO THREAD FOR TESTING PURPOSES



PREPAREENTRY(0x1C, 1);



void tick() {}


extern volatile int context_change_requested;
extern volatile unsigned int lockflag;





class TestThread1 : public Thread {
public:
	Semaphore* s;

	~TestThread1() { waitToComplete(); }

	TestThread1(Semaphore* sem, StackSize ss = defaultStackSize, Time ts = defaultTimeSlice) : Thread(ss, ts), s(sem) {
		lockCout
		cout << "Thread" << Thread::getID() << " created with time slice of " << ts << endl;
		unlockCout
	}

	void run() {
		int mustSignal = 0;

		for (int i = 0; i < 15; ++i) {
			Time waitTime;
			int x;

			lockCout
			cout << "Thread" << Thread::getRunningID() << ": i = " << i << flush;

			if ((x = rand() % 20) >= 0 && x <= 6) {
				cout << ", waiting " << (waitTime = (Time) rand() % 183 + 3) << endl;
				unlockCout
				++mustSignal;
				LOCK_INTE
				if (s->val() > 0) {
					cout << "Thread" << getID() << " does not stop at semaphore." << endl;
					s->wait(waitTime);
				}
				else {
					int c = s->wait(waitTime);
					if (c == 1) {
						cout << "Thread" << getID() << " signalled to continue!" << endl;
					}
					else {
						cout << "Thread" << Thread::getThreadByID(getID())->getID() << " waiting time expired!" << endl;
					}
				}
				UNLOCK_INTE
			}
			else if (x >= 7 && x <= 12) {
				cout << ", signalling" << endl;
				--mustSignal;
				s->signal();
				unlockCout
			}
			else {
				cout << endl;
				unlockCout
			}

			for (int j = 0; j < 10000; ++j)
				for (int k = 0; k < 20000; ++k);

			if (i == 29) {
				lockCout
				cout << "Thread" << getID() << " complete!" << endl;
				unlockCout
			}
		}

		if (mustSignal > 0) {
			lockCout
			cout << "Thread" << Thread::getID() << " signalling " << mustSignal << " times" << endl;
			unlockCout
			while (mustSignal-- > 0) s->signal();
		}
	}
};



class TestThread2 : public Thread {
public:
	Semaphore* s;

	~TestThread2() { waitToComplete(); }

	TestThread2(Semaphore* sem) : Thread(minimumStackSize, 0), s(sem) {
		lockCout
		cout << "Thread" << getID() << " created" << endl;
		unlockCout
	}

	void run() {
		unsigned int x = 1;
		unsigned int N = 18;
		lockCout
		cout << "Thread" << getID() << "signalling if x is 0. Chances: 1 in " << N << endl;
		unlockCout
		while (x != 0) {
			x = rand() % N;
			lockCout
			cout << "x = " << x << endl;
			unlockCout
			for (int j = 0; j < 10000; ++j)
				for (int k = 0; k < 20000; ++k);
		}
		lockCout
		cout << "Thread " << getRunningID() << " signalling!" << endl;
		unlockCout
		s->signal();
	}
};



class TestThread3 : public Thread {
public:
	~TestThread3() { waitToComplete(); }

	void run() {
		Event ev(0x1C);
		for (int i = 0; i < 43; ++i) {
			ev.wait();
			ev.wait();
			lockCout
			cout << "TICK " << i << endl;
			unlockCout
		}
	}
};





int userMain(int argc, char* argv[]) {
	int i;

	Semaphore s1(2);
	const int N = 4;
	TestThread1* tt1[N];

	for (i = 0; i < N; ++i) {
		LOCK
		tt1[i] = new TestThread1(&s1, defaultStackSize, (Time) rand() % 41);
		UNLOCK
		tt1[i]->start();
	}


	for (i = 0; i < 5; ++i) {
		Thread* t;
		lockCout
		cout << "-- userMain " << i << flush;
		if ((t = Thread::getThreadByID((ID) i)) != 0) cout << "   |||   Thread" << t->getID() << " status number is " << t->myPCB->status << endl;
		else cout << endl;
		unlockCout
		for (int j = 0; j < 10000; ++j)
			for (int k = 0; k < 30000; ++k);
	}


	for (i = 0; i < N; ++i) {
		lockCout
		cout << "----- about to delete Thread" << tt1[i]->getID() << endl;
		unlockCout
		delete tt1[i];
	}

	for (int j = 0; j < 20000; ++j)
		for (int k = 0; k < 20000; ++k);

	Semaphore* s2 = new Semaphore(0);
	LOCK
	Thread* tt2 = new TestThread2(s2);
	tt2->start();
	UNLOCK
	lockCout
	cout << "\nuserMain Thread (ID: " << Thread::getRunningID() << ") waiting indefinitely at semaphore!" << endl;
	unlockCout
	s2->wait(0);
	delete tt2;
	delete s2;

	Thread* tt3 = new TestThread3();
	tt3->start();
	delete tt3;

	lockCout
	cout << "\n----- userMain ended happily " << endl;
	unlockCout

	return 0;
}


