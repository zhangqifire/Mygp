#ifndef __STREE__
#define __STREE__
//**********************************************************************
//This file contains declaration and utility to manipulate sentencial  *                                    *
//trees. Programmed by NXH.                                            *         *
//**********************************************************************
#include<iostream>
#include<string.h>
#include<assert.h>
#include<ctype.h>
#include<stdlib.h>
#include "gconst.h"
using namespace std;
//*********************************************************************
//Declarations and Functions for Node trees                           *
//*********************************************************************
struct _node{
	      char name[MAXNAME];
	      struct _node *sibling, *children;
	      double value; //for ephemeral const
	   };
typedef struct _node node;
//***************************
//Make a Node               *
//***************************
node *MakeNode(char *nname,double value)
{
  node *t;
  t=new node;
  assert(t!=NULL);
  strcpy(t->name,nname);
  t->value=value;
  t->sibling=NULL;
  t->children=NULL;
  return t;
}
//*****************************
//Delete a subtree            *
//*****************************
void DeleteNode(node *t)
{
  node *q,*p;
  if (t)
   {
    if(t->children==NULL)
      delete t;
     else
      {
	p=t->children;
	while(p)
	  {
	    q=p->sibling;
	    DeleteNode(p);
	    p=q;
	  }
	 delete t;
      }
  }
}
//**********************************
//Copy a subtree                   *
//**********************************
node *CopyNode(node *s)
{
  node *t,*p,*q;
  if(s)
   {
     t=MakeNode(s->name,s->value);
     q=s->children;
     t->children=CopyNode(q);
     p=t->children;
     while (q)
      {
	p->sibling=CopyNode(q->sibling);
	q=q->sibling;
	p=p->sibling;
      }
     return t;
   }
  return NULL;
}
//***********************************
//Get the height of a subtree       *
//***********************************
int GetHeightNode(node *t)
{
   if (t->children==NULL)
     return 0;
   else
     {
       unsigned int c,max=0;
       node *p=t->children;
       while (p)
	{
	  c=GetHeightNode(p);
	  if (c>max)
	      max=c;
	  p=p->sibling;
	 }
	return max+1;
      }
}
//*************************************
//Compare two subtree                 *
//*************************************
BOOL NodeCmp(node *t,node *s)
{
 if((t!=NULL) && (s!=NULL))
  {
  if(!strcmp(t->name,s->name))
   {
     node *p=t->children,*q=s->children;
     BOOL b=TRUE;
     while(b && (p!=NULL) && (q!=NULL))
      {
	b=NodeCmp(p,q);
	p=p->sibling;
	q=q->sibling;
      }
      if (b==FALSE)
	  return b;
       else
	  if ((p==NULL)&&(q==NULL))
	   return TRUE;
	  else
	   return FALSE;
   }
   else
    return FALSE;
  }
 else
  if((t==NULL) && (s==NULL))
   return TRUE;
    else
     return FALSE;
}
//*********************************************************
//Travel a Tree in Preorder,Inorder and Postoder          *
//*********************************************************
void (*_doit)(node *n);
void Preorder(node *t)
{
   node *p;
   if(t)
    {
     _doit(t);
      p=t->children;
      while (p)
       {
	 Preorder(p);
	 p=p->sibling;
       }
    }
}
//Travel a Tree in Inoder
void Inorder(node *t)
{
  node *p;
  if (t)
   {
     p=t->children;
     Inorder(p);
     _doit(t);
     while (p)
     {
      p=p->sibling;
      Inorder(p);
     }
   }
}
//Travel a Tree in Postorder
void Postorder(node *t)
{
 node *p;
 if (t)
  {
    p=t->children;
    while (p)
     {
      Postorder(p);
      p=p->sibling;
     }
    _doit(t);
  }
}
//***************************
//Get the size of the tree  *
//***************************
int  _nodecount;
BOOL _countleave;
void NodeCount(node *t)
{
 _nodecount++;
 if(!_countleave && (t->children==NULL))//if don't count the leaves
 _nodecount--;
}
int  GetSizeNode(node *t,BOOL countleave)
{
 _countleave=countleave;
 _nodecount=0;
 _doit=NodeCount;
 Preorder(t);
 return _nodecount;
}
//***************************************
//Get the average branching of a tree   *
//***************************************
int _nodebranch;
void NodeBranchCount(node *t)
{
 node *p;
 BYTE i;
 if(t)
 {
  if (t->children!=NULL)
     {
      i=0;
      p=t->children;
      while(p)
      {
       i++;
       p=p->sibling;
      }
      _nodebranch+=i;
     }
 }
}
double GetAVGNode(node *t)
{
 int size;
 if (t->children==NULL)
   return 1.0;
 size=GetSizeNode(t,FALSE);
 _nodebranch=0;
 _doit=NodeBranchCount;
 Preorder(t);
 return ((double)_nodebranch)/size;
}
//*******************************************************
//Find ith node and its parent (in preorder search)     *
//*******************************************************
node *_idnode;
node *_idnodeparent;
int   _indexnode;
void FindNode(node *t,node *parent)
{
 if(_indexnode>0)
  {
   if(t)
    {
     node *p;
     _indexnode--;
     if (_indexnode==0)
      {
       _idnode=t;
       _idnodeparent=parent;
      }
      else
       {
	p=t->children;
	while(p)
	 {
	  FindNode(p,t);
	  p=p->sibling;
	 }
       }
    }
  }
}
void LocateNode(int i,node *t,node *parent)
{
  _indexnode=i;
  _idnode=NULL;
  _idnodeparent=NULL;
  FindNode(t,parent);
}
//***********************************
//From a string to STree            *
//***********************************
char _sbuff[MAXSTRING];
int  _curchar=0;
node *StringToTree()
{
   char n[MAXNAME];
   int i;
   node *t;
   while(_sbuff[_curchar]==' ')
     _curchar++;
    if(isalpha(_sbuff[_curchar]))
     {
	i=0;
	while (isalnum(_sbuff[_curchar])&&(i<MAXNAME-1))
	 {
	   n[i]=_sbuff[_curchar];
	   i++;_curchar++;
	 }
	n[i]=0;//terminate the string
	t=MakeNode(n,VOIDVALUE);
       while(_sbuff[_curchar]==' ')
	_curchar++;
       switch (_sbuff[_curchar]){
	case '(':
		_curchar++;
		t->children=StringToTree();
		_curchar++;
		while(_sbuff[_curchar]==' ')
		 _curchar++;
	       //finding for the next sibling
		if(_sbuff[_curchar]==',')
		  {
		    _curchar++;
		    t->sibling=StringToTree();
		  }
		return t;
	case ')':
	       //_curchar++;
		return t;
	case ',':
		_curchar++;
		t->sibling=StringToTree();
		return t;
	case 0:
		return t;//new part
	default:
	      cout<<"Error in the string";
	      exit(1);
      }
     }
     else
     {
       cout<<"Error in the string";
       exit(1);
     }
    return NULL;
}
//*************************************
//From a tree to string with brackets *
//*************************************
void TreeToString(node *r)
{
 node *p;
 if (r)
   {
    strcat(_sbuff,r->name);
    if(r->children)
     {
       strcat(_sbuff,"(");
       p=r->children;
       while(p)
	{
	  TreeToString(p);
	  p=p->sibling;
	  if (p)
	   strcat (_sbuff,",");
	}
       strcat(_sbuff,")");
     }
   }
}
void DisplaySTree(node *t)
{
 strcpy(_sbuff,"");
 TreeToString(t);
 cout<<endl<<_sbuff;
}
//*****************************************************
//To check if node p is in the subtree stermed from q *
//*****************************************************
BOOL InNode(node *p, node *q)
{
 node *t;
 t=q;
 if (t==p)
  return TRUE;
 else
  if (t)
   {
    t=t->children;
    while (t)
     if (InNode(p,t))
      return TRUE;
     else
      t=t->sibling;
    }
  return FALSE;
}
//*******************************************************
//This function compare two node it returns             *
//+1 if p is in q,-1 if q is in p, and 0 if incomparable*
//*******************************************************
int NodeOrder(node *p, node *q)
{
 if (InNode(p,q))
  return 1;
 else
  if (InNode(q,p))
    return -1;
  else
   return 0;
}

#endif
