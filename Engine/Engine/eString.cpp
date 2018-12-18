#include "eString.h"


eString::eString(void)
{
}
eString::~eString(void)
{
}

void eString::append(char* chs)
{

}
int eString::indexOf(char c)
{
	unsigned int i =0;
	do
	{
		if(chars[i]==c)
			return i;
		i++;
	}while(i<length);
	return -1;
}

int eString::indexOf(char* cs)
{
	unsigned int i =0;
	do
	{
		if(chars[i]==cs[0])
		{

		}
		return i;
		i++;
	}while(i<length);
	return -1;
}