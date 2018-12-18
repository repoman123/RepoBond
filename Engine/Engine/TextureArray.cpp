#include "TextureArray.h"


TextureArray::TextureArray(void)
{
	texCount=0;
	unsigned int i=0;
	ppTex2D = NULL;
	ppSRV = NULL;
}
TextureArray::~TextureArray(void)
{
}

void TextureArray::AddTexture(const wchar_t* file)
{
	texturePaths.insert(pair<unsigned int,const wchar_t*>(texCount,file));
	texCount++;
}
/*
void TextureArray::AddTexture(ID3D10Device* device,wchar_t* path)
{
	 D3DX10CreateShaderResourceViewFromFile(device,path, NULL, NULL, &texs[texCount], NULL);
	texCount++;
}
*/

/*
unsigned int TextureArray::GetIndexOf(const wchar_t* name)
{
	unsigned int i=0;
	do
	{
		i++;
	}while(wcscmp(name,Textures[i]->name.data())==0);
	return i;
}
*/

HRESULT TextureArray::UpdateTextureArray(ID3D10Device* pd3dDevice)
{
    HRESULT hr = S_OK;
    D3D10_TEXTURE2D_DESC desc;
    ZeroMemory( &desc, sizeof(D3D10_TEXTURE2D_DESC) );
    for(int i=0; i<(int)texCount; i++)
    {
        ID3D10Resource *pRes = NULL;
        D3DX10_IMAGE_LOAD_INFO loadInfo;
        ZeroMemory( &loadInfo, sizeof( D3DX10_IMAGE_LOAD_INFO ));
        loadInfo.Width = 510;
        loadInfo.Height  = 510;
        loadInfo.FirstMipLevel = 1;
        loadInfo.Usage = D3D10_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
        loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        D3DX10CreateTextureFromFile( pd3dDevice,texturePaths[i], &loadInfo, NULL, &pRes, &hr );

        if( pRes )
        {
            ID3D10Texture2D* pTemp;
            pRes->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&pTemp );
            pTemp->GetDesc( &desc );

            D3D10_MAPPED_TEXTURE2D mappedTex2D;
            if(DXGI_FORMAT_R8G8B8A8_UNORM != desc.Format)   //make sure we're R8G8B8A8
                return false;

            if(desc.MipLevels > 4)
                desc.MipLevels -= 4;
            if(!(ppTex2D))
            {
				texSizes.insert(pair<unsigned int,D3DXVECTOR2>(texCount,D3DXVECTOR2((float)desc.Width,(float)desc.Height)));
                desc.Usage = D3D10_USAGE_DEFAULT;
                desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
                desc.CPUAccessFlags = 0;
                desc.ArraySize = texCount;
                pd3dDevice->CreateTexture2D( &desc, NULL, &ppTex2D);
            }

            for(UINT iMip=0; iMip < desc.MipLevels; iMip++)
            {
                pTemp->Map( iMip, D3D10_MAP_READ, 0, &mappedTex2D );

                pd3dDevice->UpdateSubresource(ppTex2D, 
                    D3D10CalcSubresource( iMip, i, desc.MipLevels ),
                    NULL,
                    mappedTex2D.pData,
                    mappedTex2D.RowPitch,
                    0 );

                pTemp->Unmap( iMip );
            }
			pRes=0;
			pTemp=0;
            delete pRes;
            delete pTemp;
        }
        else
        {
            return false;
        }
    }

    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
    SRVDesc.Format = desc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
    SRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
    SRVDesc.Texture2DArray.ArraySize = texCount;
    pd3dDevice->CreateShaderResourceView(ppTex2D, &SRVDesc, &ppSRV );

    return hr;
}

unsigned int TextureArray::GetTexCount()
{
	return texCount;
}