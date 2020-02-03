//Author: Harutyun Minasyan
//USC NetID: Hminasya
//Date Last Modified: 06/06/18


#ifndef _TOKENBUCKET_H_
#define _TOKENBUCKET_H_

#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "time.h"
#include "pthread.h"
#include "helper.h"



//A struct specifying a token.
typedef struct
{
	tv arrTime; //The arrival time of the token into the bucket.
	int tokenID; 			//The token Identifier. Has to be a positive integer.
}Token;

//A struct specifying a Bucket.
typedef struct 
{
	int MAX_SIZE; 	      //The Maximum number of Tokens the bucket can hold.
	int numTokens;	      //The number of tokens currently in the bucket.
	int nextTokID;        //The ID tracker that is used to assign new IDs to incomin Tokens.
	int tokensDropped;    //The number of tokens dropped due to full bucket.
	double tokArrTime; 	  //The Arrival Time of the tokens in seconds.
	tv globalTime;		  //The startTime of the whole emulation.
	tv lastTokArrTime; //The arrival time of the last token in the bucket.
	Token ** tokenBucket; //An array of pointers to Token objects. The array should be dynamically created using MAX_SIZE.

}Bucket;


//For function descriptions refer to the TokenBucket.c file. Each functions description is above it.
//initializes the bucket.
void initBucket(Bucket*, int, int, tv); 

//Creates tokens with by a process unique to the bucket params.
int createToken(Bucket*); 

//Removes tokens from the bucket by a specific amount if possible.
int getTokens(Bucket*, int);

//Returns the amount the bucket should sleep before creating a new packet.
int sleepBucket(Bucket*);


//private
//helper function used to print information about tokens and the bucket.
void printTokenArrivalInfo(Token*,  Bucket*); 



#endif /*_TOKENBUCKET_H_*/
