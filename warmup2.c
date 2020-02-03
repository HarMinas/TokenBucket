//Author: Harutyun Minasyan
//USC NetID: Hminasya
//Date Last Modified: 06/06/18



#include "warmup2.h"


int main(int argc, char* argv[])
{

	

	
	//Setting up the system 
	sysDefInit(&sysDefs);
	processCommandLine(argc, argv, &sysDefs, arrSize); //Processing the command line arguements
	initBucket(&tokenBucket, sysDefs.vals[sysDefs.buckSize],sysDefs.vals[sysDefs.tokArr],  sysDefs.globTime); //Bucket setup
	queueInit(&q1);
	queueInit(&q2);
	setSysDefs(&sysDefs, &tokenBucket, &packetConstructor); //sysDefs setup
	double* vals = sysDefs.vals;
	packetConstrInit(&packetConstructor, vals[sysDefs.maxPacks], vals[sysDefs.packToks], sysDefs.globTime);

	





	//Initializing the thread argument structs
	ThreadArgs *tArgs = initThreadArgs(&mutex, &cv, &tokenBucket, &packetConstructor, &q1,&q2);
	printEmulationParams(tArgs, vals);
	printf("\n");
	printCurrTime(&sysDefs.globTime);
	printf("Emulation began\n");	





	sigemptyset(&set);
  	sigaddset(&set, SIGINT);
  	sigprocmask(SIG_BLOCK, &set, 0);




	//Creating the Threads
	pthread_create(&sigThread, NULL, (void*)&sigHandler, (void*) tArgs);

	pthread_create(&packAdderThr, NULL, (void*)&runPacketAdder, (void*) tArgs);

	pthread_create(&bucketThr, NULL, (void*)&runBucketFilter, (void*) tArgs);

	pthread_create(&s1Thr, NULL, (void*)&runServer1, (void*) tArgs);

	pthread_create(&s2Thr, NULL, (void*)&runServer2, tArgs);



	//Joining the Threads back to the main thread
	pthread_join(packAdderThr, NULL);
	printf("packAdderThr joined\n");
	pthread_join(bucketThr, NULL);
	printf("bucketThr joined\n");
	pthread_join(sigThread, NULL);
	printf("sigThr joined\n");
	pthread_join(s1Thr, NULL);
	printf("s1Thr joined\n");
	pthread_join(s2Thr, NULL);
	printf("s2Thr joined\n");


	gettimeofday(&tArgs->emulEndTime, NULL);
	processTimeVals(&sysDefs.globTime, &tArgs->emulEndTime);

printCurrTime(&sysDefs.globTime);
printf("Emulation Ends\n");
printf("\n");
	printStatics(tArgs);

	free(tArgs);


	//Returning from the program.

	cleanQueue(&q1);
	cleanQueue(&q2);
	

	return 0;
}

















//initialized the structure
void sysDefInit(SystemDefaults* sysDefs)
{
	sysDefs->packArr = 0; 
	sysDefs->servTime = 1;
	sysDefs->tokArr = 2;
	sysDefs->buckSize = 3;
	sysDefs->packToks = 4;
	sysDefs->maxPacks = 5;

	sysDefs->keys[sysDefs->packArr] = "-lambda";
	sysDefs->keys[sysDefs->servTime] = "-mu";
	sysDefs->keys[sysDefs->tokArr] = "-r";
	sysDefs->keys[sysDefs->buckSize] = "-B";
	sysDefs->keys[sysDefs->packToks] = "-P";
	sysDefs->keys[sysDefs->maxPacks] = "-n";

	sysDefs->vals[sysDefs->packArr] = 1;
	sysDefs->vals[sysDefs->servTime] = 0.35;
	sysDefs->vals[sysDefs->tokArr] = 1.5;
	sysDefs->vals[sysDefs->buckSize] = 10;
	sysDefs->vals[sysDefs->packToks] = 3;
	sysDefs->vals[sysDefs->maxPacks] = 20;
	gettimeofday(&sysDefs->globTime, NULL);
}


void printEmulationParams(void* args, double * vals)
{
	ThreadArgs * tArgs = (ThreadArgs*) args;
	printf("Emulation Parameters:\n");
	printf("    number to arrive = %d\n", (int)vals[5]);
	char* fileName = tArgs->pc->fileName;

	if(fileName == NULL)
	{
		printf("    lambda = %.2lf\n", vals[0]);
		printf("    mu = %.2lf\n", vals[1]);
	}
	
	printf("    r = %.2lf\n", vals[2]);
	printf("    B = %d\n", (int)vals[3]);
	if(fileName == NULL)
	{
		printf("    P = %d\n", (int)vals[4]);
	}
	if(fileName != NULL)
	{
		printf("    tsfile = %s\n", fileName);
		
	}
}



