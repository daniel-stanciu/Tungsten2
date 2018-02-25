#pragma once
#include "pch.h"
#include <list>
#include "MemoryCache.h"
#include "Sprite.h"
#include "DataContainers.h"

/*

typedef unsigned __int32 Index;
struct SpriteBatch
{
	ID3D11ShaderResourceView* p_texture;
	unsigned int sprite_count;
	unsigned int previous_sprites;
	SpriteBatch(ID3D11ShaderResourceView* p_texture,
		unsigned int sprite_count,
		unsigned int previous_sprites)
	{
		this->sprite_count=sprite_count;
		this->p_texture=p_texture;
		this->previous_sprites=previous_sprites;
	}
};
class SpriteCache
{
private:
	Dictionary< std::list<Sprite*>* >	spriteDictionary;
	MemoryCache<Vertex>	vertex_cache;
	MemoryCache<Index>	index_cache;
	TextureAtlas*		textureAtlas;
	unsigned int		sprite_count;
	int batch_index;
	SpriteBatch currentBatch;
public:
	void Register(Sprite* p_sprite);
	void Register(Sprite** pp_sprite,int number);
	SpriteCache(void);
	void Update();
	inline void GetSize();
	void GetData(MemoryBlock* vertexData,MemoryBlock* indexData);
	void SetTextureAtlas(TextureAtlas* texAtlas);
	SpriteBatch GetNextBatch();
	~SpriteCache(void);
};

*/