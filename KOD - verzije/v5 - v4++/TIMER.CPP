#include <dos.h>
#include "TIMER.H"
#include "DEFS.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "IDLE.H"
#include "KERNSEM.H"
#include "thread.h"
//#include <iostream.h>	// for timer diagnostics



extern void tick();

extern IdleThread* idle;
extern Thread* starting;



volatile unsigned int lockflag = 1;

volatile int context_change_requested = 0;	// equals 1 when context change could not be executed due to lockflag being set to 0
volatile int explicit_call = 0;				// denotes that timer(...) is invoked by dispatch()

volatile Time counter = defaultTimeSlice;	// what to initialize with?

unsigned tsp;
unsigned tss;
unsigned tbp;


void interrupt timer(...) {

	// this occurs when timer is called "naturally", every 55ms and not by dispatch()
	if (! explicit_call) {
		KernelSem::timerInform();	// informs all semaphores of passing time
		if (counter != infiniteTimeSlice && counter != 0) counter--;
		tick();
		asm int 60h;	// calls the old routine
	}

	if (explicit_call || context_change_requested || counter == 0) {
		if (lockflag) {
			context_change_requested = 0;


			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}


			int runningStatus = PCB::running->status;

// TIMER DIAGNOSTICS
/*			cout << "CONTEXT CHANGE! COUNTER, STATUS, ID = " << counter << ", " << runningStatus << ", " << Thread::getRunningID() << endl;
			asm cli;	// in case cout caused sti
*/

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;


			if (runningStatus == RUNNING /*&& PCB::running != idle*/) {
				// RUNNING  !=  IDLE, BLOCKED, FINISHED
				PCB::running->status = READY;
				Scheduler::put((PCB*) PCB::running);
			}
			PCB::running = Scheduler::get();
			if (PCB::running == 0) {
				PCB::running = idle->myPCB;
			}
			else {
				PCB::running->status = RUNNING;
			}


			counter = PCB::running->timeSlice;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		}

		else context_change_requested = 1;
	}

	explicit_call = 0;
}



void initialize() {
	LOCK
#ifndef BCC_BLOCK_IGNORE
	pInterrupt oldRoutine = getvect(0x8);
	setvect(0x60, oldRoutine);
	setvect(0x8, timer);
#endif

	starting = new Thread();	// starting thread - represents main function
	starting->myPCB->status = RUNNING;
	PCB::running = starting->myPCB;

	idle = new IdleThread();
	idle->start();

	UNLOCK
}



void restore() {
	if (PCB::running != starting->myPCB) return;	// in case some other thread calls this function

	LOCK
#ifndef BCC_BLOCK_IGNORE
	pInterrupt oldRoutine = getvect(0x60);
	setvect(0x8, oldRoutine);
#endif
	// no more context-changing periodic interrupts

	delete idle;
	delete starting;

	UNLOCK
}