/*
	This method takes in a string array and parses it to match optional args in the argv array
	and store the values that follow a particular flag in a nums array for a later use. 

	@param argc-The size of the argv array-how many strings are stored.
	@param argv-An array of strings that should hold flags that start with "-" character. Each flag must be followed by 
				its correspoding value. The flag-value pairs can be in any order. 
	@param commandArgs- This array holds strings that represent the flags the function can support. The flags start with "-" char.
	@param nums- This array holds the values that will be stored from the argv array. It has the same lenght as the commandArgs 
				and each index strickty should correspond to the commandArgs indeces. (e.g. if flag x is in commandArgs[0], the
				value that should corrrespond to that flag, say y should be stored as a double at nums[0].
	@param arraySize-An integer that represents the size of the commandArgs and nums (by the number of bins and not bytes.)
	@param fileName - If the -t flag is set int argv, this value is set to the next string in the argv if it exists
	@return - TRUE if the function executed withoud errors or FALSE otherwise.

	PRE: nums[] and commandArgs[] should have the same index mappings for value-flag pairs. Both arrays must be of the same size.
		fileName must be NULL
*/
int processCommandLine(int argc, char* argv[], SystemDefaults* sysDefs, const int arraySize)
{	
	int argvIndex = 1;
	int comArgsInd;
	sysDefs->fileName = NULL; //setting the fileName to null just in case the -t flag is off.
	while (argvIndex < argc)
	{
		if (strcmp(argv[argvIndex], "-t") == 0) //Checking for the File name  flag and setting fileNames value to its value
		{
			if(argvIndex+1 < argc)
			{
				sysDefs->fileName = malloc(100);
				strcpy(sysDefs->fileName, argv[argvIndex+1]);
				printf("filename is %s\n", sysDefs->fileName);
				argvIndex++;
				continue;
			}
		}
		for(comArgsInd = 0; comArgsInd < arraySize; comArgsInd++) //Iterating over the commanArgs array to find a match
		{
			if(strcmp(argv[argvIndex], sysDefs->keys[comArgsInd]) == 0) //Comparing the string for a flag
			{
				if(argvIndex+1 < argc) //checking if there is a value for the flag
				{
					if(argv[argvIndex+1][0] != '-') //making sure the value of the flag doesnt start with '-' char
					{
						if(sscanf(argv[argvIndex+1], "%lf", &sysDefs->vals[comArgsInd]) < 1 ) //Simple error check on the scanf
						{
							return FALSE;
						}
						break;
					}
				}
			}
		}
		argvIndex++;
	}
	return TRUE;
}


/*
	This function takes in the nums array and extracts information for various parameters of the program (e.g. max bucket size,
	packet inter-arrival time etc.) into the currect variables

	@param nums - the array that contains the raw numeric information for program parametes as doubles
	@param b - the bucket object used for storing the tokens. MAX_SIZE of it should be set.
	@param servTime - the address of the serviveTime default to be set.
	@param packetNum - the address of the packetNum default to be set.
	@param packArrTime - the address of the packet arrival time default to be set.
	@param servTime - the address of the token arrival time default to be set.

	PRE: This function should be called after processCommandLine();
*/
void setSysDefs(SystemDefaults* sysDefs, Bucket* b, PackConstr* pInfo)
{
 
   if(1.0/sysDefs->vals[sysDefs->servTime] > 10.0)
	{
		pInfo->servTime = 10.0;
	}
	else
	{
		pInfo->servTime = ((1.0/sysDefs->vals[sysDefs->servTime])* 1000);
	}
	if(1.0/sysDefs->vals[sysDefs->packArr] > 10.0)
	{
		pInfo->intArrTime = 10.0;
	}
	else
	{
		pInfo->intArrTime = (double)(1.0/sysDefs->vals[sysDefs->packArr]);
	}
    pInfo->MAX_PACKETS = (int)sysDefs->vals[sysDefs->maxPacks];
    pInfo->tokPerPack = (int)sysDefs->vals[sysDefs->packToks];
    pInfo->fileName = sysDefs->fileName;

  	b->MAX_SIZE = (int)sysDefs->vals[sysDefs->buckSize];
   	b->tokArrTime = (double)(1.0/sysDefs->vals[sysDefs->tokArr]);


}




