//************************************************************************
//This module contains fitness case and functions to compute raw fitness *
//as well as standardized fitness for Symbolic Regression Problem        *
//Programmed by NXH.                                                     *
//************************************************************************
#ifndef __SYMBOLICREG__
#define __SYMBOLICREG__

#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<assert.h>
#include<math.h>
#include<stdlib.h>
#include "gconst.h"
#include "random.h"
#include "term.h"
#include "stree.h"
#include "genetic.h"

#define INFPLUS  exp(700)
#define INFMINUS -exp(700)
#define PVAL(y) ((y==HUGE_VAL)?(INFPLUS):((y==-HUGE_VAL)?(-INFMINUS):(y)))
//****************************************
//Fitness cases for symbolic regression  *
//They are sampled between 0  and 1      *
//Seed for sampling 12345                *
//****************************************
struct _sample{
		 double x;
		 double y;
		};
typedef struct _sample sample;
sample fitcase[NUMFITCASE];
BOOL SuccPredicate=FALSE;
void SetFitCase()
{
 //FILE *fp;
  double x;
  int i;
//  char filename[50];
//  cout<<endl<<"Input Data File Name:";
//  cin>>filename;
//  fp=fopen(filename,"rt");assert(fp!=NULL);
  for(i=0;i<NUMFITCASE;i++)
   {
    // fscanf(fp,"%lf%lf",&x,&y);
     fitcase[i].x=2*Next_Double()-1;
     x=fitcase[i].x;
     //fitcase[i].y=x*x*x + x*x + x;
    // fitcase[i].y=x*x*x*x+x*x*x+x*x+x;
     fitcase[i].y=x*x*x*x*x + x*x*x*x + x*x*x + x*x +x;
     //fitcase[i].y=x*x*x*x*x*x + x*x*x*x*x + x*x*x*x + x*x*x + x*x + x; 
   }
 // fclose(fp);
}
//********************************************************************
//This function compute the function (in tree form) with given input *
//********************************************************************
double Compute(node *st,double x)
{
 node *p;
 double l,r;
 if (!strcmp(st->name,"X"))
     return x;
 else //st->name="EXP"
  {
    p=st->children;
    if (!strcmp(st->name,"add"))
    {
     l=Compute(p,x);
     r=Compute(p->sibling,x);
     return PVAL(l+r);
    }
   else
   if (!strcmp(st->name,"sub"))
    {
     l=Compute(p,x);
     r=Compute(p->sibling,x);
     return PVAL(l-r);
    }
   else
   if (!strcmp(st->name,"mul"))
    {
     l=Compute(p,x);
     r=Compute(p->sibling,x);
     return PVAL(l*r);
    }
    else
    if (!strcmp(st->name,"div"))
    {
     l=Compute(p,x);
     r=Compute(p->sibling,x);
    if(r==0)
     return 1;
    else
     return PVAL(l/r);
    }
   else
   if (!strcmp(st->name,"sin"))
    {
     l=Compute(p,x);
     return sin(l);
    }
   else
   if (!strcmp(st->name,"cos"))
    {
     l=Compute(p,x);
     return cos(l);
    }
   if (!strcmp(st->name,"ep"))
    {
     l=Compute(p,x);
     return PVAL(exp(l));
    }
   else
    {
     l=Compute(p,x);
    if(l==0)
     return 0;
    else
     return log(fabs(l));
    }
  }
}
//*********************************************
//Compute the raw fitness for an invidual     *
//*********************************************
double ComputeRF(individual st)
{
  double sum=0,t;
  int i;
  int hit;
  hit=0;
  for(i=0;i<NUMFITCASE;i++)
   {
     t=PVAL(fabs(Compute(st.chrom,fitcase[i].x)-fitcase[i].y));
     if(t<=0.01)
       hit++;
     sum+=t;
   }
   if(hit==NUMFITCASE)
     SuccPredicate=TRUE;
   return sum;
}
//****************************************************************
//This function initializes elementary trees and adjoining table *
//****************************************************************
void LoadTerm()
{
 cout<<endl<<"Loading terms...";
 InitTerm();
 AddTerm("add",2);
 AddTerm("sub",2);
 AddTerm("div",2);
 AddTerm("mul",2);
 AddTerm("sin",1);
 AddTerm("cos",1);
 AddTerm("ep",1);
 AddTerm("log",1);
 AddTerm("X",0);
 SwapTerm(100);
 cout<<endl<<"Done!";
}
#endif
