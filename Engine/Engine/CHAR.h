
//this is a custom unicode 32bit struct
#ifndef CHAR_H
#define CHAR_H

#include <aiTypes.h>
#include <d3dx10math.h>
typedef unsigned int CHAR;
//declerations
static unsigned int clen(const CHAR*);
static int cAt(const CHAR*,const CHAR);
static int cAt(const CHAR*,const CHAR*,int cl);
static void cf(const CHAR*,float);
static void cd(const CHAR*,double);
static void cs(const CHAR*,short);
static void ci(const CHAR*,int);
static void cl(const CHAR*,long);
//functions
static unsigned int clen(const CHAR* cs)
{
	unsigned int i =0;
	do
	{
		i++;
	}while(cs[i]!=0);
	return i;
}

static int cAt(const CHAR* cs,const CHAR c)
{
	unsigned int i =0;
	do
	{
		if(cs[i]==c)
			return i;
		i++;
	}while(cs[i]!=0);
	return -1;
}

static int cAt(const CHAR* cs,const CHAR* cc,int cl)
{
	unsigned int i =0;
	unsigned int ii =0;
	do
	{
		if(cs[i]==cc[ii])
		{
			ii++;
			if(ii=cl)
				return i;
		}
		else
			ii=0;
		i++;
	}while(cs[i]!=0);
	return -1;
}

static void cf(const CHAR*,float)
{
	if(
}
static void 
#endif
