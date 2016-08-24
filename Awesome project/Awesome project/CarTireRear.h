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
#include "CarComponent.h"
#include "PosColorVertex.h"
#include <memory>

class CarTireRear :
	public Component
{
public:
	CarTireRear();
	~CarTireRear();

	void init() override;
	void update();

	void setCarObject(CarComponent in_car) { car = in_car; };

private:

	CarComponent car;
};