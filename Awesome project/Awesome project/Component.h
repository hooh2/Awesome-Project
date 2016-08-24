#pragma once

#include <vector>
#include <memory>

//--- GLM Functions ---
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

class Entity;

class Component
{

public:

	Component();
	~Component();

	virtual void init() = 0;
	virtual void update() = 0;

	void setEntity(Entity* entity) { this->entity = entity; }

protected:

	Entity* entity;

};

using ComponentPtr = std::shared_ptr<Component>;