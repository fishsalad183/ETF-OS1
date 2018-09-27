#include <dos.h>
#include <iostream.h>	// radi dijagnostickog ispisa
#include "MACRO.H"
#include "TIMER.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "THREAD.H"
#include "IDLE.H"
#include "KERNSEM.H"



extern void tick();	// ZA TEST-OKRUZENJE!!!
#ifndef _OS1_USER_H_
void tick() {}
#endif



extern IdleThread* idle;
extern Thread* starting;



volatile unsigned int lockflag = 1;


volatile int context_change_requested = 0;	// pamti da je zahtevana promena, ali nije bila moguca zbog lockflag
volatile int explicit_call = 0;				// oznacava da je pozvan timer pomocu dispatch

volatile Time counter = 1;	// cime se inicijalizuje?

unsigned tsp;
unsigned tss;
unsigned tbp;


void interrupt timer(...) {

	KernelSem::timerInform();

	if (!explicit_call && counter != infiniteTimeSlice && counter != 0) counter--;	// PCB::running->timeSlice ili counter?

	if (counter == 0 || context_change_requested || explicit_call) {
		if (lockflag) {
			context_change_requested = 0;

			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}


			int runningStatus = PCB::running->status;


			// DIJAGNOSTICKI ISPIS!
			cout << "CONTEXT CHANGE! COUNTER, STATUS = " << counter << ", " << runningStatus << endl;
			asm cli;	// za slucaj da se unutar cout desi da se promeni


			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;


			if (runningStatus == RUNNING /*&& PCB::running != idle*/) {
				// ako je RUNNING, nije IDLE, nije BLOCKED, nije FINISHED
				// takodje
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

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			counter = PCB::running->timeSlice;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		}

		else context_change_requested = 1;
	}

	// prekidna rutina se poziva samo kad je stvarno doslo do prekida, na 55ms
	if (! explicit_call) {
		tick();		// OVA FUNKCIJA MORA DA SE POZOVE ZBOG TEST-OKRUZENJA --- OVDE???
		asm int 60h;
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

	starting = new Thread();	// pocetna nit - predstavlja main
	starting->myPCB->status = RUNNING;
	PCB::running = starting->myPCB;

	idle = new IdleThread();
	idle->start();

	UNLOCK
}



void restore() {
	if (PCB::running != starting->myPCB) return;	// u slucaju da se do ovde dodje iz neke druge niti
													// hmmm??

	LOCK
#ifndef BCC_BLOCK_IGNORE
	pInterrupt oldRoutine = getvect(0x60);
	setvect(0x8, oldRoutine);
#endif
	// VISE NEMA PREKIDA KOJI MENJAJU KONTEKST

	delete idle;
	delete starting;

	UNLOCK
}
