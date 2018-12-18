#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <fstream>
using namespace std;
#include <D3DX10math.h>
const int HMapScale=50;
class HeightMap
{
public:
	HeightMap(D3DXVECTOR3* pos,float w,float h);
	void Initialize(D3DXVECTOR3* pos,float w,float h);
	HeightMap();
	~HeightMap(void);
	bool Load(const wchar_t* path);
	bool Save(const wchar_t* path);
	float GetHeight(float,float);
	float GetHeight(D3DXVECTOR3*);
	unsigned int width;
	unsigned int height;
	float** data;
private:
	int x;
	int y;
	float z;
};

#endif