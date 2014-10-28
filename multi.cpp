//Programmed by NXH
//#include<conio.h>//Delete this line if you are in Unix
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<assert.h>
#include "gconst.h"
#include "random.h"
#include "term.h"
#include "stree.h"
#include "genetic.h"
#include "mulplex.h"
//#include "bdtree1.h"
//#include "symreg.h"
//#include "evenpg.h"
int generation,gen=0;
//individual bestsofar;
individual *bestcurrent;
double *average;
//*****************************************************
//This Function initializes for the program           *
//*****************************************************
void Init()
{
 //Initialize global variables
 //Loading Elementary trees and Adjoining Table
  LoadTerm();
 //Initialize population
  poplen=1000;
  oldpop=new individual[poplen];assert(oldpop!=NULL);
  newpop=new individual[poplen];assert(newpop!=NULL);
 //Initialize Probabilities
  pcross=0.9;
  pmutate=0.1;
//Initializes satistics
  ncross=0;
  nmutate=0;
//Number of generations
  generation=100;
  bestcurrent=new individual[generation];assert(bestcurrent!=NULL);
  average=new double[generation];assert(average!=NULL);
//Some global variables
  SuccPredicate=FALSE;
//Set up fitness cases
  SetFitCase();
}
void ComputeFitness()
{
 int i,j, pos;
 individual t;
 double min,sum=0;
 //First Compute Raw fitness
 for(i=0;i<poplen;i++)
   oldpop[i].fitness=ComputeRF(oldpop[i]);
//local search
//  Greedy(100);
 //Second Compute standardized fitness
// for(i=0;i<poplen;i++)
 // oldpop[i].fitness=(double) NUMFITCASE-oldpop[i].fitness;
 //Find the best of the current population
   min=oldpop[0].fitness;
   pos=0;
   sum=0;
   for(i=1;i<poplen;i++)
   {
    if (oldpop[i].fitness<min)
    {
      min=oldpop[i].fitness;
      pos=i;
    }
    sum+=oldpop[i].fitness;
    }
 //copy the best and average
   bestcurrent[gen]=CopyIndividual(oldpop[pos],TRUE);
   average[gen]=sum/poplen;

 //Third Compute Adjusted fitness
   AdjustFitness();
 //Finally Compute nomarlized fitness
   NormalizeFitness();
}
void Evolution()
{
 int i,j,l,k;
 individual *temp;
 //Initialize a population
 RampedInit(6,0.5);
 gen=0;
 while((gen<generation) && !SuccPredicate)
 {
   ComputeFitness();

   cout<<endl<<"generation..."<<gen;//<<"...Best Fitness:"<<bestcurrent[gen].fitness;
   l=0;
  while(l<poplen)
   {
    i=TourSelect(3);
    j=TourSelect(3);
   if(Flip(pcross))
   {
    if (SubTreeSwap(oldpop[i],oldpop[j],newpop[l],newpop[l+1]))
      ncross++;
    else
     {
      newpop[l]=CopyIndividual(oldpop[i],TRUE);
      newpop[l+1]=CopyIndividual(oldpop[j],TRUE);
     }
   }
    else
     {
      newpop[l]=CopyIndividual(oldpop[i],TRUE);
      newpop[l+1]=CopyIndividual(oldpop[j],TRUE);
     }
//mutation test
 if(Flip(pmutate))
  {
    ReplaceSubTree(newpop[l],15,TRUE);
   //PointReplace(newpop[l]);
    nmutate++;
  }
  if(Flip(pmutate))
  {
   ReplaceSubTree(newpop[l+1],15,TRUE);
 //PointReplace(newpop[l+1]);
   nmutate++;
  }
    l+=2;
   }
  gen++;
  for(k=0;k<poplen;k++)
   DeleteNode(oldpop[k].chrom);
  temp=oldpop;
  oldpop=newpop;
  newpop=temp;
 }
}

int main()
{
 int i,j,pos;
 double min;
 long seed;
 char seed1[]="12345";
 double sum=0.0,temp;
 FILE *fp,*fp1,*fp2;
 sample testcase[No_Test];
 InitRanGen();//success key: 12306-tam06,12307-tam07,12309-tam09
 seed=atol(seed1);
 Set_Seed(seed);//some (in many) other keys for successful induction of
 Init();        //the target function are 12347-12352,12371-12374,...
 Evolution();
 cout<<endl<<"Last generation:"<<gen-1;
 cout<<endl<<"Best Individuals:";
 for(i=0;i<gen;i++)
  {
   cout<<endl<<"Generation:"<<i<<endl;
   DisplayIndividual(bestcurrent[i]);
   cout<<endl<<"**********************";
  // getch();
  }
 min=bestcurrent[0].fitness;
 pos=0;
 for(i=1;i<gen;i++)
   if(bestcurrent[i].fitness<min)
    {
     min=bestcurrent[i].fitness;
     pos=i;
    }
  cout<<endl<<"The best found at generation:"<<"("<<pos<<")";
  DisplayIndividual(bestcurrent[pos]);
  cout<<endl<<"Number of crossover:"<<ncross;
  cout<<endl<<"Number of Replacement:"<<nmutate;
  //getch();//Delete this line if you are in Unix
  fp=fopen(TESTFILE,"r");
  assert(fp!=NULL);
  fp1=fopen("expectout.txt","w");
  assert(fp1!=NULL);
  fp2=fopen("systemout.txt","w");
  assert(fp2!=NULL);
  for(i=0;i<No_Test;i++)
  {
     for(j=0;j<NUMVAR;j++)
	   fscanf(fp,"%lf",&testcase[i].x[j]);
     fscanf(fp,"%lf",&testcase[i].y);
     fprintf(fp1,"%lf\n",temp=Compute(bestcurrent[pos].chrom,testcase[i].x));
     fprintf(fp2,"%lf\n",testcase[i].y);
	 sum += (temp-testcase[i].y)*(temp-testcase[i].y);  
  }
  cout<<endl<<"The test RMSE is:"<<sqrt(sum/No_Test);
 // getch();
  fclose(fp);
  fclose(fp1);
  fclose(fp2);
}





