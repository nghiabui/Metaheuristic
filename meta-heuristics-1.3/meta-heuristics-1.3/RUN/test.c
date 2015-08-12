#include <stdlib.h>
#include <stdio.h>

inline char compare(double num1, double num2)
{
	
	static char a1[50];
	static char a2[50];
	sprintf(a1,"%lf",num1);
	sprintf(a2,"%lf",num2);
	return !strcmp(a1,a2);
	
}
inline char is_equals(double A, double B )
{
 static float maxRelativeError=0.000001;
 static float maxAbsoluteError=0.000001;
 if (fabs(A - B) < maxAbsoluteError)
    return 1;

  static float relativeError;
  if (fabs(B) > fabs(A))
    relativeError = fabs((A - B) / B);
  else
    relativeError = fabs((A - B) / A);
  if (relativeError <= maxRelativeError)
    return 1;

  return 0;
}
double convert()
 {

	int makespan = 3185;
	int tardiness = 686;
	double result = makespan;
	printf("initial %0.30lf \r\n ", result);
	int tmp=1;
	while(tmp<makespan) tmp=tmp*10;
	result= (double)(makespan/tmp);
	printf("after divide %0.30lf \r\n ", result);
	result += tardiness;
	printf("after add tardiness %0.30lf \r\n ", result);
	printf("Converting makespan %d tardiness %d to result %0.30lf \n",makespan,tardiness,result);
	return result;

}
int main()
{

//	double gain;
//	gain = convert();
//	printf("gain is %f decoded to \r\n",gain);
//	int tardiness = (int) gain;
//	printf("tardiness: %d \r\n",tardiness);
//	gain = gain - tardiness;
//	printf("gain is %f decoded to \r\n",gain);
//	double num1 = (gain -((int)gain)*1.0);
//	double zero = 0.0;
//	while (!is_equals(gain -((int)gain)*1.0,0))
//	{
//
//		gain = gain * 10;
//		printf("gain %f (int) gain %d \n",gain,(int)gain);
//	}
	int i=1;
	double a=10.0;
for(i=0;i<10;i++)
{
	a=a/10;
	printf("%0.30lf \r\n",a);
}
	return 0;
}
