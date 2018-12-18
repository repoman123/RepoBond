#include "etext.h"


eText::eText(void)
{
	length=0;
	zval=1.0f;
}

eText::~eText(void)
{
}

void eText::Initialize(const wchar_t* txt,int x,int y,SpriteRenderer* sprend)
{
	this->sprend=sprend;
	textColor = D3DXVECTOR4(0,0,0,1);
	X=x;
	Y=y;
	text.assign(txt);
	this->font = font;
	length = wcslen(txt);
	font = *sprend->font;
	SetTextSize(46);
	if(length != 0)
	InitializeBuffers();
}

bool eText::InitializeBuffers()
{
	HRESULT result;
	SpriteRenderer::TextInstance* instances;
	D3D10_BUFFER_DESC instanceBufferDesc;
	D3D10_SUBRESOURCE_DATA instanceData;
	// Create the instance array.
	instances = new SpriteRenderer::TextInstance[length];
	if(!instances)
	{
		return false;
	}
	unsigned int i =0;
	int tw =0;
	do
	{
		eFont::Character* c = font.GetCharacter(text[i]);
		instances[i].UV.x = c->UV.x;
		instances[i].UV.y = c->UV.y;
		instances[i].charSize.x = (float)c->w;
		instances[i].charSize.y = (float)c->h;
		D3DXVECTOR2 v = sprend->ToScreenPos(X+tw,Y,(unsigned int)(instances[i].charSize.x*fs),(unsigned int)(instances[i].charSize.y*fs));
		instances[i].location.x=v.x;
		instances[i].location.y = v.y;
		instances[i].location.z = zval;
		tw+=(int)(c->w*fs);
		i++;
	}while(i<length);
	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(SpriteRenderer::TextInstance) * length;
	instanceBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;

	// Create the instance buffer.
	result = sprend->device->CreateBuffer(&instanceBufferDesc, &instanceData, &instBuf);
	if(FAILED(result))
	{
		delete [] instances;
		instances = 0;
		return false;
	}

	// Release the instance array now that the instance buffer has been created and loaded.
	delete [] instances;
	instances = 0;
	return true;
}

void eText::RenderBuffers()
{
// Set the vertex and instance buffers to active in the input assembler so it can be rendered.
	unsigned int stride = sizeof(SpriteRenderer::TextInstance);
	unsigned int offset =0;
	sprend->device->IASetVertexBuffers(1, 1,&instBuf,&stride,&offset);
}

void eText::Render()
{
	if(length != 0)
	{
	RenderBuffers();
	
	sprend->textShader->SetFontSize(fs);
	sprend->textShader->SetTexture(sprend->font->Texture);
	sprend->textShader->SetTextColor(textColor);
	sprend->textShader->Render(sprend->device,length);
	}
}

void eText::SetTextSize(unsigned int s)
{
	fs = (float)s/font.fontSize;
	sprend->textShader->SetFontSize(fs);
}

bool eText::UpdateBuffers()
{
	HRESULT result;

	if(lastlength<length)
	{
		//if the text is longer we need to create a new buffer that is bigger
		bool res =InitializeBuffers();
		return res;
	}
	else
	{
	SpriteRenderer::TextInstance* instances = new SpriteRenderer::TextInstance[length];
	//set instances to zero
	memset(instances, 0,(sizeof(SpriteRenderer::TextInstance) * length));
	void* instPtr;
	unsigned int i =0;
	int tw =0;
	do
	{
		eFont::Character* c = font.GetCharacter(text[i]);
		instances[i].UV.x = c->UV.x;
		instances[i].UV.y = c->UV.y;
		instances[i].charSize.x = (float)c->w;
		instances[i].charSize.y = (float)c->h;
		D3DXVECTOR2 v = sprend->ToScreenPos(X+tw,Y,(unsigned int)(instances[i].charSize.x*fs),(unsigned int)(instances[i].charSize.y*fs));
		instances[i].location.x=v.x;
		instances[i].location.y = v.y;
		instances[i].location.z = zval;
		tw+=(int)(c->w*fs);
		i++;
	}while(i<length);
	instPtr=0;
	result = instBuf->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&instPtr);
	if(FAILED(result))
	{
		delete[] instances;
		return false;
	}
	//copy the data to the buffer
	unsigned int width = sizeof(SpriteRenderer::TextInstance) * length;
	memcpy(instPtr, (void*)instances,width);

	instBuf->Unmap();
	delete[] instances;
	return true;
	}
}

bool eText::UpdateText()
{
	lastlength = length;
	length = text.length();
	bool result=true;
	if(length != 0)
	{
	result=UpdateBuffers();
	}
	return result;
}

unsigned int eText::GetTextSize()
{
	return (int)(font.fontSize*fs);
}

unsigned int eText::GetTextLengthInPixels()
{
	unsigned int fos = GetTextSize();
	return MeasureString(text.data(),fos);
}

unsigned int eText::MeasureString(const wchar_t* chs)
{
	unsigned int i =0;
	unsigned int l = wcslen(chs);
	float x=0;
	if(l!=0)
	do
	{
		x+= font.GetCharacter(chs[i])->w;
		i++;
	}while(i<l);
	return (int)ceil(x);
}

unsigned int eText::MeasureString(const char* chs)
{
	unsigned int i =0;
	unsigned int l = strlen(chs);
	float x=0;
	if(l!=0)
	do
	{
		x+=font.GetCharacter((wchar_t)chs[i])->w;
		i++;
	}while(i<l);
	return (int)ceil(x);
}

unsigned int eText::MeasureString(const wchar_t* chs,unsigned int fontSize)
{
	unsigned int i =0;
	unsigned int l = wcslen(chs);
	float fs = (float)fontSize/ font.fontSize;
	float x=0;
	if(l!=0)
	do
	{
		x+= font.GetCharacter(chs[i])->w*fs;
		i++;
	}while(i<l);
	return (int)ceil(x);
}

unsigned int eText::MeasureString(const char* chs,unsigned int fontSize)
{
	unsigned int i =0;
	unsigned int l = strlen(chs);
	float fs =(float)fontSize/ font.fontSize;
	float x=0;
	if(l!=0)
	do
	{
		x+=font.GetCharacter((wchar_t)chs[i])->w*fs;
		i++;
	}while(i<l);
	return (int)ceil(x);
}

