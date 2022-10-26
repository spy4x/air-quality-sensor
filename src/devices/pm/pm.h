#ifndef PM_H
#define PM_H

#include "_config.h"
#include "_shared.h"

void pmSetup();
void pmSleep();
void pmWakeUp();
void pmRead();
uint16_t pmGet1p0();
uint16_t pmGet2p5();
uint16_t pmGet10p0();

#endif