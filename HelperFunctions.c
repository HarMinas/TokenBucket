


#include "helpers.h"



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
	for(i = *startIndex; i < strLenght; i++)
	{	
		currentChar = line[i];
		if(currentChar != ' ' && currentChar != '\t' && currentChar != '\n')
		{
			*startIndex = i;
			break;
		}
	}
	for(i = *startIndex; i < strLenght; i++)
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
	tv * currentTime;
	gettimeofday(currentTime, NULL);
	currentTime->tv_usec = currentTime->tv_usec - globalTime->tv_usec;
	currentTime->tv_sec = currentTime->tv_sec - globalTime->tv_sec;

	printf("%.6lf: \n", currentTime);
}

