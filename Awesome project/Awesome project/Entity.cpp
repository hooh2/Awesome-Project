#include "Entity.h"



Entity::Entity()
{

}


Entity::~Entity()
{

}

void Entity::addComponent(ComponentPtr component)
{

	component->setEntity(this);
	components.push_back(component);

}

void Entity::init()
{

	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->init();
	}

}

void Entity::update()
{

	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->update();
	}

}