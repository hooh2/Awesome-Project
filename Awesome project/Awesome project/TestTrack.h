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
	std::vector<std::vector<glm::vec3>> model_quadsMinAndMax;
	std::vector<std::vector<unsigned int>> model_trianglesInQuads;

	void calcWeelTranslationY(glm::vec3 weel, float radius, unsigned int & triangle, float & weelTransY);
	float isPointInsideSphere(glm::vec3 point, glm::vec3 sphere, float radius);
	void projectionPoint(glm::vec3 point, unsigned int triangle, glm::vec3 & projPoint);
	void in_triangle(glm::vec3 pos_coord, unsigned int & triangle);
	bool BarycentricCalculation2Dvec(glm::vec2 point, std::vector<glm::vec2> triangle);
	bool do_line_intersects(std::vector<glm::vec2> line1, std::vector<glm::vec2> line2);
	void QuadSeparation(std::vector<std::vector<glm::vec3>> triangles, std::vector<std::vector<glm::vec3>>& quadsMinAndMax, std::vector<std::vector<unsigned int>>& trisInQuads);
	void trianglesInQuadsSeparation(std::vector<std::vector<glm::vec3>> triangles, std::vector<std::vector<glm::vec3>>& quadsMinAndMax, std::vector<std::vector<unsigned int>>& trisInQuads);
	void wrightTriangles(std::string path, std::vector<std::vector<unsigned int>>& vectorToWright);
	void readTriangles(std::string path, std::vector<std::vector<unsigned int>>& vectorToRead);
	void calcBoundingBox(std::vector<std::vector<glm::vec3>> triangles, std::vector<glm::vec3>& boundingBox);
	bool inBoundingBox(glm::vec3 point, std::vector<glm::vec3> boundingBox);
};

