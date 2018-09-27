//pretpostavljeni memorijski model: huge

#include <iostream.h>
#include <dos.h>
#include "DEFS.H"
#include "SCHEDULE.H"
#include "PCB.H"
#include "THREAD.H"
#include "TIMER.H"
#include "SEMAPHOR.H"


// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti


//#define lockCout lockflag=0;

/*#define unlockCout lockflag=1;\
		if (zahtevana_promena_konteksta) {\
				dispatch();\
		}*/

/*class PCB{
public:
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned zavrsio;
	int kvant;
};*/


//typedef void interrupt (*pInterrupt)(...);


//PCB *p[3];
//volatile PCB* running;

/*volatile int nextThread = 2;
PCB* getNextPCBToExecute(){
	if (nextThread == 1)
		nextThread = 2;
	else nextThread = 1;
	if (p[nextThread]->zavrsio){
		if (nextThread == 1)
			nextThread = 2;
		else nextThread = 1;
		if (p[nextThread]->zavrsio)
			nextThread = 0;
	}
	return p[nextThread];
}*/


//pomocne promenljive za prekid tajmera
/*unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int brojac = 20;
volatile int zahtevana_promena_konteksta = 0;

volatile unsigned int lockflag = 1;*/

/*void interrupt timer(...){	// prekidna rutina

	int dummy = 0;

	if (!zahtevana_promena_konteksta) brojac--; 
	if (brojac == 0 || zahtevana_promena_konteksta) {
			if (lockflag) {
				zahtevana_promena_konteksta = 0;

				asm {
					// cuva sp
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}


				lockflag = 0;
				cout << "Promena konteksta! Brojac = " << brojac << endl;
				asm cli;
				lockflag = 1;

				running->sp = tsp;
				running->ss = tss;
				running->bp = tbp;

				//running= getNextPCBToExecute();	// Scheduler

				if (! running->zavrsio) Scheduler::put((PCB*) running);
				running = Scheduler::get();

				tsp = running->sp;
				tss = running->ss;
				tbp = running->bp;

				brojac = running->kvant;


				asm {
					mov sp, tsp   // restore sp
					mov ss, tss
					mov bp, tbp
				}

			}
			else zahtevana_promena_konteksta = 1;

		}
	// poziv stare prekidne rutine koja se 
    // nalazila na 08h, a sad je na 60h
    // poziva se samo kada nije zahtevana promena
    // konteksta – tako se da se stara
    // rutina poziva samo kada je stvarno doslo do prekida
	if (!zahtevana_promena_konteksta) asm int 60h;
		                                              
	//zahtevana_promena_konteksta = 0;
}*/

/*void dispatch(){ // sinhrona promena konteksta
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}*/

//unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina


// postavlja novu prekidnu rutinu
/*void inic(){
#ifndef BCC_BLOCK_IGNORE
	asm cli
	pInterrupt oldRoutine = getvect(0x8);
	setvect(0x8, timer);
	setvect(0x60, oldRoutine);
	asm sti*/
/*	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax	
		mov ax, word ptr es:0020h	
		mov word ptr oldTimerOFF, ax	

		mov word ptr es:0022h, seg timer	 //postavlja 
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu	
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}*/
/*#endif
}*/


// vraca staru prekidnu rutinu
/*void restore(){
#ifndef BCC_BLOCK_IGNORE
	asm cli
	pInterrupt oldRoutine = getvect(0x60);
	setvect(0x8, oldRoutine);
	asm sti*/
/*	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}*/
/*#endif
}*/


/*void exitThread(){
	running->zavrsio = 1;
	dispatch();
}*/

/*
class B : public Thread {
protected:
	void run() {
		for (int i =0; i < 30; ++i) {

				lockCout
				cout<<"u b() i = "<<i<<endl;
				unlockCout

				for (int k = 0; k<10000; ++k)
					for (int j = 0; j <30000; ++j);
			}
	}
public:
	B(StackSize ss = defaultStackSize, Time ts = defaultTimeSlice) : Thread(ss, ts) {}
	~B() { waitToComplete(); }
};
*/

/*void a(){
	for (int i =0; i < 30; ++i) {

		lockCout
		cout<<"u a() i = "<<i<<endl;
		unlockCout

		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}*/



/*void b(){
	for (int i =0; i < 30; ++i) {

		lockCout
		cout<<"u b() i = "<<i<<endl;
		unlockCout

		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}*/


