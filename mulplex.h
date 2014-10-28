//************************************************************************
//This module contains fitness case and functions to compute raw fitness *
//as well as standardized fitness for MultiPlexer Problems               *
//Programmed by NXH.                                                     *
//************************************************************************
#ifndef __MULTIPLEXER__
#define __MULTIPLEXER__

#include<iostream>
#include<string.h>
#include<stdio.h>
#include<assert.h>
#include<math.h>
#include<stdlib.h>
#include "gconst.h"
#include "random.h"
#include "term.h"
#include "stree.h"
#include  "genetic.h"
using namespace std;
#define NUMADDRESS  2
struct _sample{
		 double x[NUMVAR];
		 double y;
		};
typedef struct _sample sample;
sample fitcase[NUMFITCASE];
BOOL SuccPredicate=FALSE;
void SetFitCase()
{
 int i,j,h,l,t;
 FILE *fp;
 fp=fopen(TRAINFILE,"r");assert(fp!=NULL);
 for(i=0;i<NUMFITCASE;i++)
  {
   for(j=0;j<NUMVAR;j++)
	    fscanf(fp,"%lf",&fitcase[i].x[j]);
   fscanf(fp,"%lf",&fitcase[i].y);
}

 for(i=0;i<NUMFITCASE;i++)
  {
   for(j=0;j<NUMVAR;j++)
	    printf("\t%lf",fitcase[i].x[j]);
   printf("\t%lf\n",fitcase[i].y);
 }
fclose(fp);
}
//********************************************************************
//This function compute the function (in tree form) with given input *
//********************************************************************
double Compute(node *st,double x[NUMVAR])
{
 node *p,*q;
 BOOL l,r;
 if (!strcmp(st->name,"x0"))
     return x[0];
 if (!strcmp(st->name,"x1"))
      return x[1];
 if (!strcmp(st->name,"x2"))
     return x[2];
 if (!strcmp(st->name,"x3"))
     return x[3];
 if (!strcmp(st->name,"x4"))
     return x[4];
 if (!strcmp(st->name,"x5"))
     return x[5];
 q=st->children;
 if(!strcmp(st->name,"+"))//+
    return (Compute(q,x)+Compute(q->sibling,x));
 if(!strcmp(st->name,"-"))//-
    return (Compute(q,x)-Compute(q->sibling,x));
 if(!strcmp(st->name,"*"))//*
    return (Compute(q,x)*Compute(q->sibling,x));
 if(!strcmp(st->name,"/"))
    return (Compute(q,x)/Compute(q->sibling,x));
 if(!strcmp(st->name,"sin"))//sin
    return sin(Compute(q,x));
 if(!strcmp(st->name,"cos"))//cos
    return cos(Compute(q,x));
 if(!strcmp(st->name,"exp"))//exp
    return exp(Compute(q,x));
    else
        return 0;
}

//*********************************************
//Compute the raw fitness for an invidual     *
//*********************************************
double ComputeRF(individual st)
{
  double sum=0;
  double t;
  int i;
  for(i=0;i<NUMFITCASE;i++)
   {
     t=(Compute(st.chrom,fitcase[i].x)-fitcase[i].y)*(Compute(st.chrom,fitcase[i].x)-fitcase[i].y);
     sum += t;
   }
  return sqrt(sum/NUMFITCASE);
}
//****************************************************************
//This function initializes elementary trees and adjoining table *
//****************************************************************
void LoadTerm()
{
 cout<<endl<<"Loading terms...";
 InitTerm();
 AddTerm("+",2);
 AddTerm("-",2);
 AddTerm("*",2);
 //AddTerm("/",2);
// AddTerm("sin",1);
 //AddTerm("cos",1);
 //AddTerm("exp",1);
 AddTerm("x0",0);
 AddTerm("x1",0);
 AddTerm("x2",0);
 AddTerm("x3",0);
 AddTerm("x4",0);
 AddTerm("x5",0);
 SwapTerm(100);
 cout<<endl<<"Done!";
}
#endif
