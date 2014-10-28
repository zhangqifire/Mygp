//This file contains some global constants and declarations
#ifndef __GCONST__
#define __GCONST__
typedef  unsigned char BOOL;
typedef int SIZE;
typedef unsigned char BYTE;
#define MAXSTRING 60000// max of a tring (for _sbuffer)
#define TRUE 1
#define FALSE 0
#define MAXNAME 6// max+1 length of name of a symbol
#define MAXDEPTH 15// max size of chromosome
#define MAXTOUR 10 //  max tournement size
#define MAXATEMPT 100// max atempt for choosing a site of a chromosome
#define MAXFUNCTION 20
#define MAXTERMINAL  20
#define MAXNODE 3000
#define TRAINFILE "/Users/zq/Desktop/mg_train.dat"
#define NUMFITCASE 500
#define TESTFILE "all_6_in.txt"
#define No_Test 1000
#define NUMVAR  6
#define VOIDVALUE -1523612789.21342
#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))

#endif
