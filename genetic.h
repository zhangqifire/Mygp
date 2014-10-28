//**************************************************************************
//This file contains individual,population representation and genetic      *
//operators.(Programmed by NXH).                                           *
//**************************************************************************
#ifndef __GENETIC__
#define __GENETIC__

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<math.h>
#include "gconst.h"
#include "random.h"
#include "term.h"
using namespace std;
//Declaration for individual and population
struct _individual {
		      node *chrom;//chromosome of individual
		      int size,height;//size and heigh of individual
		      double branch;//branching of individual
		      double fitness;//fitness of individual
		   };
typedef struct _individual individual;
individual *oldpop,*newpop;//New and old populations
unsigned int poplen; //population length
double pcross,pmutate,pcopy;//probability for genetic operations
long ncross,nmutate;//statistics
//*********************
//Copy an individual  *
//*********************
individual CopyIndividual(individual t,BOOL copychrom)
{
 individual v;
 if(copychrom)
  v.chrom=CopyNode(t.chrom);
 else
  v.chrom=t.chrom;
  v.size=t.size;
  v.height=t.height;
  v.fitness=t.fitness;
  v.branch=t.branch;
 return v;
}
void DisplayIndividual(individual v)
{
 cout<<endl<<"-----------------------------";
 cout<<endl<<"Genotype Structure:";
 DisplaySTree(v.chrom);
 cout<<endl<<"Size:"<<v.size;
 cout<<endl<<"Height:"<<v.height;
 cout<<endl<<"Branching:"<<v.branch;
 cout<<endl<<"Fitness:"<<v.fitness;
}
node *GrowthTreeGen(int maxdepth)
{
int i;
BYTE a;
node *t,*p;
if(maxdepth==0) //to the limit then choose a terminal
    {
     i=IRandom(0,gltcard-1);
     t=MakeNode(glterminal[i].name,VOIDVALUE);
     return t;
    }
  else //choosing from function and terminal
   {
    i=IRandom(0,gltermcard-1);
    t=MakeNode(glterm[i].name,VOIDVALUE);
    if(glterm[i].arity>0) //if it is function
    {
     t->children=GrowthTreeGen(maxdepth-1);
     p=t->children;
     for(a=1;a<glterm[i].arity;a++)
       {
	p->sibling=GrowthTreeGen(maxdepth-1);
	p=p->sibling;
       }
     p->sibling=NULL;
    }
    return t;
   }
}
node *FullTreeGen(int maxdepth)
{
int i;
BYTE a;
node *t,*p;
if(maxdepth==0) //to the limit then choose a terminal
    {
     i=IRandom(0,gltcard-1);
     t=MakeNode(glterminal[i].name,VOIDVALUE);
     return t;
    }
  else //choosing from function
   {
    i=IRandom(0,glfcard-1);
    t=MakeNode(glfunction[i].name,VOIDVALUE);
     t->children=FullTreeGen(maxdepth-1);
     p=t->children;
     for(a=1;a<glfunction[i].arity;a++)
       {
	p->sibling=FullTreeGen(maxdepth-1);
	p=p->sibling;
       }
     p->sibling=NULL;
    return t;
   }
}
//*****************************************************
//Ramp Initialization with a percentage of full tree  *
//*****************************************************
void RampedInit(int depth,double percentage)//(minl>=2)
{
 int i,j;
 BYTE k;
 node *t,*p;
 for(i=0;i<poplen;i++)
  {
   //choose randomly from the function set
    j=IRandom(0,glfcard-1);
    t=MakeNode(glfunction[j].name,VOIDVALUE);
   if(Next_Double()<percentage)//Growth
   {
    t->children=GrowthTreeGen(depth-1);
    p=t->children;
    for(k=1;k<glfunction[j].arity;k++)
    {
     p->sibling=GrowthTreeGen(depth-1);
     p=p->sibling;
    }
    p->sibling=NULL;
   }
   else
    {
     t->children=FullTreeGen(depth-1);
     p=t->children;
    for(k=1;k<glfunction[j].arity;k++)
    {
      p->sibling=FullTreeGen(depth-1);
      p=p->sibling;
     }
     p->sibling=NULL;
    }
   oldpop[i].chrom=t;
   oldpop[i].size=GetSizeNode(t,TRUE);
   oldpop[i].height=GetHeightNode(t);
   oldpop[i].branch=GetAVGNode(t);
  
  }
}
//************************************************
//This function computes Adjusted Fitness        *
//This assumes that the fitness of an individual *
//is already standardized (it means the smaller) *
//the better)                                    *
//************************************************
void AdjustFitness()
{
  int i;
  for(i=0;i<poplen;i++)
   oldpop[i].fitness=1/(1+oldpop[i].fitness);
}
//***********************************************
//This function computes normalized fitness it  *
//supposes that the fitness of an individual is *
//already adjusted (it means between 0 - 1 and  *
//The bigger the better)                        *
//***********************************************
void NormalizeFitness()
{
  double sum=0;
  int i;
  for(i=0;i<poplen;i++)
   sum+=oldpop[i].fitness;
  for(i=0;i<poplen;i++)
   oldpop[i].fitness=oldpop[i].fitness/sum;
}
//*************************************************
//Selections works on Normalized Fitness          *
//*************************************************
//Propotionate Selection
int ProSelect()
{
  double k=Next_Double();
  int i=0;
  while((i<poplen-1)&&(k>0))
   {
     k-=oldpop[i].fitness;
     if(k>0)
      i++;
   }
   return i;
}
//Tournement Selection
int TourSelect(int size)
{
	int i,j,pos=0;
	double max=-1000;
	assert(size<=MAXTOUR);
	for(i=0;i<size;i++)
    {
		j=IRandom(0,poplen-1);
		if (oldpop[j].fitness>max)
		{
			max=oldpop[j].fitness;
			pos=j;
		}
    }
	return pos;
}
//Linear Ranking Selection
//**************************************************
//Crossover Section                                *
//**************************************************
BOOL SubTreeSwap(individual parent1,individual parent2,individual &child1,individual &child2)
{
 individual temp1, temp2;
 int count, crosspoint1,crosspoint2;
 node *node1,*node2,*t;
 double valuetemp;
 char nametemp[MAXNAME];
 count=0;
 while (count<MAXATEMPT)
   {
    //copy the parents
    temp1=CopyIndividual(parent1,TRUE);
    temp2=CopyIndividual(parent2,TRUE);
    //attemp to make crossover
    crosspoint1=IRandom(2,temp1.size);//excluding the root
    crosspoint2=IRandom(2,temp2.size);
    LocateNode(crosspoint1,temp1.chrom,NULL);
    node1=_idnode;
    LocateNode(crosspoint2,temp2.chrom,NULL);
    node2=_idnode;
    //try to do crossover
    //first swaping contents
    strcpy(nametemp,node1->name);
    strcpy(node1->name,node2->name);
    strcpy(node2->name,nametemp);
    valuetemp=node1->value;
    node1->value=node2->value;
    node2->value=valuetemp;
   //then swap the children
    t=node1->children;
    node1->children=node2->children;
    node2->children=t;
   //compute the heigh after that
    temp1.height=GetHeightNode(temp1.chrom);
    temp2.height=GetHeightNode(temp2.chrom);
    if((temp1.height<=MAXDEPTH)&&(temp2.height<=MAXDEPTH))
     {
      temp1.size=GetSizeNode(temp1.chrom,TRUE);
      temp1.branch=GetAVGNode(temp1.chrom);
      temp2.size=GetSizeNode(temp2.chrom,TRUE);
      temp2.branch=GetAVGNode(temp2.chrom);
      child1=CopyIndividual(temp1,FALSE);
      child2=CopyIndividual(temp2,FALSE);
      return TRUE;
     }
    else
     {
      DeleteNode(temp1.chrom);
      DeleteNode(temp2.chrom);
     }
    count++;
   }
return FALSE;
}
//**************************************************
//Mutation Section                                 *
//**************************************************************
BOOL ReplaceSubTree(individual &child,int maxdepth,BOOL type)
{
 individual temp;
 int replacepoint,count;
 node *node1,*parentnode1,*t,*p;
 count=0;
while(count<MAXATEMPT)
{
 temp=CopyIndividual(child,TRUE);
 replacepoint=IRandom(2,temp.size);//excluding the root
 LocateNode(replacepoint,temp.chrom,NULL);
 node1=_idnode;
 parentnode1=_idnodeparent;
 if (type) //growth
   t=GrowthTreeGen(maxdepth);
 else
   t=FullTreeGen(maxdepth);
  p=parentnode1->children;
  if(p==node1)//first child
    parentnode1->children=t;
   else
    {
     while(p->sibling!=node1)
      p=p->sibling;
      p->sibling=t;
    }
   t->sibling=node1->sibling;
 DeleteNode(node1);
 temp.height=GetHeightNode(temp.chrom);
 if(temp.height<=MAXDEPTH)
  {
   DeleteNode(child.chrom);
   child.chrom=temp.chrom;
   child.height=temp.height;
   child.size=GetSizeNode(child.chrom,TRUE);
   temp.branch=GetAVGNode(child.chrom);
   return TRUE;
  }
 DeleteNode(temp.chrom);
 count++;
 }
return FALSE;
}
BOOL ReplaceSubTree2(individual &child,int maxdepth,int inimaxdepth,BOOL type)
{
 individual temp;
 int replacepoint,count;
 node *node1,*parentnode1,*t,*p;
 count=0;
while(count<MAXATEMPT)
{
 temp=CopyIndividual(child,TRUE);
 replacepoint=IRandom(1,temp.size);//including the root
 LocateNode(replacepoint,temp.chrom,NULL);
 node1=_idnode;
 parentnode1=_idnodeparent;
if(replacepoint==1)
{
 if (type) //growth
   t=GrowthTreeGen(IRandom(1,maxdepth));
 else
   t=FullTreeGen(IRandom(1,maxdepth));
}
else
{
 if (type) //growth
   t=GrowthTreeGen(IRandom(0,maxdepth));
 else
   t=FullTreeGen(IRandom(0,maxdepth));
}
 if (replacepoint==1)//at the root
    temp.chrom=t;
 else
  {
   p=parentnode1->children;
   if(p==node1)//first child
     parentnode1->children=t;
    else
     {
      while(p->sibling!=node1)
       p=p->sibling;
      p->sibling=t;
     }
   t->sibling=node1->sibling;
  }
 DeleteNode(node1);
 temp.height=GetHeightNode(temp.chrom);
 if(temp.height<=MAXDEPTH)
  {
   DeleteNode(child.chrom);
   child.chrom=temp.chrom;
   child.height=temp.height;
   child.size=GetSizeNode(child.chrom,TRUE);
   temp.branch=GetAVGNode(child.chrom);
   return TRUE;
  }
 DeleteNode(temp.chrom);
 count++;
 }
return FALSE;
}

BOOL PointReplace(individual &child)
{
 int count, replacepoint,i;
 node *node1,*p;
 BYTE arity;
   replacepoint=IRandom(1,child.size);//including the root
   LocateNode(replacepoint,child.chrom,NULL);
   node1=_idnode;
   //find the arity
   p=node1->children;
   arity=0;
   while(p)
    {
     arity++;
     p=p->sibling;
    }
   //find the number of term having the same arity
   count=0;
   for(i=0;i<gltermcard;i++)
    if(glterm[i].arity==arity)
     count++;
  //find one of them at random
   count=IRandom(1,count);
   i=0;
   while(count>0)
    {
     if(glterm[i].arity==arity)
      count--;
     if(count>0)
      i++;
    }
 //replace the node
   strcpy(node1->name,glterm[i].name);
   return TRUE;
}
#endif
