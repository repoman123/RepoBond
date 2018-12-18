#include "eFont.h"

eFont::eFont(void)
{
	Spacing=0;
}

eFont::~eFont(void)
{
}

bool eFont::Initialize(ID3D10Device* device,char* file,unsigned int ncs,unsigned int fontSize)
{
	this->fontSize=fontSize;
	CharNum=ncs;
	ifstream fin;
	unsigned int i;
	char temp;
	// Create the font spacing buffer.
	Characters = new Character[ncs];
	CharIndices = new unsigned int[126];
	// Read in the font size and spacing between chars.
	fin.open(file);
	if(fin.fail())
	{
		return false;
	}
	for(i=0; i<ncs; i++)
	{
		int val =0;
		fin >> val;
		Characters[i].value=val;
		fin >> skipws;
		fin.get(temp);
		fin >> Characters[i].UV.x;
		fin >> skipws;
		fin.get(temp);
		fin >> Characters[i].UV.y;
		fin >> skipws;
		fin.get(temp);
		fin >> Characters[i].w;
		fin >> skipws;
		fin.get(temp);
		fin >> Characters[i].h;
		//we add the index to charindices so we can look up characters
		CharIndices[Characters[i].value]=i;
	}
	// Close the file.
	fin.close();
	Texture = new TextureClass();
	unsigned int l = strlen(file);
	//we change the file extensiln so we can load the 
	//corresponding texture
	string* s = new string();
	s->assign(file);
	s->erase(l-3,3);
	s->append("png");
	wchar_t* ws = new wchar_t[s->length()];
	MultiByteToWideChar(CP_ACP,0,s->data(),-1,ws,s->length());
	ws[s->length()]=0;
	Texture->Initialize(device,ws);
	return true;
}

eFont::Character* eFont::GetCharacter(wchar_t c)
{
	return &Characters[CharIndices[c]];
}

/*
bool eFont::Initialize(ID3D10Device* device,char* file)
{
	char** CharPaths;//the paths for the chars
	DIR *dir;
	CharNum=0;
	Characters = new TextureClass[MAX_CHARS];
	CharIndices = new unsigned int[MAX_CHARS];
	chars = new wchar_t[MAX_CHARS];
	CharPaths = new char*[MAX_CHARS];
	unsigned int i =0;
	do
	{
		CharPaths[i]=new char[MAX_FILE_LENGTH];
		i++;
	}while(i<MAX_CHARS);

	struct dirent *ent;
		dir = opendir (file);
	if (dir != NULL) {

	  parse all the files and directories within directory
	  while ((ent = readdir (dir)) != NULL)
	  {
		  if(ent->d_name[0]!='.')
		  {
				char* fn = new char[MAX_FILE_LENGTH];
				char* s = new char[MAX_CHAR];
				s[MAX_CHAR-1]=0;
				unsigned int l = strlen(ent->d_name);
				unsigned int ii =0;
				fn = ent->d_name;
				i=0;
				ii=0;
				do
				{
					if(ent->d_name[i]!='.' && i<MAX_CHAR)
					{
						s[ii]=fn[i];
						ii++;
					}
					else
					{
						if(s[0]>0)
						{
						int ci = atoi(s);
						if(ci<52428)
						{
						CharIndices[ci]=CharNum;
						memcpy(CharPaths[ci],fn,strlen(fn)+1);
						chars[CharNum]=ci;
						CharNum++;
						}
						}
						i=MAX_FILE_LENGTH;
					}
					i++;
				}while(i<MAX_FILE_LENGTH);
		  }
	  }
	  closedir (dir);
	} else {
	  //could not open directory
	  perror ("");
	  return false;
	}
	i=0;
	Textures = new ID3D10ShaderResourceView*[CharNum];
	do
	{
		string* st = new string();
		st->assign(file);
		char* name = CharPaths[chars[i]];
		st->append(name);
		wchar_t* pw = new wchar_t[MAX_PATH];
		unsigned int ci = strlen(st->data());
		unsigned int cii =0;
		const char* chs = st->data();
		do
		{
			wchar_t ch = (wchar_t)chs[cii];
			pw[cii]=ch;
			cii++;
		}while(cii<ci);
		pw[ci]=0;
		Characters[i].Initialize(device,pw);
		Textures[i]=Characters[i].GetTexture();
		delete st;
		i++;
	}while(i<CharNum);
	//the characters should have same height so we set font height to be that of character one
	h = Characters[0].h;
	delete CharPaths;

	

	return true;
}
*/

int eFont::GetHeight()
{
	return h;
}

/* bool eFont::Initialize(ID3D10Device* device,wchar_t* file)
{
	wchar_t** CharPaths;//the paths for the chars
	DIR *dir;
	CharNum=0;
	Characters = new TextureClass[MAX_CHARS];
	CharIndices = new unsigned int[MAX_CHARS];
	chars = new wchar_t[MAX_CHARS];
	CharPaths = new wchar_t*[MAX_CHARS];
	unsigned int i =0;
	do
	{
		CharPaths[i]=new wchar_t[MAX_FILE_LENGTH];
		i++;
	}while(i<MAX_CHARS);

	struct dirent *ent;
		dir = opendir (file);
	if (dir != NULL) {

	  parse all the files and directories within directory

	  while ((ent = readdir (dir)) != NULL)
	  {
		wchar_t* fn = new wchar_t[MAX_FILE_LENGTH];
		wchar_t* s = new wchar_t[MAX_CHAR];
		s[MAX_CHAR-1]=0;
		fn=ent->d_name;
		i=0;
		unsigned int ii =0;
		do
		{
			if(ent->d_name[i]!='.' && i<MAX_CHAR)
			{
				s[ii]=fn[i];
				ii++;
			}
			else
			{
				if(s[0]>0)
				{
				int ci = _wtoi(s);
				CharIndices[ci]=CharNum;
				memcpy(CharPaths[ci],fn,wcslen(fn)+1);
				chars[CharNum]=ci;
				CharNum++;
				}
				i=MAX_FILE_LENGTH;
			}
			i++;
		}while(i<MAX_FILE_LENGTH);
	  }
	  closedir (dir);
	} else {
	  could not open directory
	  perror ("");
	  return false;
	}
	i=0;
	do
	{
		wstring* st = new wstring();
		st->assign(file);
		st->append(CharPaths[chars[i]]);
		Characters[i].Initialize(device,st);
		delete st;
		i++;
	}while(i<CharNum);
	return true;
	delete CharPaths;
}
*/





/*

ID3D10ShaderResourceView* eFont::GetCharecter(wchar_t c)
{
	int ci = CharIndices[(int)c];
	return Characters[ci].GetTexture();
}
*/
/*
TextureClass* eFont::GetTexture(wchar_t c)
{
	TextureClass* tex;
	unsigned int i = CharIndices[(int)c];
	if(i<52428)
	tex = &Characters[i];
	else
	tex = &Characters[CharIndices[0]];
	return tex;
}
*/
