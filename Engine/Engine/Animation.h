#ifndef ANIMATION_H
#define ANIMATION_H

#include "AnimChannel.h"
#include <string>

using namespace std;

class Animation
{
public:
	Animation(void);
	~Animation(void);
	double length;
	double time;
	bool frameBased;
	bool animate;
	bool loop;
	D3DXMATRIX GetTransform();
	void Advance(double t);
	void Restart();
	void SetStart(unsigned int frame);
	void SetEnd(unsigned int frame);
	void SetStartTime(double time);
	void SetEndTime(double time);
	void SetPosition(unsigned int frame);
	void SetPosition(double time);
	AnimKey* posKeys;
	AnimKey* rotKeys;
	AnimKey* scKeys;
	unsigned int posKey;
	unsigned int rotKey;
	unsigned int scaleKey;
	unsigned int numKeyPos;
	unsigned int numKeyRot;
	unsigned int numKeySc;
	D3DXMATRIX scale;
	D3DXMATRIX rot;
	D3DXMATRIX tra;
	string name;
	bool needsUpdate;
	D3DXMATRIX last;
private:
	double origLength;
	unsigned int startFrame;
	unsigned int endFrame;
	double startTime;
	double endTime;
};

#endif
