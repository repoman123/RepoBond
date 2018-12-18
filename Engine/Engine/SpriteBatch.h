#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include "SpriteRenderer.h"
#include "Sprite.h"

class SpriteBatch
{
public:
	SpriteBatch(void);
	~SpriteBatch(void);
	bool Initialize(SpriteRenderer*,Sprite*);
	bool Initialize(SpriteRenderer*);
	map <int,Sprite *> sprites;
	void AddSprite(Sprite*);
	bool UpdateSprites();
	bool UpdateTextures();
	void Render();
	SpriteRenderer* sprend;
	unsigned int spriteCount;
private:
	ID3D10Buffer* instBuf;
	bool InitializeBuffers();
	bool UpdateBuffers();
	void RenderBuffers();
	bool resize;
};

#endif
