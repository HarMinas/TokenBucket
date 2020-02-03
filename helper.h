

#ifndef _HELPER_H_
#define _HELPER_H_

#include "stdio.h"
#include "stdlib.h"
#include "sys/time.h"
#include "cs402.h"
#include "string.h"


typedef struct timeval tv;

void checkMalloc(void*, char*);

void setStrIndex(char*, int*, int*);

void printCurrTime(tv*);

void processTimeVals(tv*, tv*); //Adjusts the current time with global time.

void addTimes(tv*, tv*);

void printTimevalMS(tv*);

double timevalToMilSec(tv* t);

double timeDifInMilSec(tv* old,tv* new);

#endif
