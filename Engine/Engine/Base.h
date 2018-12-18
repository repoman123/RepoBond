#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "Animation.h"
//#include "Animator.h"
#include "BoundingBox.h"
#include <unordered_map>
using namespace std;

#define degtorad(degree) ((float)(D3DX_PI / 180.0f) * (degree)) // converts from degrees to radians
#define radtodeg(radian) ((float)(180.0f / D3DX_PI) * (radian)) // converts from radians to degrees

class Base
{
public:
	enum ObjectType
	{
		camera,
		mesh,
		light,
		bone,
		base,
		target,
	};
	Base(void);
	~Base(void);
	bool render;
	bool transform;
	void SetMatrix(D3DXMATRIX*);
	void SetAnimStart(unsigned int,bool apToCh);
	void SetAnimEnd(unsigned int,bool apToCh);
	void SetAnimPos(unsigned int,bool apToCh);
	void SetAnimPos(double time,bool apToCh);
	void StopAnimation(bool apToCh,bool apToPr);
	void StartAnimation(bool apToCh,bool apToPr);
	void Rotate(float,float,float);
	void Rotate(D3DXQUATERNION*);
	void Translate(float,float,float);
	void Scale(float,float,float);
	void Offset(float,float,float);
	static Animation* GetCurrentAnim(Base*);
	static D3DXMATRIX* GetMatrix(Base*);
	static void Update(Base*);
	static D3DXVECTOR3 MatrixPosition(D3DXMATRIX*);
	static D3DXVECTOR3 MatrixScaling(D3DXMATRIX*);
	static D3DXQUATERNION MatrixRotation(D3DXMATRIX*);
	void AddAnimation(Animation* anim);
	unsigned int animCount;
	unsigned int currentAnim;
	virtual void SetCurrentAnim(unsigned int,bool apToCh);
	unordered_map <unsigned int,Animation*> animations;
	//Animator* animator;
	Base* parent;
	unordered_map <int,Base*> children;
	unsigned int childCount;
	void AddChild(Base*);
	char* ToChars(const wchar_t*,unsigned int l);
	wchar_t* ToWChars(const char*,unsigned int length);
	bool hasbox;
	string* name;
	ObjectType type;
	bool operator == (const char*& other) const;
	bool hasAnimation;
	BoundingBox* box;
	bool boxUpdate;
	bool hasMatrix;
	D3DXMATRIX Matrix;
	D3DXVECTOR3 rotation,translation,scale;
	D3DXMATRIX rotMat,traMat,scaleMat,finalMat;
};

#endif
