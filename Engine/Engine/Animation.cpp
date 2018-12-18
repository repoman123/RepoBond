#include "Animation.h"

Animation::Animation()
{
	posKey=0;
	rotKey=0;
	scaleKey=0;
	needsUpdate=true;
	frameBased=false;
	time=0;
	startTime=0;
	loop=true;
}

Animation::~Animation(void)
{
}

D3DXMATRIX Animation::GetTransform()
{
	this;
	if(needsUpdate)
	{
	//	if(posKey > this->numKeyPos && rotKey > this->numKeyRot && scaleKey > this->numKeySc)
		//{
			//posKey=this->numKeyPos-1;
			//rotKey=this->numKeyRot-1;
			//scaleKey=this->numKeySc-1;
		//}
	D3DXMatrixTranslation(&tra,posKeys[posKey].value->x ,posKeys[posKey].value->y ,posKeys[posKey].value->z);
	D3DXMatrixRotationQuaternion(&rot,rotKeys[rotKey].rotvalue);
	D3DXMatrixScaling(&scale,scKeys[scaleKey].value->x,scKeys[scaleKey].value->y,scKeys[scaleKey].value->z);
	last = scale*rot*tra;
	}
	needsUpdate=false;
	return last;
}

void Animation::Advance(double t)
{
	if(animate)
	{
		time+=t/60;
		if(time>=endTime)
			if(loop)
			Restart();
			else
			animate=false;
		if(numKeyPos>posKey+1)
		if(posKeys[posKey].time <= time)
		{
			posKey++;
			needsUpdate=true;
		}
		if(numKeyRot>rotKey+1)
		if(rotKeys[rotKey].time <= time)
		{
			rotKey++;
			needsUpdate=true;
		}
		if(numKeySc>scaleKey+1)
		if(scKeys[scaleKey].time <= time)
		{
			scaleKey++;
			needsUpdate=true;
		}
	}
}

void Animation::Restart()
{
	time=startTime;
	scaleKey=startFrame;
	rotKey=startFrame;
	posKey=startFrame;
}

void Animation::SetStart(unsigned int frame)
{
	unsigned int frames=0;
	if(numKeyPos > frames)
		frames = numKeyPos;
	if(numKeyRot > frames)
		frames = numKeyRot;
	if(numKeySc > frames)
		frames = numKeySc;
	if(frames > frame)
	{
	startTime = (length / frames)*frame;
	startFrame=frame;
	scaleKey=startFrame;
	rotKey=startFrame;
	posKey=startFrame;
	this->time=startTime;
	}
}

void Animation::SetEnd(unsigned int frame)
{
	unsigned int frames=0;
	if(numKeyPos > frames)
		frames = numKeyPos;
	if(numKeyRot > frames)
		frames = numKeyRot;
	if(numKeySc > frames)
		frames = numKeySc;
	if(frames > frame)
	{
	endTime = (length / frames)*frame;
	endFrame=frame;
	}
}

void Animation::SetStartTime(double time)
{
	unsigned int frames=0;
	if(numKeyPos > frames)
		frames = numKeyPos;
	if(numKeyRot > frames)
		frames = numKeyRot;
	if(numKeySc > frames)
		frames = numKeySc;
	startTime=time;
	startFrame=(unsigned int)(time / (length/frames));
	scaleKey=startFrame;
	rotKey=startFrame;
	posKey=startFrame;
	this->time=startTime;
	int test=0;
}

void Animation::SetEndTime(double time)
{
	unsigned int frames=0;
	if(numKeyPos > frames)
		frames = numKeyPos;
	if(numKeyRot > frames)
		frames = numKeyRot;
	if(numKeySc > frames)
		frames = numKeySc;
	
	unsigned int frame = (unsigned int)(time / (length/frames));
	if(frames > frame)
	{	
		endFrame = frame;
		endTime=time;
	}
	int test=0;
}

void Animation::SetPosition(unsigned int frame)
{
	unsigned int frames=0;
	if(numKeyPos > frames)
		frames = numKeyPos;
	if(numKeyRot > frames)
		frames = numKeyRot;
	if(numKeySc > frames)
		frames = numKeySc;
	if(frames > frame)
	{
	this->time = posKeys[posKey].time;
	posKey=frame;
	rotKey=frame;
	scaleKey=frame;
	Advance(0);
	}
}

void Animation::SetPosition(double time)
{
	this->time=time;
	Advance(0);
}
