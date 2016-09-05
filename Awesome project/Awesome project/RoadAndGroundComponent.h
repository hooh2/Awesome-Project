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

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem> 

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
	CarComponent car;

	std::vector<glm::vec3> model_normals;
	std::vector<std::vector<glm::vec3>> model_triangles;
	std::vector<std::vector<glm::vec3>> quadsMinAndMax;
	std::vector<std::vector<unsigned int>> trianglesInQuads;

	void Y_cordinate(glm::vec3 pos_coord, float& point_y);
	bool BarycentricCalculation2Dvec(glm::vec2 point, std::vector<glm::vec2> triangle);
	void QuadSeparation();
	void trianglesInQuadsSeparation(std::string path);
	void wrightTrianglesInQuads(std::string path);
	void readTrianglesInQuads(std::string path);
	bool do_line_intersects(std::vector<glm::vec2> line1, std::vector<glm::vec2> line2);
	void calcBoundingBox(std::vector<std::vector<glm::vec3>> triangles, std::vector<glm::vec3>& boundingBox);

};