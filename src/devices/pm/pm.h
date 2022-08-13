#ifndef PM_H
#define PM_H

#include "_config.h"
#include "_shared.h"

void pmSetup();
void pmSleep();
void pmWakeUp();
uint16_t pmGet();

#endif