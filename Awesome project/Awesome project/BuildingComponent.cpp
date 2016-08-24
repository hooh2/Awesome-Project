#include "BuildingComponent.h"

extern std::vector<bgfx::TextureHandle> textures;

BuildingComponent::BuildingComponent() {}
BuildingComponent::~BuildingComponent() {}

void BuildingComponent::init()
{
	auto scene = aiImportFile("models/grass_v21.OBJ", aiProcessPreset_TargetRealtime_MaxQuality);

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
					printf("Building Matirial %d Texture Not NULL; \n", i);

					auto filename = std::string("./textures/") + path.C_Str();
					bgfx::TextureHandle tex = loadTexture(filename.c_str());

					textures[texOffset + i] = tex;
				}
				else
				{
					printf("Building Matirial %d Texture NULL; \n", i);
				}
			}
		}

		collisionParams.resize(scene->mNumMeshes);

		for (size_t i = 0; i < scene->mNumMeshes; i++)
		{
			auto model = std::make_shared<Model>();
			model->mMesh = scene->mMeshes[i];

			model->m_tex = textures[texOffset + model->mMesh->mMaterialIndex];

			this->entity->addComponent(model);

			model->init();

			std::vector<glm::vec3> pos;
			std::vector<glm::vec3> nor;
			std::vector<unsigned int> ind;
			std::vector<std::vector<glm::vec3>> tris;

			model->getMeshVertexPos(pos);
			model->getMeshVertexNor(nor);
			model->getMeshIdices(ind);
			model->getMeshTris(tris);

			collisionParams[i].indices = ind;
			collisionParams[i].positions = pos;
			collisionParams[i].normals = nor;
			collisionParams[i].triangles = tris;
		}
	}

	pos = glm::vec3(0.f, 0.f, 0.f);
	mtx = glm::translate(mtx, pos);

	this->entity->updateMTX(mtx);
}

void BuildingComponent::update()
{
	
}

void BuildingComponent::faceComposer(std::vector<collisionStruct>& params)
{

}