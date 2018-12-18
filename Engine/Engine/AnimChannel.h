#ifndef ANIMCHANNEL_H
#define ANIMCHANNEL_H

#include "AnimKey.h"

class AnimChannel
{
public:
	AnimChannel(void);
	~AnimChannel(void);
	AnimKey* posKeys;
	AnimKey* rotKeys;
	AnimKey* scKeys;
	int numKeyPos;
	int numKeyRot;
	int numKeySc;
};

#endif
