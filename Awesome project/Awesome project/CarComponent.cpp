#include "CarComponent.h"

glm::vec3 rightF = glm::vec3(2.1f, 0.8f, 6.5f);
glm::vec3 leftF = glm::vec3(-2.1f, 0.8f, 6.5f);
glm::vec3 rightB = glm::vec3(2.1f, 0.8f, 0.f);
glm::vec3 leftB = glm::vec3(-2.1f, 0.8f, 0.f);

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
	bool mooving;
	std::vector<bool> collisionRespond;

	for (unsigned int r = 0; r < carCollisionContour.size(); r++)
	{
		if (collisionRespondBuildings[r] || 
			collisionRespondCars[r] || 
			collisionRespondEnvBenches[r]/* || 
			collisionRespondEnvBusStops[r] || 
			collisionRespondEnvLightPoles[r] || 
			collisionRespondEnvMisc[r] || 
			collisionRespondEnvStreetLights[r]*/)
		{
			collisionRespond.push_back(true);
		}
		else
		{
			collisionRespond.push_back(false);
		}
	}

	if (inputGetKeyState(entry::Key::KeyW) && !collisionRespond[2] && !collisionRespond[3] && !collisionRespond[4])
	{
		speedZ = speed;
		mooving = true;
	}
	else if (inputGetKeyState(entry::Key::KeyS) && !collisionRespond[0] && !collisionRespond[1])
	{
		speedZ = -speed;
		mooving = true;
	}
	else
	{
		speedZ = 0;
		mooving = false;
	}

	if (inputGetKeyState(entry::Key::KeyA) && !collisionRespond[3] && mooving)
	{
		rotY = -turn_speed;
	}
	else if (inputGetKeyState(entry::Key::KeyD) && !collisionRespond[2] && mooving)
	{
		rotY = turn_speed;
	}
	else
	{
		rotY = 0;
	}

	float wRFTtransLocal;
	float wLFTtransLocal;
	float wRBTtransLocal;
	float wLBTtransLocal;

	presenceCheck(wRFTonTT, wRFTtransTT, wRFTtrans, wRFTtransLocal);
	presenceCheck(wLFTonTT, wLFTtransTT, wLFTtrans, wLFTtransLocal);
	presenceCheck(wRBTonTT, wRBTtransTT, wRBTtrans, wRBTtransLocal);
	presenceCheck(wLBTonTT, wLBTtransTT, wLBTtrans, wLBTtransLocal);

	float middleBetwinFrontweels;
	float middleBetwinRearWeels;

	if (wRFTtransLocal > wLFTtransLocal)
	{
		middleBetwinFrontweels = wRFTtransLocal;
		if (wRFTtransLocal > 1.2f)
		{
			rotZ = 1.2f;
		}
		else if (wLFTtransLocal < -1.2f)
		{
			rotZ = 1.2f;
		}
		else
			rotZ = 0;
	}
	else
	{
		middleBetwinFrontweels = wLFTtransLocal;
		if (wLFTtransLocal > 1.2f)
		{
			rotZ = -1.2f;
		}
		else if (wRFTtransLocal < -1.2f)
		{
			rotZ = -1.2f;
		}
		else
			rotZ = 0;
	}

	if (wRBTtransLocal > wLBTtransLocal)
		middleBetwinRearWeels = wRBTtransLocal;
	else
		middleBetwinRearWeels = wLBTtransLocal;

	if (middleBetwinFrontweels > 2.0f)
		rotX = middleBetwinFrontweels;
	else if (middleBetwinFrontweels < -2.0f)
		rotX = -3.f;
	else
		rotX = middleBetwinFrontweels;

	if (middleBetwinRearWeels > 2.0f)
		carfooting = 2.5f;
	else if (middleBetwinRearWeels < -2.0f)
		carfooting = -2.5f;
	else
		carfooting = middleBetwinRearWeels;

	updateTransform(-rotX, rotY, rotZ);
	car_pos += speedZ * glm::vec3(rotationMTX[2][0], rotationMTX[2][1], rotationMTX[2][2]);

	car_pos += glm::vec3(0.f, carfooting, 0.f);

	mtx = glm::translate(mtx, car_pos);
	mtx = mtx * rotationMTX;

	std::vector<glm::vec3> collisionContour;
	collisionContourUpdate(carBB, collisionContour, mtx);
	carCollisionContour = collisionContour;

	glm::vec4 rightFT = glm::vec4(rightF, 1.f);
	worldRirhtFT = mtx * rightFT;
	glm::vec4 leftFT = glm::vec4(leftF, 1.f);
	worldLeftFT = mtx * leftFT;
	glm::vec4 rightBT = glm::vec4(rightB, 1.f);
	worldRightBT = mtx * rightBT;
	glm::vec4 leftBT = glm::vec4(leftB, 1.f);
	worldLeftBT = mtx * leftBT;

	car_mtx = mtx;
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

void CarComponent::presenceCheck(bool pres, float ifPres, float notPres, float& value)
{
	if (pres)
	{
		value = ifPres;
	}
	else
	{
		value = notPres;
	}
}

void CarComponent::collisionContourUpdate(std::vector<glm::vec3> modelBB, std::vector<glm::vec3>& collisionContour, glm::mat4 mtx)
{
	glm::vec4 point1 = glm::vec4(modelBB[0].x, modelBB[0].y, modelBB[0].z, 1.0f);
	glm::vec4 point2 = glm::vec4(modelBB[1].x, modelBB[0].y, modelBB[0].z, 1.0f);
	glm::vec4 point3 = glm::vec4(modelBB[1].x, modelBB[0].y, modelBB[1].z, 1.0f);
	glm::vec4 point4 = glm::vec4(modelBB[0].x, modelBB[0].y, modelBB[1].z, 1.0f);

	glm::vec4 point34 = glm::vec4((point3.x + point4.x)/2, modelBB[0].y, (point3.z + point4.z) / 2, 1.0f);

	point1 = mtx * point1;
	point2 = mtx * point2;
	point3 = mtx * point3;
	point4 = mtx * point4;

	point34 = mtx * point34;

	glm::vec3 point1_v3 = glm::vec3(point1);
	glm::vec3 point2_v3 = glm::vec3(point2);
	glm::vec3 point3_v3 = glm::vec3(point3);
	glm::vec3 point4_v3 = glm::vec3(point4);

	glm::vec3 point34_v3 = glm::vec3(point34);

	collisionContour.push_back(point1_v3);
	collisionContour.push_back(point2_v3);
	collisionContour.push_back(point3_v3);
	collisionContour.push_back(point4_v3);
	collisionContour.push_back(point34_v3);
}