ThreadArgs* initThreadArgs(pthread_mutex_t* mut, pthread_cond_t* cv,Bucket* buck, PackConstr* packConstr, Queue* queue1, Queue *queue2)
{
	ThreadArgs* tArgs = (ThreadArgs*) malloc(sizeof(ThreadArgs));
	checkMalloc(tArgs, "threadArgs");

	tArgs->q1 = queue1;
	tArgs->q2 = queue2;
	tArgs->b = buck;
	tArgs->m = mut;
	tArgs->pc = packConstr;
	tArgs->cv = cv;
	tArgs->packServed = &packetServed;
	
	tArgs->intArrAcc = 0;
	tArgs->packsDropped = 0;
	tArgs->servTimeAcc = 0;
	tArgs->spentQ1Acc = 0;
	tArgs->spentQ2Acc = 0;
	tArgs->packsServedQ1 = 0;
	tArgs->packsServedQ2 = 0;
	tArgs->spentS1Acc = 0;
	tArgs->spentS2Acc = 0;
	tArgs->packsServedS1 = 0;
	tArgs->packsServedS2 = 0;
	tArgs->totalInSys = 0;
	return tArgs;
}

















//The signal handler
void sigHandler(void* args)
{
	ThreadArgs * tArgs = (ThreadArgs*) args;
	int sig = 0;
	sigwait(&set, &sig);
	printf("\n");
	printf("     SIGINT caught, no new packets or Tokens will be allowed\n");
	printf("\n");
	pthread_mutex_lock(tArgs->m);
	sigPressed = TRUE;
	pthread_cancel(packAdderThr);
	pthread_cancel(bucketThr);

	cleanQueue(tArgs->q1);
	cleanQueue(tArgs->q2);
	pthread_mutex_unlock(tArgs->m);
	pthread_cond_broadcast(tArgs->cv);

	pthread_exit(0); 
}



//The function ran by the bucket thread
void runBucketFilter(void* args)
{

	ThreadArgs* tArgs = (ThreadArgs*) args;
	while((tArgs->pc->MAX_PACKETS - tArgs->packsDropped) > *tArgs->packServed)
	{
			sleepBucket(tArgs->b);
			pthread_mutex_lock(tArgs->m);
			createToken(tArgs->b);

			if(movePacket(tArgs->q1,tArgs->q2, tArgs->b, tArgs->b->globalTime, tArgs))
			{
				pthread_cond_broadcast(tArgs->cv);
			}
			pthread_mutex_unlock(tArgs->m);		
	}
	pthread_cond_broadcast(tArgs->cv);
	pthread_exit(0);
}

//moves packet from Q1 to Q2- Must work with the bucket controller thread

//PRE: MUST BE CALLED INSIDE A MUTEX
int movePacket(Queue* q1, Queue* q2, Bucket* b, tv globTime, ThreadArgs* tArgs)
{
	if(queueEmpty(q1))
	{
		return FALSE;
	}

	const Packet* peekedP = queuePeek(q1);
	if(getTokens(b, peekedP->tokenReq))
	{	
		Packet * p = queuePop(q1);
		gettimeofday(&p->pLvsQ1, NULL);
		processTimeVals(&globTime, &p->pLvsQ1);
		printCurrTime(&globTime);
		tArgs->spentQ1Acc = tArgs->spentQ1Acc + timeDifInMilSec(&p->pEntQ1, &p->pLvsQ1);
		printf("P%d leaves Q1, time in Q1=%.3lf, token bucket now has %d tokens\n", p->packetID, timeDifInMilSec(&p->pEntQ1, &p->pLvsQ1), b->numTokens);
		
		queuePush(q2, p);
		gettimeofday(&p->pEntQ2, NULL);
		processTimeVals(&globTime, &p->pEntQ2);
		printCurrTime(&globTime);
		printf("Packet P%d enters Q2\n", p->packetID);
		return TRUE;
	} 
	return FALSE;
}





//Functions ran y the packet adder thread
void runPacketAdder(void* args)
{
	ThreadArgs * tArgs = (ThreadArgs*) args;
	if(tArgs->pc->fileName != NULL) //Trace Mode
	{

		runTraceMode(tArgs);
	}
	else 
	{

		runDeterMode(tArgs);
	}

}

