#ifndef DEFS_H_
#define DEFS_H_


// SET MEMORY MODEL TO HUGE

//#define CURRENTLY_WRITING_CODE

#ifdef CURRENTLY_WRITING_CODE
#define LOCK asm sti;
#define UNLOCK asm cli;
#endif






#ifndef CURRENTLY_WRITING_CODE
#define LOCK {asm { pushf; cli; }}
#define UNLOCK {asm { popf; }}
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



// possible Thread (PCB) states
#ifndef PCB_H_
#define NEW 1
#define READY 2
#define RUNNING 3
#define BLOCKED 4
#define FINISHED 5
#define IDLE 6
#endif



#endif /* DEFS_H_ */
