#include "pch.h"
#include "SpriteCache.h"
/*

SpriteCache::SpriteCache()
	:currentBatch(NULL,0,0)
{
	this->sprite_count=0;
	this->batch_index=0;
}


SpriteCache::~SpriteCache(void)
{
}

void SpriteCache::Register(Sprite* p)
{
	std::wstring tag=p->tag;
	std::list<Sprite*>** p_list=spriteDictionary.Seek(tag);
	if(p_list==NULL)
	{
		if(!textureAtlas->GetTexture(tag)) return;
		spriteDictionary.Register(new std::list<Sprite*>(),tag);
		p_list=spriteDictionary.Seek(tag);
	}
	(*p_list)->push_back(p);
	sprite_count++;
	return;
	
}

void SpriteCache::Register(Sprite** pp_sprite,int number)
{
	while(number>0)
	{
		Sprite* p=*pp_sprite;
		Register(p);
		number--;
		pp_sprite++;
	}
}

void SpriteCache::Update()
{
	vertex_cache.ResetCapacity(sprite_count*4);
	index_cache.ResetCapacity(sprite_count*6);
	Vertex* p_vertex=vertex_cache.GetPtr();
	Index*  p_index =index_cache.GetPtr();
	int index=0;
	int index2=0;

	std::list<Sprite*>* list;
	while(spriteDictionary[index])
	{
		list=spriteDictionary[index]->val;
		std::list<Sprite*>::iterator it;
		it=list->begin();
		it!=list->end();
		for(it=list->begin();it!=list->end();it++)
		{
			(*it)->CopyVertices(p_vertex+index2*4);
			
			int i_p=index2*4;
			Index* p=index_cache.GetPtr()+index2*6;
			p[0]=0+i_p;
			p[1]=1+i_p;
			p[2]=2+i_p;
			p[3]=2+i_p;
			p[4]=1+i_p;
			p[5]=3+i_p;
			index2++;
		}
		index++;
	}
	
}

void SpriteCache::GetData(MemoryBlock* vertexData,MemoryBlock* indexData)
{
	vertexData->Ptr=vertex_cache.GetPtr();
	vertexData->Width=4*sizeof(Vertex)*sprite_count;
	indexData->Ptr=index_cache.GetPtr();
	indexData->Width=6*sizeof(Index)*sprite_count;
}

void SpriteCache::SetTextureAtlas(TextureAtlas* texAtlas)
{
	this->textureAtlas=texAtlas;

}

SpriteBatch SpriteCache::GetNextBatch()
{
	//#!
	Pair< std::list<Sprite*>* >* p=this->spriteDictionary[batch_index];
	if(p==NULL)
	{
		currentBatch=SpriteBatch(NULL,0,0);
		batch_index=0;
		return currentBatch;
	}
	currentBatch.p_texture=textureAtlas->GetTexture(p->tag);
	currentBatch.previous_sprites+=currentBatch.sprite_count;
	currentBatch.sprite_count=p->val->size();
	batch_index++;
	return currentBatch;

}

*/