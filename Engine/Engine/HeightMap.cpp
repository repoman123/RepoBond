#include "HeightMap.h"


HeightMap::HeightMap(void)
{
}
HeightMap::~HeightMap(void)
{
}

HeightMap::HeightMap(D3DXVECTOR3* pos,float w,float h)
{
	this->x= (unsigned int)floor(pos->x);
	this->y= (unsigned int)floor(pos->y);
	w=floor(w);
	h=floor(h);
	z=pos->z;
	w=w/HMapScale;
	h=h/HMapScale;
	width=(unsigned int)w;
	height=(unsigned int)h;
	unsigned int i =0;
	data = new float*[width];
	do
	{
		data[i]=new float[height];
		i++;
	}while(i<w);
}

void HeightMap::Initialize(D3DXVECTOR3* pos,float w,float h)
{
	x= (unsigned int)floor(pos->x);
	y= (unsigned int)floor(pos->y);
	w=floor(w);
	h=floor(h);
	z=pos->z;
	w=w/HMapScale;
	h=h/HMapScale;
	width=(unsigned int)w;
	height=(unsigned int)h;
	unsigned int i =0;
	data = new float*[width];
	do
	{
		data[i]=new float[height];
		i++;
	}while(i<w);
}

float HeightMap::GetHeight(float fx,float fy)
{
	int pX=(int)(x-(floor(fx)))/HMapScale;
	int pY=(int)(y-(floor(fy)))/HMapScale;
	if(pX < (int)width && pY < (int)height)
		return z-data[pX][pY];
	return 0;
}

float HeightMap::GetHeight(D3DXVECTOR3* pos)
{
	this;
	int pX=(int)(x-(floor(pos->x)))/HMapScale;
	int pY=(int)(y-(floor(pos->y)))/HMapScale;
	if(pX < (int)width && pY < (int)height)
		return z-data[pX][pY];
	return 0;
}

bool HeightMap::Load(const wchar_t* path)
{
	ifstream fin;
	fin.open(path);
	if(fin.fail())
		return false;
	fin.ignore(256,'=');
	fin >> width;

	fin.ignore(256,'=');
	fin >> height;

	fin.ignore(256,'=');
	fin >> x;

	fin.ignore(256,'=');
	fin >> y;

	fin.ignore(256,'=');
	fin >> z;

	unsigned int i =0;
	data = new float*[width];
	do
	{
		data[i]=new float[height];
		i++;
	}while(i<width);
	i=0;
	unsigned int ii =0;
	do
	{
		ii=0;
		do
		{
			fin >> data[i][ii];
			ii++;
		}while(ii<height);
		i++;
	}while(i<width);
	fin.close();
	return true;
}

bool HeightMap::Save(const wchar_t* path)
{
	ofstream fout;
	fout.open(path);
	if(fout.fail())
		return false;
	fout << "Width=";
	fout << width;
	fout.put('\n');
	fout << "Height=";
	fout << height;
	fout.put('\n');
	fout << "X=";
	fout << x;
	fout.put('\n');
	fout << "Y=";
	fout << y;
	fout.put('\n');
	fout << "Z=";
	fout << z;
	fout.put('\n');
	unsigned int i=0;
	unsigned int ii =0;
	do
	{
		ii=0;
		do
		{
			fout << data[i][ii];
			fout.put('\n');
			ii++;
		}while(ii<height);
		i++;
	}while(i<width);
	fout.flush();
	fout.close();
	return true;
}
