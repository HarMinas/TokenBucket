

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "stdio.h"
#include "stdlib.h"
#include "time.h"


typedef struct timevals tv;

void checkMalloc(void*, char*);

void setStrIndex(char*, int*, int*);

void printcurrTime(tv*);

#endif
