#include "RoadAndGroundComponent.h"

extern std::vector<bgfx::TextureHandle> textures;

RoadAndGroundComponent::RoadAndGroundComponent() {}
RoadAndGroundComponent::~RoadAndGroundComponent() {}

void RoadAndGroundComponent::init()
{
	auto scene = aiImportFile("models/Ground_FV.OBJ", aiProcessPreset_TargetRealtime_MaxQuality);

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
					printf("Ground Matirial %d Texture Not NULL; \n", i);

					auto filename = std::string("./textures/") + path.C_Str();
					bgfx::TextureHandle tex = loadTexture(filename.c_str());

					textures[texOffset + i] = tex;
				}
				else
				{
					printf("Ground Matirial %d Texture NULL; \n", i);
				}
			}
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
				normals.push_back(mesh_normals[n]);
				triangles.push_back(mesh_tris[n]);
			}
		}
	}

	QuadSeparation(triangles);

	pos = glm::vec3(0.f, 0.f, 0.f);
	mtx = glm::translate(mtx, pos);

	this->entity->updateMTX(mtx);
}

void RoadAndGroundComponent::update()
{

}



void RoadAndGroundComponent::Y_cordinate()
{

}

bool RoadAndGroundComponent::BarycentricCalculation(glm::vec3& point, float dist, int i)
{
	//glm::vec3 P = point - norms[i] * dist;

	//// Compute vectors        
	//glm::vec3 v0 = verts[triangles[i].z] - verts[triangles[i].x];
	//glm::vec3 v1 = verts[triangles[i].y] - verts[triangles[i].x];
	//glm::vec3 v2 = P - verts[triangles[i].x];

	//// Compute dot products
	//float dot00 = glm::dot(v0, v0);
	//float dot01 = glm::dot(v0, v1);
	//float dot02 = glm::dot(v0, v2);
	//float dot11 = glm::dot(v1, v1);
	//float dot12 = glm::dot(v1, v2);

	//// Compute barycentric coordinates
	//float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	//float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	//float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	//// Check if point is in triangle
	//if ((u >= 0) && (v >= 0) && (u + v < 1))
	//	return true;
	//else
		return false;
}

void RoadAndGroundComponent::QuadSeparation(std::vector<std::vector<glm::vec3>> triangles)
{
	std::vector<glm::vec3> MinAndMax;
	calcBoundingBox(triangles, MinAndMax);

	glm::vec3 minOfTris = MinAndMax[0];
	glm::vec3 maxOfTris = MinAndMax[1];

	float x_length = maxOfTris.x - minOfTris.x;
	float z_length = maxOfTris.z - minOfTris.z;

	float quadSeze_x = x_length / 8;
	float quadSeze_z = z_length / 8;

	std::vector<std::vector<glm::vec3>> quadsMinAndMax;

	for (unsigned int w = 0; w < 8; w++)
	{
		for (unsigned int h = 0; h < 8; h++)
		{
			std::vector<glm::vec3> quadMinAndMax;
			glm::vec3 minQuadCoord = glm::vec3(minOfTris.x + ((7 - w)*quadSeze_x), minOfTris.y, minOfTris.z + ((7 - h)*quadSeze_z));
			quadMinAndMax.push_back(minQuadCoord);
			glm::vec3 maxQuadCoord = glm::vec3(maxOfTris.x - (w*quadSeze_x), maxOfTris.y, maxOfTris.z - (h*quadSeze_z));
			quadMinAndMax.push_back(maxQuadCoord);
			quadsMinAndMax.push_back(quadMinAndMax);
		}
	}

}

void RoadAndGroundComponent::calcBoundingBox(std::vector<std::vector<glm::vec3>> triangles, std::vector<glm::vec3>& boundingBox)
{
	float minX = triangles[0][0].x;
	float minY = triangles[0][0].y;
	float minZ = triangles[0][0].z;
	float maxX = triangles[0][0].x;
	float maxY = triangles[0][0].y;
	float maxZ = triangles[0][0].z;

	for (int i = 1; i < triangles.size(); i++)
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