void runTraceMode(ThreadArgs* tArgs)
{
	int lineSize = 1024;
	char* line = (char*) malloc(lineSize); //Allocating 1024 bytes for a line of information from the file.
	checkMalloc(line, "line");
	FILE* file = fopen(tArgs->pc->fileName, "r");
	fgets(line, lineSize, file);
	tArgs->pc->MAX_PACKETS = (double)strtod(line, NULL);
	

	Packet* p;
	int i;
	for(i = 0; i < tArgs->pc->MAX_PACKETS; i++)
	{

		

		fgets(line, lineSize, file);
		updatePackInfo(tArgs->pc, line);
		sleepPacketAdder(tArgs->pc);
		pthread_mutex_lock(tArgs->m);
		p = createPacketTrace(tArgs->pc, line);
		tv intArrTime;
		time_t secRes = p->pArrTime.tv_sec - tArgs->pc->lastPackArr.tv_sec; 
		suseconds_t msRes = p->pArrTime.tv_usec - tArgs->pc->lastPackArr.tv_usec; 
		if(msRes < 0)
		{
			secRes--;
			msRes = msRes + 1000000;
		}
		intArrTime.tv_sec = secRes;
		intArrTime.tv_usec = msRes;

		tArgs->intArrAcc = tArgs->intArrAcc + timevalToMilSec(&intArrTime);
		

		printCurrTime(&tArgs->pc->globTime);
		printf("Packet P%d arrived, needs %d tokens, inter-arrival time = %.3lf\n", p->packetID, p->tokenReq, timevalToMilSec(&intArrTime));
		tArgs->pc->lastPackArr = p->pArrTime;
		if(p->tokenReq > tArgs->b->MAX_SIZE)
		{
			printf("Packet P%d requires %d tokens to be processed, which exceeds buckets capacity \n", p->packetID, p->tokenReq);
			printf("Dropping this packet\n");
			tArgs->packsDropped = tArgs->packsDropped + 1;
			free(p);
		}
		else
		{
			if(p->tokenReq <= tArgs->b->numTokens && queueEmpty(tArgs->q1))
			{
				getTokens(tArgs->b, p->tokenReq);
				if(queuePush(tArgs->q2, p) == TRUE)
				{
					gettimeofday(&p->pEntQ2, NULL);
					processTimeVals(&tArgs->pc->globTime, &p->pEntQ2);
					printCurrTime(&tArgs->pc->globTime);
					printf("Packet P%d enters Q2\n", p->packetID);
					pthread_cond_broadcast(tArgs->cv);
				}
				
			}
			else
			{
				gettimeofday(&p->pEntQ1, NULL);
				processTimeVals(&tArgs->pc->globTime, &p->pEntQ1);
				printCurrTime(&tArgs->pc->globTime);
				printf("Packet P%d enters Q1\n", p->packetID);
				queuePush(tArgs->q1,p);
			}
		}
		pthread_mutex_unlock(tArgs->m);
	}
	pthread_exit(0);
} 





void runDeterMode(ThreadArgs* tArgs)
{
	Packet * p;
	int i;
	for(i = 0; i < tArgs->pc->MAX_PACKETS; i++)
	{
		sleepPacketAdder(tArgs->pc);
		
		pthread_mutex_lock(tArgs->m);
		p = createPacket(tArgs->pc);
		tv intArrTime;
		time_t secRes = p->pArrTime.tv_sec - tArgs->pc->lastPackArr.tv_sec; 
		suseconds_t msRes = p->pArrTime.tv_usec - tArgs->pc->lastPackArr.tv_usec; 

		if(msRes < 0)
		{
			secRes--;
			msRes = msRes + 1000000;
		}
		intArrTime.tv_sec = secRes;
		intArrTime.tv_usec = msRes;

		tArgs->intArrAcc = tArgs->intArrAcc + timevalToMilSec(&intArrTime);

		printCurrTime(&tArgs->pc->globTime);
		printf("Packet P%d arrived, needs %d tokens, inter-arrival time = %.3lf\n", p->packetID, p->tokenReq, timevalToMilSec(&intArrTime));
		tArgs->pc->lastPackArr = p->pArrTime;

		if(p->tokenReq > tArgs->b->MAX_SIZE)
		{
			printf("Packet P%d requires %d tokens to be processed, which exceeds buckets capacity \n", p->packetID, p->tokenReq);
			printf("Dropping this packet\n");
			tArgs->packsDropped = tArgs->packsDropped + 1;
			free(p);
		}
		else
		{
			if(p->tokenReq <= tArgs->b->numTokens && queueEmpty(tArgs->q1))
			{
				getTokens(tArgs->b, p->tokenReq);
				if(queuePush(tArgs->q2, p) == TRUE)
				{
					gettimeofday(&p->pEntQ2, NULL);
					processTimeVals(&tArgs->pc->globTime, &p->pEntQ2);
					printCurrTime(&tArgs->pc->globTime);
					printf("Packet P%d enters Q2\n", p->packetID);
					pthread_cond_broadcast(tArgs->cv);
				}
				
			}
			else
			{
				gettimeofday(&p->pEntQ1, NULL);
				processTimeVals(&tArgs->pc->globTime, &p->pEntQ1);
				printCurrTime(&tArgs->pc->globTime);
				printf("Packet P%d enters Q1\n", p->packetID);
				queuePush(tArgs->q1,p);
			}
		}
		pthread_mutex_unlock(tArgs->m);
	}
	pthread_exit(0);
}


















