#ifndef __RANDOMGEN__
#define __RANDOMGEN__
#include<math.h>
#include<assert.h>
#include "gconst.h"
long idum;//seed number
long IA;
long IM;
double AM;
long IQ;
long IR;
long NTAB;
double NDIV;
double EPS;
double RNMX;
long iy;
long *iv;
//Functions
void  InitRanGen();
void Set_Seed(int );
void Set_Seed(long );
double Next_Gaussian(double , double);
double Next_Double();
double Next_Gaussian();
BOOL Flip(double);
int IRandom(int,int);
//Initialize the random generator
void InitRanGen()
{
  idum=(long) -12345;
  IA=(long) 16807;
  IM=2147483647;
  AM=(1.0/IM);
  IQ=127773;
  IR=(long)2836;
  NTAB=32;
  NDIV=(1.0+(IM-1.0)/(double)NTAB);
  EPS=1.2e-7;
  RNMX=(1.0-EPS); //largest double less than 1
  iy=0;
  iv=new long[NTAB]; assert(iv!=NULL);
}
void Set_Seed(int x)
{
  idum=(long) -x;
}
void Set_Seed(long x)
{
  idum=-x;
}
double Next_Gaussian(double mean,double stdev)
{
 double x=(double) Next_Gaussian();
 return (double)(x*stdev+mean);
}
double Next_Double()
{
 int j;
 long k;
 double temp;
if (idum<=0 || !iy)
 {
   if (-idum<1)
      idum=1;
   else
      idum=-idum;
 for(j=NTAB+7;j>=0;j--)
  {
    k=(long)(idum/(double)IQ);
    idum=IA*(idum-k*IQ)-IR*k;
    if (idum<0)
      idum+=IM;
    if (j<NTAB)
     iv[j]=idum;
  }
  iy=iv[0];
 }
 k=(long)(idum/(double)IQ);
 idum=IA*(idum-k*IQ)-IR*k;
 if (idum<0)
 idum+=IM;
 j=(int)(iy/NDIV);
 iy=iv[j];
 iv[j]=idum;
 if ((temp=AM*iy)>RNMX)
    return RNMX;
 else
   return temp;
}
double Next_Gaussian()
{
 static int iset=0;
 static double gset;
 double fac,rsq,v1,v2;
 if (iset==0)
   {
     do
      {
	v1=2.0*Next_Double()-1.0;
	v2=2.0*Next_Double()-1.0;
	rsq=v1*v1+v2*v2;
      }
     while (rsq>=1.0||rsq==0.0);
    fac=sqrt(-2.0*log(rsq)/(double)rsq);
    gset=v1*fac;
    iset=1;
    return v2*fac;
   }
  else
   {
    iset=0;
    return gset;
   }
}
//return TRUE with a probability
BOOL Flip(double prob)
{
  double temp=Next_Double();
  return (temp<=prob) ? TRUE:FALSE;
}
//return a random integer between lower and upper
int IRandom(int lower,int upper)
{
 int temp;
 temp=lower+(int)(Next_Double()*(upper-lower+1));
  return temp;
}
#endif
