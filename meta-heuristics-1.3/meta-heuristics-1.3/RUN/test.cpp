#include <stdlib.h>
#include <stdio.h>

int main()
{

	double gain=6.840000;
	while ((gain -((int)gain)*1.0)>0.0)
	{
		printf("gain %f (int) gain %d %f \n",gain,(int)gain,(gain -((int)gain)*1.0));
		gain = gain * 10;
	}
	printf("hehe: %d\r\n",(gain -((int)gain)*1.0)>0.0);
	return 0;
}
