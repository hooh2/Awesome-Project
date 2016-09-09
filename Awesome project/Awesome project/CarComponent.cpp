#include "CarComponent.h"

glm::vec3 rightF = glm::vec3(2.1f, 0.f, 6.5f);
glm::vec3 leftF = glm::vec3(-2.1f, 0.f, 6.5f);
glm::vec3 rightB = glm::vec3(2.1f, 0.f, 0.f);
glm::vec3 leftB = glm::vec3(-2.1f, 0.f, 0.f);

CarComponent::CarComponent() {}
CarComponent::~CarComponent() {}

void CarComponent::init()
{
	glm::mat4 mtx;
	updateTransform(0, 0, 0);
	car_pos += speedZ * glm::vec3(rotationMTX[2][0], rotationMTX[2][1], rotationMTX[2][2]);
	mtx = glm::translate(mtx, car_pos);
	mtx = mtx * rotationMTX;
	car_mtx = mtx;
}

void CarComponent::update()
{
	glm::mat4 mtx;

	if (inputGetKeyState(entry::Key::KeyA))
	{
		rotY = -turn_speed;
		tireFrontRot = 100.f;
	}
	else if (inputGetKeyState(entry::Key::KeyD))
	{
		rotY = turn_speed;
		tireFrontRot = -100.f;
	}
	else
	{
		rotY = 0;
		tireFrontRot = 0.f;
	}

	if (inputGetKeyState(entry::Key::KeyW))
	{
		speedZ = speed;
	}
	else if (inputGetKeyState(entry::Key::KeyS))
	{
		speedZ = -speed;
	}
	else
	{
		speedZ = 0;
	}

	updateTransform(0, rotY, 0);
	car_pos += speedZ * glm::vec3(rotationMTX[2][0], rotationMTX[2][1], rotationMTX[2][2]);

	mtx = glm::translate(mtx, car_pos);
	mtx = mtx * rotationMTX;

	glm::vec4 rightFT = glm::vec4(rightF, 1.f);
	worldRirhtFT = mtx * rightFT;
	glm::vec4 leftFT = glm::vec4(leftF, 1.f);
	worldLeftFT = mtx * leftFT;
	glm::vec4 rightBT = glm::vec4(rightB, 1.f);
	worldRightBT = mtx * rightBT;
	glm::vec4 leftBT = glm::vec4(leftB, 1.f);
	worldLeftBT = mtx * leftBT;

	glm::vec3 middleBetwinFrontweels;
	midpoint(glm::vec3(worldRirhtFT.x, wRFT_y, worldRirhtFT.z), glm::vec3(worldLeftFT.x, wLFT_y, worldLeftFT.z), middleBetwinFrontweels);
	glm::vec3 middleBetwinRearWeels;
	midpoint(glm::vec3(worldRightBT.x, wRBT_y, worldRightBT.z), glm::vec3(worldLeftBT.x, wLBT_y, worldLeftBT.z), middleBetwinRearWeels);

	glm::vec3 vector_a = glm::vec3(middleBetwinFrontweels - middleBetwinRearWeels);
	glm::vec3 vector_b = glm::vec3(glm::vec3(middleBetwinFrontweels.x, middleBetwinRearWeels.y, middleBetwinFrontweels.z) - middleBetwinRearWeels);

	float car_rot = glm::acos(glm::dot(vector_a, vector_b) / (glm::length(vector_a) * glm::length(vector_b)));

	//if (car_rot > 0)
	//	mtx = glm::rotate(mtx, car_rot, glm::vec3(rotationMTX[2][0], rotationMTX[2][1], rotationMTX[2][2]));

	car_mtx = mtx;
	
	car_pos = middleBetwinRearWeels;

	//bgfx::dbgTextPrintf(2, 2, 0x0f, "car_rot_2: %f", car_rot);

}

void CarComponent::updateTransform(float xinc, float yinc, float zinc)
{
	glm::mat4 matrixX, matrixXY;

	//rotation about the local x axis
	glm::quat q = glm::angleAxis(xinc, glm::vec3(rotationMTX[0][0], rotationMTX[0][1], rotationMTX[0][2]));
	matrixX = glm::mat4_cast(q) * rotationMTX;

	//rotation about the local y axis
	q = glm::angleAxis(yinc, glm::vec3(matrixX[1][0], matrixX[1][1], matrixX[1][2]));
	matrixXY = glm::mat4_cast(q) * matrixX;

	//rotation about the local z axis
	q = glm::angleAxis(zinc, glm::vec3(matrixXY[2][0], matrixXY[2][1], matrixXY[2][2]));
	rotationMTX = glm::mat4_cast(q) * matrixXY;
}

void CarComponent::midpoint(glm::vec3 a, glm::vec3 b, glm::vec3& middle)
{
	middle.x = (a.x + b.x) / 2;
	middle.y = (a.y + b.y) / 2;
	middle.z = (a.z + b.z) / 2;
}