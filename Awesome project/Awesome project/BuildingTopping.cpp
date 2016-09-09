#include "BuildingTopping.h"

extern std::vector<bgfx::TextureHandle> textures;

BuildingTopping::BuildingTopping() {}
BuildingTopping::~BuildingTopping() {}

void BuildingTopping::init()
{
	auto scene = aiImportFile("models/Buildings_topping.OBJ", aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->mNumMeshes != 0)
	{
		int texOffset = textures.size();
		textures.resize(texOffset + scene->mNumMaterials);

		if (scene->HasMaterials())
		{
			printf("Loading Buildings Top Psrts Textures ... ");
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
					printf("\nBuildings Top Parts Texture %d not found; \n", i);
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

	pos = glm::vec3(0.f, 0.f, 0.f);
	mtx = glm::translate(mtx, pos);

	this->entity->updateMTX(mtx);
}

void BuildingTopping::update()
{

}