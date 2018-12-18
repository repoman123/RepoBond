////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
#ifndef CAMERACLASS_H
#define CAMERACLASS_H
//////////////
// INCLUDES //
//////////////
#include "inputclass.h"
#include <d3dx10math.h>
#include "Base.h"

const float moveSpeed=5.0f;
const float turnSpeed=1000.0f;
////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera: public Base
{
public:
	Camera();
	//Camera(aiCamera*);
	Camera(const Camera&);
	~Camera();
	void SetPosition(float, float, float);
	void OffsetPosition(float,float,float);
	D3DXVECTOR3 GetPosition();
	void Render();
	D3DXMATRIX GetView();
	D3DXMATRIX GetProjection();
	D3DXVECTOR3 position;
	D3DXVECTOR3 up;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 rotation;
	void SetAspect(int w,int h);
	bool hasTarget;
	Base* Target;
	float clipNear;
	float clipFar;
	float aspect;
	float fov;
private:
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projMatrix;
};

#endif