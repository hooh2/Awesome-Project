#include "CarTireFrontLeft.h"

extern std::vector<bgfx::TextureHandle> textures;

CarTireFrontLeft::CarTireFrontLeft() {}
CarTireFrontLeft::~CarTireFrontLeft() {}

void CarTireFrontLeft::init()
{
	auto scene = aiImportFile("models/F40tire_front_left.obj", aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->mNumMeshes != 0)
	{
		int texOffset = textures.size();
		textures.resize(texOffset + scene->mNumMaterials);

		if (scene->HasMaterials())
		{
			printf("Loading Car front left tire Textures ... ");
			for (size_t i = 0; i < scene->mNumMaterials; i++)
			{
				aiString path;
				scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				if (path.operator!=(aiString("")))
				{
					auto filename = std::string("./textures/") + path.C_Str();
					bgfx::TextureHandle tex = loadTexture(filename.c_str());

					textures[texOffset + i] = tex;
				}
				else
				{
					printf("\nCar front left tire Texture %d not assigned; \n", i);
				}
			}
			printf("Done! \n");
		}

		//collisionParams.resize(scene->mNumMeshes);

		for (size_t i = 0; i < scene->mNumMeshes; i++)
		{
			auto model = std::make_shared<Model>();
			model->mMesh = scene->mMeshes[i];

			model->m_tex = textures[texOffset + model->mMesh->mMaterialIndex];

			this->entity->addComponent(model);

			//std::vector<glm::vec3> pos;
			//std::vector<glm::vec3> nor;
			//std::vector<unsigned int> ind;
			//std::vector<std::vector<glm::vec3>> tris;

			//model->getMeshVertexPos(pos);
			//model->getMeshVertexNor(nor);
			//model->getMeshIdices(ind);
			//model->getMeshTris(tris);

			//collisionParams[i].indices = ind;
			//collisionParams[i].positions = pos;
			//collisionParams[i].normals = nor;
			//collisionParams[i].triangles = tris;
		}
	}

	this->entity->getCarObject(car);

	glm::vec3 pos;
	car.getCarPos(pos);
	this->entity->updatePosition(pos);
}

void CarTireFrontLeft::update()
{
	glm::mat4 mtx;
	glm::vec3 car_pos;
	float turnRot = 30.f;

	this->entity->getCarObject(car);

	car.getCarMTX(mtx);
	car.getCarPos(car_pos);
	car.getTireFrontRot(turnRot);

	//mtx = glm::rotate(mtx, turnRot, glm::vec3(0.f, 1.f, 0.f));

	this->entity->updateMTX(mtx);
	this->entity->updatePosition(car_pos);
}
