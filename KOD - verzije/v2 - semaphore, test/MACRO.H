#ifndef MACRO_H_
#define MACRO_H_


// MALA/VELIKA SLOVA ZA IMENA .H I .CPP FAJLOVA????????

// komentare prevesti na engleski!!

//#define CURRENTLY_WRITING_CODE	// ZATO STO BUDJAVI KOMPAJLER BAGUJE

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



/*#ifndef QUEUE_H_
#define WAIT_INDEFINITELY 0
#define WAIT_LIMITED 1
#endif*/



//typedef unsigned int Time;	//??



#endif /* MACRO_H_ */
