#ifndef __TERMS__
#define __TERMS__
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "gconst.h"
#include "random.h"
using namespace std;
//functions and terminals structure
struct _term {
	       char name[MAXNAME];
	       BYTE arity;
	     };
typedef struct _term term;
//Global variables for functions and term
term glterm[MAXFUNCTION+MAXTERMINAL];
int  gltermcard; //number of function and terminal
term glfunction[MAXFUNCTION];
int glfcard;
term glterminal[MAXTERMINAL];
int gltcard;
//*********************************
//Initialization of global terms  *
//*********************************
void InitTerm(void)
{
 gltermcard=0;
 glfcard=0;
 gltcard=0;
}
//*********************************
//Adding a new term               *
//*********************************
void AddTerm(char *name, BYTE arity)
{
    glterm[gltermcard].arity= arity;
    strcpy(glterm[gltermcard].name,name);
    gltermcard++;
   if(arity==0)
    {
     glterminal[gltcard].arity= arity;
     strcpy(glterminal[gltcard].name,name);
     gltcard++;
    }
   else
    {
     glfunction[glfcard].arity= arity;
     strcpy(glfunction[glfcard].name,name);
     glfcard++;
    }
}
void PrintTerm(void)
{
 int i;
for(i=0;i<gltermcard;i++)
 {
  cout<<endl<<"--------TERM "<<i<<"---------";
  cout<<endl<<"NAME:"<<glterm[i].name;
  cout<<endl<<"ARITY:"<<(int) glterm[i].arity;
 }
}
//swapterm to make it random distributed
void SwapTerm(int times)
{
 int i,j,k;
 term temp;
assert(gltermcard>=2);
 for(i=0;i<times;i++)
  {
   j=IRandom(0,gltermcard-1);
   k=IRandom(0,gltermcard-1);
   temp=glterm[j];
   glterm[j]=glterm[k];
   glterm[k]=temp;
  }
}
#endif
