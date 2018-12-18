#ifndef GUN_H
#define GUN_H

#include "modelclass.h"

class Gun : public Model
{
public:
	Gun(void);
	~Gun(void);
	string name;
	unsigned int magSize;
	bool hasScope;
	bool twoHanded;
	Base* handle1;
	Base* handle2;
	unsigned int barreldirection,barrel;
	unsigned int animShootStart,animShootEnd,animPrepStart,animPrepEnd;
	D3DXVECTOR3 scale;
	Ray dir;
	virtual void Update();
	float scopeMinZoom;
	float scopeMaxZoom;
	float damage;
};

#endif