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

class CarTireFrontLeft :
	public Component
{
public:
	CarTireFrontLeft();
	~CarTireFrontLeft();

	void init() override;
	void update();

	void setCarObject(CarComponent in_car) { car = in_car; };

private:

	CarComponent car;

	//struct collisionStruct
	//{
	//	std::vector<glm::vec3> positions;
	//	std::vector<glm::vec3> normals;
	//	std::vector<unsigned int> indices;
	//	std::vector<std::vector<glm::vec3>> triangles;
	//};

	//std::vector<collisionStruct> collisionParams;
};