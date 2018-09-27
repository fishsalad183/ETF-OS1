#ifndef MACRO_H_
#define MACRO_H_



#define LOCK { asm {\
				pushf;\
				cli;\
				}\
			}

#define UNLOCK { asm { popf }}



#define lockCout lockflag = 0;
#define unlockCout lockflag = 1;\
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


#endif /* MACRO_H_ */
