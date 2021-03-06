#include "CarTireFrontRight.h"

extern std::vector<bgfx::TextureHandle> textures;

CarTireFrontRight::CarTireFrontRight() {}
CarTireFrontRight::~CarTireFrontRight() {}

void CarTireFrontRight::init()
{
	auto scene = aiImportFile("models/F40tire_front_right.obj", aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->mNumMeshes != 0)
	{
		int texOffset = textures.size();
		textures.resize(texOffset + scene->mNumMaterials);

		if (scene->HasMaterials())
		{
			printf("Loading Car front right tire Textures ... ");
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
					printf("\nCar front right tire Texture %d not assigned; \n", i);
				}
			}
			printf("Done! \n");
		}

		for (size_t i = 0; i < scene->mNumMeshes; i++)
		{
			auto model = std::make_shared<Model>();
			model->mMesh = scene->mMeshes[i];

			model->m_tex = textures[texOffset + model->mMesh->mMaterialIndex];

			this->entity->addComponent(model);
		}
	}

	this->entity->getCarObject(car);

	glm::vec3 pos;
	car.getCarPos(pos);
	this->entity->updatePosition(pos);
}

void CarTireFrontRight::update()
{
	glm::mat4 mtx;
	glm::vec3 car_pos;

	this->entity->getCarObject(car);

	car.getCarMTX(mtx);
	car.getCarPos(car_pos);

	this->entity->updateMTX(mtx);
	this->entity->updatePosition(car_pos);
}
