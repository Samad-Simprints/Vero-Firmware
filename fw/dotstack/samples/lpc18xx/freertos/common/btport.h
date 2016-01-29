
#ifndef __BTPORT_H_INCLUDED__
#define __BTPORT_H_INCLUDED__


void btport_init(void);
void btport_start(void);
void btport_handleRx(void);
void btport_handleTx(void);
void btport_configureForHighSpeed(void );
void btport_setLoadingInitScript(int state);
void btport_suspendRx(void);
void btport_resumeRx(void);


#endif // __BTPORT_H_INCLUDED__
