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

	std::vector<glm::vec3> normals;
	std::vector<std::vector<glm::vec3>> triangles;

	void Y_cordinate();
	bool BarycentricCalculation(glm::vec3 & point, float dist, int i);
	void QuadSeparation(std::vector<std::vector<glm::vec3>> triangles);
	void calcBoundingBox(std::vector<std::vector<glm::vec3>> triangles, std::vector<glm::vec3>& boundingBox);

};