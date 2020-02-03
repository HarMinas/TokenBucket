//Author: Harutyun Minasyan
//USC NetID: Hminasya
//Date Last Modified: 06/06/18


#ifndef _WARMUP2_H_
#define _WARMUP2_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "pthread.h"
#include "queue.h"
#include "time.h"
#include "tokenBucket.h"
#include "packet.h"
#include "helper.h"
#include "signal.h"


sigset_t set;
int sigPressed = FALSE;
//System Defaults and other arrays
const int arrSize = 6;

typedef struct SystemDefaults
{
	tv globTime; //Will store the start time of emulation.
	char* fileName; //will store the fileName.
	char* keys[6]; 
	double vals[6]; //the rawValues to be processed. stored as doubles
	int packArr; 
	int servTime;
	int tokArr;
	int buckSize;
	int packToks;
	int maxPacks;
}SystemDefaults;



//The 4 thread IDs
pthread_t packAdderThr, bucketThr, s1Thr, s2Thr, sigThread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;


//PacketParameters
Bucket tokenBucket;
PackConstr packetConstructor;
SystemDefaults sysDefs;
Queue q1, q2;
int packetServed;


//structures to hold the arguments that will be passed into the threads
typedef struct ThreadArgs
{
	Queue* q1;
	Queue* q2;
	Bucket* b;
	PackConstr* pc;
	pthread_mutex_t * m;
	pthread_cond_t *cv;
	tv emulEndTime;
	int* packServed;

	double intArrAcc;
	int packsDropped;
	double servTimeAcc;
	double spentQ1Acc;
	double spentQ2Acc;
	int packsServedQ1;
	int packsServedQ2;
	double spentS1Acc;
	double spentS2Acc;
	int packsServedS1;
	int packsServedS2;
	double totalInSys;

}ThreadArgs;

int packetServed;









//Main helper functions
void sysDefInit(SystemDefaults*);
int processCommandLine(int, char**, SystemDefaults*, const int);
void setSysDefs(SystemDefaults*, Bucket*, PackConstr*);
ThreadArgs* initThreadArgs(pthread_mutex_t*, pthread_cond_t* cv, Bucket*, PackConstr* , Queue*, Queue*);
void printEmulationParams(void*, double *);


void sigHandler(void*);
//Bucket Thread functions
void runBucketFilter(void*);
int movePacket(Queue*, Queue*, Bucket*, tv, ThreadArgs*);


//Packet adder thread helpers
void runPacketAdder(void*);
void runTraceMode(ThreadArgs*);
void runDeterMode(ThreadArgs*);


//Server threads
void runServer1(void*);
void runServer2(void*);


//statistics
void printStatics(ThreadArgs*);
#endif /*_WARMUP2_H_*/

