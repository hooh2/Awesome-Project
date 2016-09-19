#pragma once

#include <common\common.h>
#include <common\entry\input.h>
#include <common\bgfx_utils.h>

//--- GLM Functions ---
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtx\vector_angle.hpp>

#include <memory>
#include <functional>
#include <numeric>
#include <vector>
#include <math.h>

#define PI 3.14159265

class CarComponent
{
public:
	CarComponent();
	~CarComponent();

	void init();
	void update();
	void updateTransform(float xinc, float yinc, float zinc);
	void midpoint(glm::vec3 a, glm::vec3 b, glm::vec3 & middle);
	void getCarMTX(glm::mat4& in_mtx) { in_mtx = car_mtx; };
	void getCarPos(glm::vec3& in_pos) { in_pos = car_pos; };
	void getRotationMTX(glm::mat4& in_rotMTX) { in_rotMTX = rotationMTX; };
	void getTireFrontRot(float& in_frontRot) { in_frontRot = tireFrontRot; };

	void getFRTcoords(glm::vec3& in_coords) { in_coords = glm::vec3(worldRirhtFT); };
	void getFLTcoords(glm::vec3& in_coords) { in_coords = glm::vec3(worldLeftFT); };
	void getRRTcoords(glm::vec3& in_coords) { in_coords = glm::vec3(worldRightBT); };
	void getRLTcoords(glm::vec3& in_coords) { in_coords = glm::vec3(worldLeftBT); };
	void setTiersYcoords(float in_RFT, float in_LFT, float in_RBT, float in_LBT)
	{
		wRFTtrans = in_RFT; wLFTtrans = in_LFT; wRBTtrans = in_RBT; wLBTtrans = in_LBT;
	};

private:

	glm::mat4 rotationMTX;
	glm::mat4 car_mtx;
	glm::vec3 car_pos;

	glm::vec4 worldRirhtFT; float wRFTtrans;
	glm::vec4 worldLeftFT;  float wLFTtrans;
	glm::vec4 worldRightBT; float wRBTtrans;
	glm::vec4 worldLeftBT;  float wLBTtrans;

	float rotY = 0.f;
	float speedZ = 0.f;
	float speed = 1.f;
	float turn_speed = 3.f;
	float tireRotSpeed = 0.f;
	float tireFrontRot = 0.f;
};

