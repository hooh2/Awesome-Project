#pragma once
#include "Component.h"

#include <common\common.h>
#include <common\bgfx_utils.h>

//--- GLM Functions ---
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#include "Entity.h"
#include "Model.h"
#include "PosColorVertex.h"
#include <memory>

class EnvironmentStreetLights :
	public Component
{
public:
	EnvironmentStreetLights();
	~EnvironmentStreetLights();

	void init() override;
	void update();

private:
	glm::vec3 pos;
	glm::mat4 mtx;
	CarComponent car;
};