void runServer1(void* args)
{

	ThreadArgs * tArgs = (ThreadArgs*) args;
	pthread_mutex_t* mut = tArgs->m;
	pthread_cond_t * cv = tArgs->cv;
	tv * globTime = &tArgs->b->globalTime;
	Queue * queue2 = tArgs->q2;
	Packet* p;
	useconds_t procTime;

	while((tArgs->pc->MAX_PACKETS - tArgs->packsDropped) > *tArgs->packServed && sigPressed != TRUE)
	{
		pthread_mutex_lock(mut);				//lock Mutex
		while(queueEmpty(queue2) && sigPressed != TRUE)
		{
			if((tArgs->pc->MAX_PACKETS - tArgs->packsDropped) <= *tArgs->packServed && sigPressed == TRUE)
			{
				pthread_mutex_unlock(mut);
				pthread_exit(0);				
			}
			pthread_cond_wait(cv, mut);
		}

		if(!queueEmpty(queue2) && sigPressed != TRUE)
		{
			p = queuePop(queue2);
			printCurrTime(globTime);
			gettimeofday(&p->pLvsQ2, NULL);
			processTimeVals(globTime, &p->pLvsQ2);

			tArgs->spentQ2Acc = tArgs->spentQ2Acc + timeDifInMilSec(&p->pEntQ2, &p->pLvsQ2);

			printf("P%d leaves Q2, time in Q2 = %.3lfms\n", p->packetID, timeDifInMilSec(&p->pEntQ2, &p->pLvsQ2));
			int num = *tArgs->packServed;
			num++;
			*(tArgs->packServed) = num;
			pthread_cond_broadcast(cv);
		
			pthread_mutex_unlock(mut);				//Unlock Mutex

			procTime = (p->serviceTime); //in miliseconds

		
			printCurrTime(globTime);
			printf("P%d begins service at S1, requesting %dms of service\n", p->packetID, p->serviceTime);
			gettimeofday(&p->pEntS, NULL);
			processTimeVals(globTime, &p->pEntS);
			usleep(procTime);

			gettimeofday(&p->pLvsS, NULL);
			processTimeVals(globTime, &p->pLvsS);
			double st = tArgs->servTimeAcc;
			double s1acc = tArgs->spentS1Acc;
			double tot = tArgs->totalInSys;
			tArgs->servTimeAcc = st + timeDifInMilSec(&p->pEntS, &p->pLvsS);
			tArgs->spentS1Acc = s1acc + timeDifInMilSec(&p->pEntS, &p->pLvsS);
			tArgs->totalInSys = tot + timeDifInMilSec(&p->pArrTime, &p->pLvsS);
			printCurrTime(globTime);
			printf("P%d departs from S1, service Time = %.3lfms, time in system %.3lfms \n",p->packetID, timeDifInMilSec(&p->pEntS, &p->pLvsS),timeDifInMilSec(&p->pArrTime, &p->pLvsS));
			free(p);
		}
		pthread_mutex_unlock(mut);
	}
	pthread_exit(0);
}



