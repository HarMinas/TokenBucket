//Author: Harutyun Minasyan
//USC NetID: Hminasya
//Date Last Modified: 06/06/18


#include "packet.h"



/*
	Initializes a PackConstr object. This function should be called once in order to further use the PackConstr object to create
	packets. For Trace mode, some of the parameters should be NULL. For further information refer to param descriptions and 
	PackConstr struct.

	@param pc - The PackConstr object to be initialized.
	@param maxPackets - the number of packets that need to be created. (NULL if in trace mode).
	@param tokPerPack - the number of tokens a packet should take to pass the token bucket filter.	(NULL if in trace mode).
	@param servTime - the amound of time a packet spends at the server for processng in milliseconds. (NULL if in trace mode).
	@param packArrTime - the amount of time between the arrival of this packet and the one before it or beginning of the 
							emulation if this will be the first packet. (NULL if in trace mode).
	@param globalTime - a timeval struct object (typedef tv) that holds the beginning of the time emulation time info.
*/
void packetConstrInit(PackConstr* pc, int maxPackets, int tokPerPack, tv globalTime)
{
	pc->MAX_PACKETS = maxPackets;
	pc->tokPerPack = tokPerPack;
	pc->nextPackID = 1;
	pc->globTime = globalTime;
	pc->lastPackArr = globalTime;
	processTimeVals(&pc->globTime, &pc->lastPackArr);
}



/*
	Creates a packet dynamically and returns a pointer to it. Uses PackConstr 

	@param packInfo - A PackConstr object that contains information about how to create the packet
	@return - A pointer to a Packet object just created.

	PRE:PackConstr referenced object should have all fields initialized that are directly used to create the packet.
*/
Packet* createPacket(PackConstr* packInfo)
{
	Packet * p = (Packet*) malloc(sizeof(Packet));	//Memory allocation for the packet and error checking
	checkMalloc(p, "packet");			
	gettimeofday(&p->pArrTime, NULL);		
	processTimeVals(&packInfo->globTime, &p->pArrTime);
	p->packetID = packInfo->nextPackID;
	p->serviceTime = packInfo->servTime;
	p->tokenReq = packInfo->tokPerPack;

	packInfo->nextPackID++;
	

	return p;
}	



/*
	returns a pointer to a newly created packet using information from an input string. First the input string is 
	converted to an array of doubles, which are then used to update a PackContsr structure. Next, this structure is used as 
	a parameter to PacketInit() function (see packetInit comments for more info).

	@param packInfo - the Packconstr object pointer.
	@param rawLine - the input string that should contain packet creating information.
	param lineLength - the lenght of the rawLine.
	@return - A pointer to a newly created Packet object.
*/
Packet* createPacketTrace(PackConstr* packInfo, char* rawLine)
{	
	updatePackInfo(packInfo, rawLine);
	Packet* p = createPacket(packInfo);
	return p;
}

void updatePackInfo(PackConstr* packInfo, char* rawLine)
{
	double * vals = processLine(rawLine);
	packInfo->intArrTime = vals[0]/1000;
	packInfo->tokPerPack = vals[1]; 
	packInfo->servTime = vals[2];
}



/*
	Takes in a line that contains Packet creation parameters and puts it in an array of doubles.
	Performs error checking on the length of the line, the consistency and spacing between elements

	@param line - an array of string values that represent the values to create a new packet.
	@param info - a pointer to an array of double. This array should hold the values used to create a new Packet.

	PRE: line and info should not be NULL.
*/
double* processLine(char* line)
{	
	int startIndex = 0;
	int endIndex = 0;
	int i;
	double * v;
	v =  malloc(3* sizeof(double));
	checkMalloc(v, "vals");
	for(i = 0; i < 3; i++)
	{
		setStrIndex(line, &startIndex, &endIndex);
		const char* start = &line[startIndex];
		v[i] = strtod(start, NULL);
		startIndex = endIndex + 1;
	}
	return v;
}	












int sleepPacketAdder(PackConstr* pInfo)
{
	tv currTime;
	gettimeofday(&currTime,NULL);
	processTimeVals(&pInfo->globTime, &currTime);
	double milSecCurrTime = timevalToMilSec(&currTime);
	double milSecLastArrTime = timevalToMilSec(&pInfo->lastPackArr);
	double milSecPackArrTime = pInfo->intArrTime * 1000;

	double sleepInterval = (milSecLastArrTime + milSecPackArrTime);
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
