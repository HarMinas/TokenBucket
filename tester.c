#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cs402.h"
#include "time.h"
#include "errno.h"
#include "unistd.h"
//#include "helper.h"










int main(int argc, char* argv[])
{


double * v;

v = malloc(3* sizeof(double));

v[0] = 3.24;
v[1]= 42.41;
v[2]= 0.23;


int i;

for (i=0;i<3;i++)
{
	printf("%.3lf\n",v[i]);
}


	







	return 0;
}

