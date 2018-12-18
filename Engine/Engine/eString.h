#ifndef ESTRING_H
#define ESTRING_H

class eString
{
public:
	eString(void);
	~eString(void);
	char* chars;
	unsigned int length;
	void append(char*);
	void append(eString*);
	void removeAt(int,int);
	int indexOf(char);
	int indexOf(char*);
	int indexOf(eString*);
};

#endif
