


#include "helper.h"



/*
	Gets a string and starts scanning it from a start index. Once it encounters the first non space/tab character, it sets
	the startIndex to that elements index. Afterwards, uses the newly set startIndex and scans the line from that index
	until it encounters the first tab/space character. At this point it sets endIndex value to the index one less of the 
	first tab/space after new startIndex.

	@param string - the c string from which will be scanned.
	@param strLenght - the total lenght of the string.
	@param starIndex - a pointer to integer which will serve as the start index for converting char data to double.
	@param endIndex - a pointer to integer which will serve as the end index for converting char data to double.
*/
void setStrIndex(char* line, int* startIndex, int* endIndex)
{
	int i;
	char currentChar;
	int strLength = (int)strlen(line);
	for(i = *startIndex; i < strLength; i++)
	{	
		currentChar = line[i];
		if(currentChar != ' ' && currentChar != '\t' && currentChar != '\n')
		{
			*startIndex = i;
			break;
		}
	}
	for(i = *startIndex; i < strLength; i++)
	{
		currentChar = line[i];
		if(currentChar == ' ' || currentChar == '\t' || currentChar == '\n')
		{
			*endIndex = i-1;
			break;
		}
	}
}


/*
	Check to see if malloc was successful. If not exits the program and prints debug message
	
	@param ptr - The pointer to check for a NULL value.
	@param name - name of the pointer that caused malloc to failed
*/
void checkMalloc(void* ptr, char* name)
{
	if(ptr == NULL)
	{
		printf("Malloc failed to allocate memory for the %s: Exiting the program\n", name);
		exit(0);
	}
}



//Gives the current time in microseconds from the start of the simulation.
void printCurrTime(tv* globTime)
{
	tv currentTime;
	gettimeofday(&currentTime, NULL);
	processTimeVals(globTime, &currentTime);
	printTimevalMS(&currentTime);
}


//deducts globTime from currTime and stores the difference in currTime (adjust the currTime to tbe global Time)
void processTimeVals(tv* globTime, tv* currentTime)
{
	if(currentTime == NULL || globTime == NULL)
	{
		printf("Current Time is NULL\n");
	}
	time_t secRes = currentTime->tv_sec - globTime->tv_sec; 
	suseconds_t msRes = currentTime->tv_usec - globTime->tv_usec; 
	if(msRes < 0)
	{
		secRes--;
		msRes = msRes + 1000000;
	}
	currentTime->tv_sec = secRes;
	currentTime->tv_usec = msRes;
}

void addTimes(tv* old, tv* new)
{
	if(old == NULL || new == NULL)
	{
		printf("Current Time is NULL\n");
	}
	time_t secRes = new->tv_sec + old->tv_sec; 
	suseconds_t msRes = new->tv_usec + old->tv_usec; 
	if(msRes < 1000000)
	{
		secRes++;
		msRes = msRes - 1000000;
	}

	new->tv_sec = secRes;
	new->tv_usec = msRes;
}


//returns the number to be printed in miliseconds
void printTimevalMS(tv* timeval)
{
	char buf[13];
	char results[13];
	int i;
	for(i = 0; i < 13; i++)
	{
		results[i] = '0';
	}


	double miliseconds = timeval->tv_sec * 1000;
	double ms = (double)timeval->tv_usec;
	miliseconds = miliseconds + (ms/1000);


	snprintf(buf, 13, "%.3lf", miliseconds);
	int numCharLenght = strlen(buf); 
	int resInd = 12 - numCharLenght;

	for(i = 0; i < numCharLenght; i++)
	{
		results[resInd] = buf[i];
		resInd++;
	}
	results[13] = '\0';
	printf("%sms: ", results);
}

double timevalToMilSec(tv* t)
{
	double miliseconds = t->tv_sec * 1000;
	double ms = (double)t->tv_usec;
	miliseconds = miliseconds + (ms/1000);

	return miliseconds;
}


double timeDifInMilSec(tv* old,tv* new)
{
	return timevalToMilSec(new) - timevalToMilSec(old);
}