/*
void createProcess(PCB *newPCB, void (*body)()){
	unsigned* st1 = new unsigned[1024];

	st1[1023] =0x200;//setovan I fleg u
                      // pocetnom PSW-u za nit
#ifndef BCC_BLOCK_IGNORE
	st1[1022] = FP_SEG(body);
	st1[1021] = FP_OFF(body);

	newPCB->sp = FP_OFF(st1+1012);	//svi sacuvani registri
									//pri ulasku u interrupt
									//rutinu
	newPCB->ss = FP_SEG(st1+1012);
	newPCB->bp = FP_OFF(st1+1012);
#endif
	newPCB->zavrsio = 0;
}*/



/*void doSomething(){
	lock;
	Thread* a = new A(defaultStackSize, 40);
	a->start();
	cout<<"napravio a"<<endl;

	Thread* b = new B(defaultStackSize, 20);
	b->start();
	cout<<"napravio b"<<endl;



	unlock;

	for (int i = 0; i < 30; ++i) {
  	lock;
  	cout<<"main "<<i<<endl;
  	unlock;

	for (int j = 0; j< 30000; ++j)
		for (int k = 0; k < 30000; ++k);
	}
	cout<<"Happy End"<<endl;
}*/


/*int main(){

  initialize();

  doSomething();

  restore();

  return 0;
}*/






extern volatile int context_change_requested;
extern volatile unsigned int lockflag;


Semaphore s = Semaphore(1);
int ret1 = -1, ret2 = -1, ret3 = -1;


class TestThread : public Thread {
	char name;
protected:
	void run() {
		for (int i = 0; i < 30; ++i) {
			if (name == 'A') {
				if (i == 3) {
					lockCout
					cout << name << " WAIT " << s.val() << endl;
					unlockCout
					ret1 = s.wait(0);
					if (ret1 == 1) {
						lockCout
						cout << "A SIGNALLED" << endl;
						unlockCout
					}
					else {
						lockCout
						cout << "A TIME EXPIRED" << endl;
						unlockCout
					}
				}
				else if (i == 25) { s.signal(); }
			}
			else if (name == 'B') {
				if (i == 3) {
					lockCout
					cout << name << " WAIT " << s.val() << endl;
					unlockCout
					ret2 = s.wait(200);
					if (ret1 == 1) {
						lockCout
						cout << "B SIGNALLED" << endl;
						unlockCout
					}
					else {
						lockCout
						cout << "B TIME EXPIRED" << endl;
						unlockCout
					}
				}
				else if (i == 25) { s.signal(); }
			}
			else if (name == 'C') {
				if (i == 1 || i == 8) {
					lockCout
					cout << name << " WAIT " << s.val() << endl;
					unlockCout
					if (i == 1) ret3 = s.wait(0);
					else ret3 = s.wait(400);
					/*if (ret3 == 1) cout << "C SIGNALLED" << endl;
					else cout << "C TIME EXPIRED" << endl;*/
				}
				else if (i == 29 || i == 28) { s.signal(); }
			}

			lockCout
			cout << name << " " << i << endl;
			unlockCout

			for (int k = 0; k < 15000; ++k)
				for (int j = 0; j <30000; ++j);
		}
	}
public:
	TestThread(char n, StackSize ss = defaultStackSize, Time ts = defaultTimeSlice) : Thread(ss, ts) {
		name = n;
	}
	~TestThread() { waitToComplete(); }
	char getName() const { return name; }
};





int userMain(int argc, char* argv[]) {

	char name[] = { 'A', 'B', 'C' };
	Time slice[] = { 40, 30, 20 };
	TestThread* t[3];

	int i;

	for (i = 0; i < 3; ++i) {
		t[i] = new TestThread(name[i], defaultStackSize, slice[i]);
		t[i]->start();
		lockCout
		cout << t[i]->getName() << " created" << endl;
		unlockCout
	}


	for (i = 0; i < 5; ++i) {
		lockCout
		cout << "userMain " << i << endl;
		unlockCout
		for (int j = 0; j < 10000; ++j)
			for (int k = 0; k < 30000; ++k);
	}


	for (i = 0; i < 3; ++i) {
		lockCout
		cout << "about to delete " << t[i]->getName() << endl;
		unlockCout
		delete t[i];
	}

	lockCout
	cout << "userMain ended happily " << ret1 << " " << ret2 << " " << ret3 << endl;
	unlockCout

	return 0;
}


