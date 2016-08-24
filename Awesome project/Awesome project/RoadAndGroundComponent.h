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

class RoadAndGroundComponent :
	public Component
{
public:
	RoadAndGroundComponent();
	~RoadAndGroundComponent();

	void init() override;
	void update();

private:
	glm::vec3 pos;
	glm::mat4 mtx;

	struct collisionStruct
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
		std::vector<std::vector<glm::vec3>> triangles;
	};

	std::vector<collisionStruct> collisionParams;
};

