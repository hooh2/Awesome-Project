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

class EnvironmentBenches :
	public Component
{
public:
	EnvironmentBenches();
	~EnvironmentBenches();

	void init() override;
	void update();

private:
	glm::vec3 pos;
	glm::mat4 mtx;
	CarComponent car;

	std::vector<glm::vec3> model_normals;
	std::vector<std::vector<glm::vec3>> model_triangles;
	std::vector<std::vector<glm::vec3>> modelBBs;
	std::vector<std::vector<unsigned int>> tiangles_in_BBs;

	void calcBoundingBox(std::vector<std::vector<glm::vec3>> triangles, std::vector<glm::vec3>& boundingBox);
	bool inBoundingBox(glm::vec3 point, std::vector<glm::vec3> boundingBox);
	bool collisionCheck(glm::vec3 collisionContourPoint);
	bool intersectCC_Triangle(glm::vec3 R1, glm::vec3 R2, unsigned int triangle);
	bool BarycentricCalculation(glm::vec3 point, float dist, unsigned int triangle);
	bool collisionBetweenPoint(glm::vec3 point, float threshold, unsigned int triangle);
};