void runServer2(void* args)
{

	ThreadArgs * tArgs = (ThreadArgs*) args;
	pthread_mutex_t* mut = tArgs->m;
	pthread_cond_t * cv = tArgs->cv;
	tv * globTime = &tArgs->b->globalTime;
	Queue * queue2 = tArgs->q2;
	Packet* p;
	useconds_t procTime;


	while((tArgs->pc->MAX_PACKETS - tArgs->packsDropped) > *tArgs->packServed && sigPressed != TRUE)
	{
		
		
		pthread_mutex_lock(mut);				//lock Mutex
		while(queueEmpty(queue2) && sigPressed != TRUE)
		{
			if((tArgs->pc->MAX_PACKETS - tArgs->packsDropped) <= *tArgs->packServed && sigPressed == TRUE)
			{
				pthread_mutex_unlock(mut);
				pthread_exit(0);
			}
			pthread_cond_wait(cv, mut);
		}
		if(!queueEmpty(queue2) && sigPressed != TRUE)
		{
			p = queuePop(queue2);
			printCurrTime(globTime);
			gettimeofday(&p->pLvsQ2, NULL);
			processTimeVals(globTime, &p->pLvsQ2);
			tArgs->spentQ2Acc = tArgs->spentQ2Acc + timeDifInMilSec(&p->pEntQ2, &p->pLvsQ2);
			printf("P%d leaves Q2, time in Q2 = %.3lfms\n", p->packetID, timeDifInMilSec(&p->pEntQ2, &p->pLvsQ2));
			int num = *tArgs->packServed;
			num++;
			*(tArgs->packServed) = num;
			pthread_cond_broadcast(cv);
							

		
			pthread_mutex_unlock(mut);//Unlock Mutex
			procTime = p->serviceTime ;

		
			printCurrTime(globTime);
			printf("P%d begins service at S2, requesting %dms of service\n", p->packetID, p->serviceTime);
			gettimeofday(&p->pEntS, NULL);
			processTimeVals(globTime, &p->pEntS);
			usleep(procTime);

			gettimeofday(&p->pLvsS, NULL);
			processTimeVals(globTime, &p->pLvsS);
			double st = tArgs->servTimeAcc;
			double s1acc = tArgs->spentS1Acc;
			double tot = tArgs->totalInSys;
			tArgs->servTimeAcc = st + timeDifInMilSec(&p->pEntS, &p->pLvsS);
			tArgs->spentS2Acc = s1acc + timeDifInMilSec(&p->pEntS, &p->pLvsS);
			tArgs->totalInSys = tot + timeDifInMilSec(&p->pArrTime, &p->pLvsS);
			printCurrTime(globTime);
			printf("P%d departs from S2, service Time = %.3lfms, time in system %.3lfms \n",p->packetID, timeDifInMilSec(&p->pEntS, &p->pLvsS),timeDifInMilSec(&p->pArrTime, &p->pLvsS));
			free(p);
		}
		pthread_mutex_unlock(mut);
	}
	pthread_exit(0);
}



void printStatics(ThreadArgs* tArgs)
{	

	tv relativeStart;
	relativeStart.tv_usec = sysDefs.globTime.tv_usec;
	relativeStart.tv_sec = sysDefs.globTime.tv_sec;
	processTimeVals(&sysDefs.globTime, &relativeStart);


	int relPackets = tArgs->pc->MAX_PACKETS - tArgs->packsDropped;
	printf("Statistics: \n");
	printf("\n");
	printf("	average packet inter-Arrival time = %.6lf\n", tArgs->intArrAcc/(double)tArgs->pc->MAX_PACKETS);
	printf("	average packet service time = %.6lf\n", tArgs->servTimeAcc/(double)relPackets);
	printf("\n");
	printf("	average number of packet in Q1 = %.6lf\n", tArgs->spentQ1Acc/timeDifInMilSec(&relativeStart, &tArgs->emulEndTime));
	printf("	average number of packet in Q2 = %.6lf\n", tArgs->spentQ2Acc/timeDifInMilSec(&relativeStart, &tArgs->emulEndTime));
	printf("	average number of packet in S1 = %.6lf\n", tArgs->spentS1Acc/timeDifInMilSec(&relativeStart, &tArgs->emulEndTime));
	printf("	average number of packet in S2 = %.6lf\n", tArgs->spentS2Acc/timeDifInMilSec(&relativeStart, &tArgs->emulEndTime));
	printf("\n");
	printf("	average time a packet spent in the system = %.6lf\n", tArgs->totalInSys/(double)relPackets);
	printf("	standard deviation for time spent in the system = xxx\n" );
	printf("\n");
	printf("	token drop probability = %.6lf\n", (double)tArgs->b->tokensDropped/(double)(tArgs->b->nextTokID-1));
	printf("	packet drop probability = %.6lf\n",(double)tArgs->packsDropped/(double)tArgs->pc->MAX_PACKETS);

}
