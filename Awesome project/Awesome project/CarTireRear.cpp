#include "CarTireRear.h"

extern std::vector<bgfx::TextureHandle> textures;

CarTireRear::CarTireRear() {}
CarTireRear::~CarTireRear() {}

void CarTireRear::init()
{
	auto scene = aiImportFile("models/OBJ/F40tire_rear.obj", aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->mNumMeshes != 0)
	{
		int texOffset = textures.size();
		textures.resize(texOffset + scene->mNumMaterials);

		if (scene->HasMaterials())
		{
			for (size_t i = 0; i < scene->mNumMaterials; i++)
			{
				aiString path;
				scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				if (path.operator!=(aiString("")))
				{
					printf("Car Matirial %d Texture Not NULL; \n", i);
				}
				else
				{
					printf("Car Matirial %d Texture NULL; \n", i);
				}
				auto filename = std::string("./textures/") + path.C_Str();
				bgfx::TextureHandle tex = loadTexture(filename.c_str());

				textures[texOffset + i] = tex;
			}
		}

		for (size_t i = 0; i < scene->mNumMeshes; i++)
		{
			auto model = std::make_shared<Model>();
			model->mMesh = scene->mMeshes[i];

			model->m_tex = textures[texOffset + model->mMesh->mMaterialIndex];

			this->entity->addComponent(model);

			model->init();
		}
	}

	this->entity->getCarObject(car);

	glm::vec3 pos;
	car.getCarPos(pos);
	this->entity->updatePosition(pos);
}

void CarTireRear::update()
{
	glm::mat4 mtx;
	glm::vec3 car_pos;

	this->entity->getCarObject(car);

	car.getCarMTX(mtx);
	car.getCarPos(car_pos);

	this->entity->updateMTX(mtx);
	this->entity->updatePosition(car_pos);
}
