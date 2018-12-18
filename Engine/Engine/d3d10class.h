////////////////////////////////////////////////////////////////////////////////
// Filename: D3DClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef D3DClass_H
#define D3DClass_H
// INCLUDES //
//////////////

#include <D3D10.h>
#include <D3DX10math.h>
#include <dxgi.h>

const D3D10_CULL_MODE CULLMODE = D3D10_CULL_BACK;
////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();
	void TurnZBufferOn();
	void TurnZBufferOff();
	bool fullScreen;
	bool Initialize(int, int, bool, HWND, bool, float, float);
	int height;
	int width;
	void Dispose();
	void BeginScene(float, float, float, float);
	void EndScene();
	ID3D10Device* GetDevice();
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);
	void GetVideoCardInfo(char*, int&);
	void TogleFullScreen();
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D10Device* m_device;
	ID3D10RenderTargetView* m_renderTargetView;
	ID3D10Texture2D* m_depthStencilBuffer;
	ID3D10DepthStencilState* m_depthStencilState;
	ID3D10DepthStencilView* m_depthStencilView;
	ID3D10DepthStencilState* m_depthDisabledStencilState;
	ID3D10RasterizerState* m_rasterState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
};
#endif