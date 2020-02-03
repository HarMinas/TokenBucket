//Author: Harutyun Minasyan
//USC NetID: Hminasya
//Date Last Modified: 06/06/18


#include "tokenBucket.h"

/*
	initializes a bucket parameters. Dynamically allocates memory for the tokenBucket internak Token
	array with size of Token size * MAX_SIZE of the bucket. 

	@param b - The Bucket object to be initialized
	@param maxSize - The size of the token array. 
	@param tokArrTime - The bucket is in charge of creating new tokens and this variable is used
						to time the creation of the new tokens.

	PRE: b has to be a non NULL pointer. maxSize >= 0, tokArrTime >=0.
	POST: numTokens && tokensDropped == 0, nextTokID ==1.
*/
void initBucket(Bucket*b, int maxSize, int tokArrTime, tv globTime)
{
	b->MAX_SIZE = maxSize;
	b->tokenBucket = malloc(b->MAX_SIZE * sizeof(Token*));
	b->nextTokID = 1;
	b->tokensDropped = 0;
	b->tokArrTime = tokArrTime;
	b->numTokens = 0;
	b->globalTime = globTime;
	b->lastTokArrTime = globTime;
	processTimeVals(&b->globalTime, &b->lastTokArrTime);
}

/*
	Creates a new token object and attempts to add it to the bucket. If the bucket is full, prints
	token parameters, descards the token and returns FALSE. Otherwise, adds the token to the bucket,
	prints parameters and returns TRUE.

	@param b - The bucket object in which to add the new token.

	PRE&POST: The bucket cannot be NULL, has to be initialized using initBucket().
				 and b->numTokens <= b->MAX_SIZE;
*/
int createToken(Bucket* b)
{
	Token* newToken = malloc(sizeof(Token));
	checkMalloc(newToken, "newToken");
	gettimeofday(&newToken->arrTime, NULL);
	processTimeVals(&b->globalTime, &newToken->arrTime);
	newToken->tokenID = b->nextTokID; //Setting the ID of the Token
	b->nextTokID++; //Incrementing the next token ID tracker
	b->lastTokArrTime = newToken->arrTime; //Setting the arrival time of the last token.

	if(b->numTokens < b->MAX_SIZE)
	{
		printTokenArrivalInfo(newToken, b);
		b->tokenBucket[b->numTokens] = newToken;// storing the newly created Token into the bucket
		b->numTokens++; //incrementing the token count
		return TRUE;
	}
	else
	{	
		printTokenArrivalInfo(newToken, b);
		free(newToken); //destroying the newToken.
		b->tokensDropped ++;
		return FALSE;
	}
}


/*
	Takes a token and a bucket, prints out the relevant information for the token during its addition to
	the bucket

	@param t - A pointer to a token object.
	@param b - A pointer to a bucket object.
*/
void printTokenArrivalInfo(Token* t,  Bucket* b)
{
	printCurrTime(&b->globalTime);
	if(b->numTokens + 1 <=  b->MAX_SIZE)
	{
		printf("Token t%d has now arrived, token bucket now has %d tokens\n",t->tokenID, b->numTokens+1);
	}
	else 
	{
		printf("Token t%d has now arrived, token bucket is full and will drop this token\n", t->tokenID);
	}
}

/*
	Takes a Bucket and reduces the number of Tokens it has by a specified amount. Returns TRUE if the operation
	was successfull or FALSE if the operation failed or Bucket did not have that many tokens

	@param b - A bucket object from which the tokens should be deducted.
	@param amount - the number of tokens to be deducted.
	@return - TRUE if the specified number of tokens were removed or FALSE otherwise.

	PRE: The tokensBucket in the b cannot have gaps or empty slots in the beginning if the numToken > 0. 
*/
int getTokens(Bucket* b, int amount)
{
	if(b->numTokens < amount)
	{
		return FALSE;
	}

	int i;
	for(i = 0; i < amount; i++)
	{
		free((b->tokenBucket[i]));
	}
	for(i = amount; i < b->MAX_SIZE; i++)
	{
		b->tokenBucket[i-amount] = b->tokenBucket[i];
	}
	b->numTokens = b->numTokens - amount;
	return TRUE;
}

//Sleeps the bucket 
int sleepBucket(Bucket* b)
{
	tv currTime;
	gettimeofday(&currTime,NULL);
	processTimeVals(&b->globalTime, &currTime);
	double milSecCurrTime = timevalToMilSec(&currTime);
	double milSecLastArrTime = timevalToMilSec(&b->lastTokArrTime);
	double milSecTokArrTime = b->tokArrTime * 1000;
	double sleepInterval = (milSecLastArrTime + milSecTokArrTime);
	useconds_t sleepTime =(sleepInterval - milSecCurrTime)*1000;
	if(sleepTime > 0)
	{
		usleep(sleepTime);
	}
	else
	{
		printf("did not sleep\n");
	}
	return TRUE;
}
