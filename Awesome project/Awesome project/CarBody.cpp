#include "CarBody.h"

extern std::vector<bgfx::TextureHandle> textures;

CarBody::CarBody() {}
CarBody::~CarBody() {}

void CarBody::init()
{
	auto scene = aiImportFile("models/F40_body.obj", aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->mNumMeshes != 0)
	{
		int texOffset = textures.size();
		textures.resize(texOffset + scene->mNumMaterials);

		if (scene->HasMaterials())
		{
			printf("Loading Car body Textures ... ");
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
					printf("\nCar body Texture %d not assigned; \n", i);
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

			std::vector<glm::vec3> mesh_normals;
			std::vector<std::vector<glm::vec3>> mesh_tris;

			model->getMeshVertexNor(mesh_normals);
			model->getMeshTris(mesh_tris);

			for (unsigned int n = 0; n < mesh_tris.size(); n++)
			{
				model_normals.push_back(mesh_normals[n]);
				model_triangles.push_back(mesh_tris[n]);
			}
		}
	}

	this->entity->getCarObject(car);

	calcBoundingBox(model_triangles, modelBB);
	car.setCarBB(modelBB[0], modelBB[1]);

	glm::vec3 pos;
	car.getCarPos(pos);
	this->entity->updatePosition(pos);
	this->entity->setCarObject(car);
}

void CarBody::update()
{
	glm::mat4 mtx;
	glm::vec3 car_pos;

	this->entity->getCarObject(car);

	car.getCarMTX(mtx);
	car.getCarPos(car_pos);

	this->entity->updateMTX(mtx);
	this->entity->updatePosition(car_pos);
	this->entity->setCarObject(car);
}

void CarBody::calcBoundingBox(std::vector<std::vector<glm::vec3>> triangles, std::vector<glm::vec3>& boundingBox)
{
	float minX = triangles[0][0].x;
	float minY = triangles[0][0].y;
	float minZ = triangles[0][0].z;
	float maxX = triangles[0][0].x;
	float maxY = triangles[0][0].y;
	float maxZ = triangles[0][0].z;

	for (unsigned int i = 1; i < triangles.size(); i++)
	{
		float x_a, y_a, z_a, x_b, y_b, z_b, x_c, y_c, z_c;

		glm::vec3 a = triangles[i][0];
		glm::vec3 b = triangles[i][1];
		glm::vec3 c = triangles[i][2];

		x_a = a.x; y_a = a.y; z_a = a.z;
		x_b = b.x; y_b = b.y; z_b = b.z;
		x_c = c.x; y_c = c.y; z_c = c.z;

		if (x_a < minX)
			minX = x_a;
		if (x_a > maxX)
			maxX = x_a;
		if (y_a < minY)
			minY = y_a;
		if (y_a > maxY)
			maxY = y_a;
		if (z_a < minZ)
			minZ = z_a;
		if (z_a > maxZ)
			maxZ = z_a;

		if (x_b < minX)
			minX = x_b;
		if (x_b > maxX)
			maxX = x_b;
		if (y_b < minY)
			minY = y_b;
		if (y_b > maxY)
			maxY = y_b;
		if (z_b < minZ)
			minZ = z_b;
		if (z_b > maxZ)
			maxZ = z_b;

		if (x_c < minX)
			minX = x_c;
		if (x_c > maxX)
			maxX = x_c;
		if (y_c < minY)
			minY = y_c;
		if (y_c > maxY)
			maxY = y_c;
		if (z_c < minZ)
			minZ = z_c;
		if (z_c > maxZ)
			maxZ = z_c;
	}

	glm::vec3 minBB(minX, minY, minZ);
	boundingBox.push_back(minBB);
	glm::vec3 maxBB(maxX, maxY, maxZ);
	boundingBox.push_back(maxBB);
}