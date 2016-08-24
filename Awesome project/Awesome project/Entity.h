#pragma once

#include "Component.h"
#include "CarComponent.h"
#include <glm\glm.hpp>

class Entity
{

public:

	Entity();
	~Entity();

	void addComponent(ComponentPtr component);

	void init();
	void update();

	glm::mat4 getMTX() { return mtx; };
	void updateMTX(glm::mat4 &newMTX) { mtx = newMTX; };

	glm::vec3 getPosition() { return pos; };
	void updatePosition(glm::vec3& newPos) { pos = newPos; };

	void setCarObject(CarComponent& in_obj) { car = in_obj; };
	void getCarObject(CarComponent& in_obj) { in_obj = car; };

private:

	std::vector<ComponentPtr> components;

	glm::mat4 mtx;
	glm::vec3 pos;
	CarComponent car;
};

using EntityPtr = std::shared_ptr<Entity>;