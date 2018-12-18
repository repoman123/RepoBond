////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Camera.h"


Camera::Camera()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;
	Target=0;
	hasTarget=false;
}

Camera::Camera(const Camera& other)
{

}
Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	return;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return D3DXVECTOR3(position.x,position.y,position.z);
}

void Camera::OffsetPosition(float x,float y,float z)
{
	position.x+=x;
	position.y+=y;
	position.z+=z;
}

void Camera::Render()
{
	D3DXMATRIX* mat = Base::GetMatrix(this);
	if(hasTarget)
	{
		lookAt=D3DXVECTOR3(0,0,0);
		D3DXVec3TransformCoord(&lookAt,&lookAt,Base::GetMatrix(Target));
	}
	position=D3DXVECTOR3(0,0,0);
	D3DXVec3TransformCoord(&position, &position,mat);
	//if(free)
	//{
		//D3DXVec3TransformNormal(&lookAt,&lookAt,&mat);
		//D3DXVECTOR3 v=lookAt+position;
		//D3DXMatrixLookAtLH(&m_viewMatrix, &position, &v, &up);
	//}
	//else
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
	D3DXMatrixPerspectiveFovLH(&m_projMatrix,fov,aspect,clipNear,clipFar);
	mat=0;
	delete mat;
	return;
}

D3DXMATRIX Camera::GetView()
{
	return m_viewMatrix;
}

D3DXMATRIX Camera::GetProjection()
{
	return m_projMatrix;
}

void Camera::SetAspect(int w,int h)
{
	aspect = float(w) / float(h);
}