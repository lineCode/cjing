#include "entities.h"

Entities::Entities(Game&game, Map&map):
	mGame(game),
	mMap(map),
	mEntityNamed(),
	mAllEntities(),
	mEntityToDraw(),
	mEntityToRemove(),
	mCamera(nullptr)
{
	// ��ʼ��ÿ���layer

	// ��ʼ���Ĳ���


}

void Entities::Update()
{
}

void Entities::Draw()
{
}

void Entities::InitEntities()
{
}

void Entities::AddEntity(const EntityPtr& entity)
{
}

void Entities::RemoveEntity(Entity& entity)
{
}

