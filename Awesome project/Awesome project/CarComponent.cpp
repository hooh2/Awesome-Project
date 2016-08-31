#include "CarComponent.h"

glm::vec3 rightF = glm::vec3(2.1f, 0.f, 6.5f);
glm::vec3 leftF = glm::vec3(-2.1f, 0.f, 6.5f);
glm::vec3 rightB = glm::vec3(2.1f, 0.f, 0.f);
glm::vec3 leftB = glm::vec3(-2.1f, 0.f, 0.f);

CarComponent::CarComponent() {}
CarComponent::~CarComponent() {}

void CarComponent::init()
{


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
	car_mtx = mtx;

	glm::vec4 rightFT = glm::vec4(rightF, 1.f);
	glm::vec4 worldRirhtFT = mtx * rightFT;

	bgfx::dbgTextPrintf(2, 2, 0x0f, "worldRirhtFT: %f, %f, %f", worldRirhtFT.x, worldRirhtFT.y, worldRirhtFT.z);
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
