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

class TestTrack :
	public Component
{
public:
	TestTrack();
	~TestTrack();

	void init() override;
	void update();

private:
	glm::vec3 pos;
	glm::mat4 mtx;
	CarComponent car;

	std::vector<glm::vec3> model_normals;
	std::vector<std::vector<glm::vec3>> model_triangles;
	std::vector<glm::vec3> modelBB;

	void calcBoundingBox(std::vector<std::vector<glm::vec3>> triangles, std::vector<glm::vec3>& boundingBox);
};

