#pragma once

#include <common\common.h>
#include <common\entry\input.h>
#include <common\bgfx_utils.h>

//--- GLM Functions ---
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#include <memory>

class CarComponent
{
public:
	CarComponent();
	~CarComponent();

	void init();
	void update();
	void updateTransform(float xinc, float yinc, float zinc);
	void getCarMTX(glm::mat4& in_mtx) { in_mtx = car_mtx; };
	void getCarPos(glm::vec3& in_pos) { in_pos = car_pos; };
	void getRotationMTX(glm::mat4& in_rotMTX) { in_rotMTX = rotationMTX; };
	void getTireFrontRot(float in_frontRot) { in_frontRot = tireFrontRot; };

private:

	glm::mat4 rotationMTX;
	glm::mat4 car_mtx;
	glm::vec3 car_pos;
	float rotY = 0.f;
	float speedZ = 0.f;
	float speed = 1.f;
	float turn_speed = 3.f;
	float tireRotSpeed = 0.f;
	float tireFrontRot = 0.f;
};

