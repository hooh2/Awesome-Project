#include "BuildingBottoms.h"

extern std::vector<bgfx::TextureHandle> textures;

BuildingBottoms::BuildingBottoms() {}
BuildingBottoms::~BuildingBottoms() {}

void BuildingBottoms::init()
{
	auto scene = aiImportFile("models/Buildings_bottoms.OBJ", aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->mNumMeshes != 0)
	{
		int texOffset = textures.size();
		textures.resize(texOffset + scene->mNumMaterials);

		if (scene->HasMaterials())
		{
			printf("Loading Buildings Bottom Parts Textures ... ");
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
					printf("\nBuildings Bottom Parts Texture %d not found; \n", i);
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

			std::vector<glm::vec3> mesh_normals;
			std::vector<std::vector<glm::vec3>> mesh_tris;
			std::vector<glm::vec3> mesh_BB;

			model->getMeshVertexNor(mesh_normals);
			model->getMeshTris(mesh_tris);

			calcBoundingBox(mesh_tris, mesh_BB);

			unsigned int tirsOFF = model_triangles.size();
			std::vector<unsigned int> tiangles_in_this_BB;
			for (unsigned int n = 0; n < mesh_tris.size(); n++)
			{
				model_normals.push_back(mesh_normals[n]);
				model_triangles.push_back(mesh_tris[n]);
				tiangles_in_this_BB.push_back(tirsOFF + n);
			}
			tiangles_in_BBs.push_back(tiangles_in_this_BB);
			modelBBs.push_back(mesh_BB);
		}
	}

	pos = glm::vec3(0.f, 0.f, 0.f);
	mtx = glm::translate(mtx, pos);

	this->entity->updateMTX(mtx);
}

void BuildingBottoms::update()
{
	this->entity->getCarObject(car);

	std::vector<glm::vec3> carCollisionContour;
	car.getCarCC(carCollisionContour);

	std::vector<bool> collisionRespond;

	for (unsigned int c = 0; c < carCollisionContour.size(); c++)
	{
		collisionRespond.push_back(collisionCheck(carCollisionContour[c]));
	}

	car.setCarColisionRespondBuildings(collisionRespond);

	this->entity->setCarObject(car);
}

void BuildingBottoms::calcBoundingBox(std::vector<std::vector<glm::vec3>> triangles, std::vector<glm::vec3>& boundingBox)
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

bool BuildingBottoms::inBoundingBox(glm::vec3 point, std::vector<glm::vec3> boundingBox)
{
	if (point.x > boundingBox[0].x && point.x < boundingBox[1].x)
	{
		if (point.z > boundingBox[0].z && point.z < boundingBox[1].z)
		{
			return true;
		}
	}

	return false;
}

bool BuildingBottoms::collisionCheck(glm::vec3 collisionContourPoint)
{
	for (unsigned int b = 0; b < modelBBs.size(); b++)
	{
		if (inBoundingBox(collisionContourPoint, modelBBs[b]))
		{
			for (unsigned int t = 0; t < tiangles_in_BBs[b].size(); t++)
			{
				if (collisionBetweenPoint(collisionContourPoint, 1.f, tiangles_in_BBs[b][t]))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool BuildingBottoms::collisionBetweenPoint(glm::vec3 point, float threshold, unsigned int triangle)
{
	float dist = glm::dot((point - model_triangles[triangle][0]), model_normals[triangle]);

	if (BarycentricCalculation(point, dist, triangle))
	{
		if (abs(dist) < threshold)
		{
			return true;
		}
	}
	return false;
}
bool BuildingBottoms::BarycentricCalculation(glm::vec3 point, float dist, unsigned int triangle)
{
	glm::vec3 P = point - model_normals[triangle] * dist;

	// Compute vectors        
	glm::vec3 v0 = model_triangles[triangle][1] - model_triangles[triangle][0];
	glm::vec3 v1 = model_triangles[triangle][2] - model_triangles[triangle][0];
	glm::vec3 v2 = point - model_triangles[triangle][0];

	// Compute dot products
	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	if ((u >= 0) && (v >= 0) && (u + v < 1))
		return true;
	else
		return false;
}
