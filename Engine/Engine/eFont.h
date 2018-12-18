#ifndef EFONT_H
#define EFONT_H

#include <d3d10.h>
#include "textureclass.h"
#include "dirent.h"
#include <WinSock.h>
#include <Windows.h>
#include <fstream>
using namespace std;

const unsigned int MAX_CHARS = 52428;
const unsigned int MAX_CHAR = 5;
const unsigned int  MAX_PATH_LENGTH = 248;
const unsigned int MAX_FILE_LENGTH = 261;

class eFont
{
public:

	struct Character
	{
		D3DXVECTOR2 UV;
		wchar_t value;
		unsigned int w;
		unsigned int h;
	};
	eFont(void);
	~eFont(void);
	bool Initialize(ID3D10Device*,char*,unsigned int numCs,unsigned int fontSize);
	//ID3D10ShaderResourceView* GetCharecter(wchar_t);
	Character* GetCharacter(wchar_t);
	int GetHeight();
	int Spacing;
	TextureClass* Texture;
	Character* Characters;
	unsigned int fontSize;
	unsigned int CharNum;//the number of characters
private:
	unsigned int* CharIndices;//the index for the texture of a wchar_t
	int h;
};

#endif
