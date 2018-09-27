#ifndef TIMER_H_
#define TIMER_H_



typedef void interrupt (*pInterrupt)(...);



void interrupt timer(...);

void initialize();

void restore();



#endif /* TIMER_H_ */
