#ifndef DEFS_H_
#define DEFS_H_



// MALA/VELIKA SLOVA ZA IMENA .H I .CPP FAJLOVA????????

// komentare prevesti na engleski!!

// kakve typedef-ove gde staviti?

#define CURRENTLY_WRITING_CODE	// ZATO STO BUDJAVI KOMPAJLER BAGUJE

#ifdef CURRENTLY_WRITING_CODE
#define LOCK asm sti;
#define UNLOCK asm cli;
#endif






#ifndef CURRENTLY_WRITING_CODE
#define LOCK { asm { pushf; cli; }}
#define UNLOCK { asm { popf }}
#endif



#define lockCout lockflag = 0;
#define unlockCout lockflag = 1;\
					if (context_change_requested) {\
						dispatch();\
					}

#define LOCK_INTE lockflag = 0;
#define UNLOCK_INTE lockflag = 1;\
					if (context_change_requested) {\
						dispatch();\
					}



#ifndef PCB_H_
#define NEW 1
#define READY 2
#define RUNNING 3
#define BLOCKED 4
#define FINISHED 5
#define IDLE 6
#endif



#ifndef EVENT_H_
#define PREPAREENTRY(entryNo, callOldRout)\
		void interrupt newRoutine##entryNo(...);\
		IVTEntry ivtEntry##entryNo(entryNo, newRoutine);\
		void interrupt newRoutine##entryNo(...) {\
			ivtEntry##entryNo.signalLinkedEvent();\
			if (callOldRout == 1)\
				ivtEntry##entryNo.callOldRoutine();\
		}
#endif



#endif /* DEFS_H_ */
