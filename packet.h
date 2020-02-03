//Author: Harutyun Minasyan
//USC NetID: Hminasya
//Date Last Modified: 06/06/18



#ifndef _PACKET_H_
#define _PACKET_H_

#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "unistd.h"
#include "helper.h"





//A stuct specifying a packet
typedef struct Packet
{
	tv pArrTime, pEntQ1, pLvsQ1, pEntQ2, pLvsQ2, pEntS, pLvsS;
	int packetID;
	int serviceTime;
	int tokenReq;
}Packet;

//A structure that will hold packet creation information for the deterministic mode.
typedef struct PackConstr
{
	int MAX_PACKETS; 		//The number of packets that need to be processed during emulation
	int servTime;	//The amount of time a packet needs to spend at the server(in seconds).
	double intArrTime;//The time between two packet arrivals at the system.
	int tokPerPack;			//The number of tokens the packet takes to pass the filter
	int nextPackID;      	//Keeps track of the packet IDs
	tv globTime;			//The time fixed at the beinging of the emulations(start of emulation)
	tv lastPackArr;  //Holds the arrival time of the last Packet to the system
	char* fileName;         //Will hold the fileName.

}PackConstr;




//Functions
void packetConstrInit(PackConstr*, int, int, tv);
Packet* createPacket(PackConstr*);
Packet* createPacketTrace(PackConstr*, char*);
void updatePackInfo(PackConstr*, char*);
double* processLine(char*);
int sleepPacketAdder(PackConstr*);
#endif /*_PACKET_H_*/
