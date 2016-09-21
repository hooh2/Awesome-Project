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
			printf("Loading GroundAndRoad Textures ... ");
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
					printf("\nGroundAndRoad Texture %d not assigned; \n", i);
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

	QuadSeparation(model_triangles, model_quadsMinAndMax, model_trianglesInQuads);

	this->entity->getCarObject(car);

	pos = glm::vec3(0.f, 0.f, 0.f);
	mtx = glm::translate(mtx, pos);

	this->entity->updateMTX(mtx);
}

void RoadAndGroundComponent::update()
{
	this->entity->getCarObject(car);

	glm::vec3 car_front_right_tier; float CFRTtrans; static unsigned int front_right_on;
	glm::vec3 car_front_left_tier;  float CFLTtrans; static unsigned int front_left_on;
	glm::vec3 car_rear_right_tier;  float CRRTtrans; static unsigned int rear_right_on;
	glm::vec3 car_rear_left_tier;   float CRLTtrans; static unsigned int rear_left_on;
	
	car.getFRTcoords(car_front_right_tier);
	car.getFLTcoords(car_front_left_tier);
	car.getRRTcoords(car_rear_right_tier);
	car.getRLTcoords(car_rear_left_tier);

	calcWeelTranslationY(car_front_right_tier, 1.2f, front_right_on, CFRTtrans);
	calcWeelTranslationY(car_front_left_tier, 1.2f, front_left_on, CFLTtrans);
	calcWeelTranslationY(car_rear_right_tier, 1.2f, rear_right_on, CRRTtrans);
	calcWeelTranslationY(car_rear_left_tier, 1.2f, rear_left_on, CRLTtrans);

	car.setTiersYcoords(CFRTtrans, CFLTtrans, CRRTtrans, CRLTtrans);

	this->entity->setCarObject(car);
}

void RoadAndGroundComponent::calcWeelTranslationY(glm::vec3 weel, float radius, unsigned int& triangle, float& weelTransY)
{
	glm::vec3 weelPproj;
	in_triangle(weel, triangle);
	projectionPoint(weel, triangle, weelPproj);
	weelTransY = isPointInsideSphere(weelPproj, weel, radius);
}

float RoadAndGroundComponent::isPointInsideSphere(glm::vec3 point, glm::vec3 sphere, float radius) 
{
	float distance = sqrt((point.x - sphere.x) * (point.x - sphere.x) +
		(point.y - sphere.y) * (point.y - sphere.y) +
		(point.z - sphere.z) * (point.z - sphere.z));
	return radius - distance;
}

void RoadAndGroundComponent::projectionPoint(glm::vec3 point, unsigned int triangle, glm::vec3& projPoint)
{
	glm::vec3 vPoint = point - model_triangles[triangle][0];
	float dist = glm::dot(vPoint, model_normals[triangle]);
	projPoint = point - dist*model_normals[triangle];
}

void RoadAndGroundComponent::in_triangle(glm::vec3 pos_coord, unsigned int& triangle)
{
	unsigned short int quad = 0;
	for (unsigned int i = 0; i < model_quadsMinAndMax.size(); i++)
	{
		if (pos_coord.x < model_quadsMinAndMax[i][1].x)
		{
			if (pos_coord.x > model_quadsMinAndMax[i][0].x)
			{
				if (pos_coord.z < model_quadsMinAndMax[i][1].z)
				{
					if (pos_coord.z > model_quadsMinAndMax[i][0].z)
					{
						quad = i;
					}
				}
			}
		}
	}

	glm::vec2 pos_point = glm::vec2(pos_coord.x, pos_coord.z);

	for (unsigned int i = 0; i < model_trianglesInQuads[quad].size(); i++)
	{
		std::vector<glm::vec2> triangle2D;
		for (unsigned int j = 0; j < 3; j++)
		{
			glm::vec2 triangle_point_2D = glm::vec2(model_triangles[model_trianglesInQuads[quad][i]][j].x, model_triangles[model_trianglesInQuads[quad][i]][j].z);
			triangle2D.push_back(triangle_point_2D);
		}

		if (BarycentricCalculation2Dvec(pos_point, triangle2D))
		{
			triangle = model_trianglesInQuads[quad][i];
		}
	}
}

bool RoadAndGroundComponent::BarycentricCalculation2Dvec(glm::vec2 point, std::vector<glm::vec2> triangle)
{
	// Compute vectors        
	glm::vec2 v0 = triangle[2] - triangle[0];
	glm::vec2 v1 = triangle[1] - triangle[0];
	glm::vec2 v2 = point - triangle[0];

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
	return (u >= 0) && (v >= 0) && (u + v < 1);
}

bool RoadAndGroundComponent::do_line_intersects(std::vector<glm::vec2> line1, std::vector<glm::vec2> line2)
{
	glm::vec2 v1 = line1[1] - line1[0];
	glm::vec2 v2 = line2[1] - line2[0];

	float s, t;
	s = (-v1.y * (line1[0].x - line2[0].x) + v1.x * (line1[0].y - line2[0].y)) / (-v2.x * v1.y + v1.x * v2.y);
	t = (v2.x * (line1[0].y - line2[0].y) - v2.y * (line1[0].x - line2[0].x)) / (-v2.x * v1.y + v1.x * v2.y);

	return s >= 0 && s <= 1 && t >= 0 && t <= 1; // Check if lines intersects
}

void RoadAndGroundComponent::QuadSeparation(std::vector<std::vector<glm::vec3>> triangles, 
											std::vector<std::vector<glm::vec3>>& quadsMinAndMax, 
											std::vector<std::vector<unsigned int>>& trisInQuads)
{
	std::vector<glm::vec3> MinAndMax;
	calcBoundingBox(triangles, MinAndMax);

	glm::vec3 minOfTris = MinAndMax[0];
	glm::vec3 maxOfTris = MinAndMax[1];

	float x_length = maxOfTris.x - minOfTris.x;
	float z_length = maxOfTris.z - minOfTris.z;

	unsigned int quads_size = 16;

	float quadSeze_x = x_length / quads_size;
	float quadSeze_z = z_length / quads_size;

	for (unsigned int w = 0; w < quads_size; w++)
	{
		for (unsigned int h = 0; h < quads_size; h++)
		{
			std::vector<glm::vec3> quadMinAndMax;
			glm::vec3 minQuadCoord = glm::vec3(minOfTris.x + ((quads_size - 1 - w)*quadSeze_x), minOfTris.y, minOfTris.z + ((quads_size - 1 - h)*quadSeze_z));
			quadMinAndMax.push_back(minQuadCoord);
			glm::vec3 maxQuadCoord = glm::vec3(maxOfTris.x - (w*quadSeze_x), maxOfTris.y, maxOfTris.z - (h*quadSeze_z));
			quadMinAndMax.push_back(maxQuadCoord);
			quadsMinAndMax.push_back(quadMinAndMax);
		}
	}

	std::string path = "models/TrianglesOfRoadAndGroundInQuads.txt";

	if (std::experimental::filesystem::exists(path))
	{
		std::cout << "File " << path << " exists \n";
		readTriangles(path, trisInQuads);
	}
	else
	{
		std::cout << "File " << path << " does not exists \n";
		trianglesInQuadsSeparation(triangles, quadsMinAndMax, trisInQuads);
		wrightTriangles(path, trisInQuads);
	}
}

void RoadAndGroundComponent::trianglesInQuadsSeparation(std::vector<std::vector<glm::vec3>> triangles,
														std::vector<std::vector<glm::vec3>>& quadsMinAndMax,
														std::vector<std::vector<unsigned int>>& trisInQuads)
{
	printf("Creating trianglesInQuads... ");

	for (unsigned int i = 0; i < quadsMinAndMax.size(); i++)
	{
		std::vector<unsigned int> trianglesInThisQuad;

		for (unsigned int j = 0; j < triangles.size(); j++)
		{
			for (unsigned int c = 0; c < 3; c++)
			{
				if (triangles[j][c].x < quadsMinAndMax[i][1].x)
				{
					if (triangles[j][c].x > quadsMinAndMax[i][0].x)
					{
						if (triangles[j][c].z < quadsMinAndMax[i][1].z)
						{
							if (triangles[j][c].z > quadsMinAndMax[i][0].z)
							{
								trianglesInThisQuad.push_back(j);
								break;
							}
						}
					}
				}
			}

			if (trianglesInThisQuad.size() != 0 && j == trianglesInThisQuad.back())
			{
				continue;
			}

			glm::vec2 point_a = glm::vec2(quadsMinAndMax[i][0].x, quadsMinAndMax[i][1].z);
			glm::vec2 point_b = glm::vec2(quadsMinAndMax[i][1].x, quadsMinAndMax[i][1].z);
			glm::vec2 point_c = glm::vec2(quadsMinAndMax[i][0].x, quadsMinAndMax[i][0].z);
			glm::vec2 point_d = glm::vec2(quadsMinAndMax[i][1].x, quadsMinAndMax[i][0].z);

			std::vector<glm::vec2> edgeAB = { point_a, point_b };
			std::vector<glm::vec2> edgeBC = { point_b, point_c };
			std::vector<glm::vec2> edgeCD = { point_c, point_d };
			std::vector<glm::vec2> edgeDA = { point_d, point_a };

			glm::vec2 triangle_point_a = glm::vec2(triangles[j][0].x, triangles[j][0].z);
			glm::vec2 triangle_point_b = glm::vec2(triangles[j][1].x, triangles[j][1].z);
			glm::vec2 triangle_point_c = glm::vec2(triangles[j][2].x, triangles[j][2].z);

			std::vector<glm::vec2> triangle_edgeAB = { triangle_point_a, triangle_point_b };
			std::vector<glm::vec2> triangle_edgeBC = { triangle_point_b, triangle_point_c };
			std::vector<glm::vec2> triangle_edgeCA = { triangle_point_c, triangle_point_a };

			if (do_line_intersects(edgeAB, triangle_edgeAB))
			{
				trianglesInThisQuad.push_back(j);
			}
			else if (do_line_intersects(edgeAB, triangle_edgeBC))
			{
				trianglesInThisQuad.push_back(j);
			}
			else if (do_line_intersects(edgeAB, triangle_edgeCA))
			{
				trianglesInThisQuad.push_back(j);
			}
			else
			{
				if (do_line_intersects(edgeBC, triangle_edgeAB))
				{
					trianglesInThisQuad.push_back(j);
				}
				else if (do_line_intersects(edgeBC, triangle_edgeBC))
				{
					trianglesInThisQuad.push_back(j);
				}
				else if (do_line_intersects(edgeBC, triangle_edgeCA))
				{
					trianglesInThisQuad.push_back(j);
				}
				else
				{
					if (do_line_intersects(edgeCD, triangle_edgeAB))
					{
						trianglesInThisQuad.push_back(j);
					}
					else if (do_line_intersects(edgeCD, triangle_edgeBC))
					{
						trianglesInThisQuad.push_back(j);
					}
					else if (do_line_intersects(edgeCD, triangle_edgeCA))
					{
						trianglesInThisQuad.push_back(j);
					}
					else
					{
						if (do_line_intersects(edgeDA, triangle_edgeAB))
						{
							trianglesInThisQuad.push_back(j);
						}
						else if (do_line_intersects(edgeDA, triangle_edgeBC))
						{
							trianglesInThisQuad.push_back(j);
						}
						else if (do_line_intersects(edgeDA, triangle_edgeCA))
						{
							trianglesInThisQuad.push_back(j);
						}
					}
				}
			}
		}

		std::sort(trianglesInThisQuad.begin(), trianglesInThisQuad.end());

		trisInQuads.push_back(trianglesInThisQuad);
	}
	printf("Done! \n");
	
}

void RoadAndGroundComponent::wrightTriangles(std::string path, std::vector<std::vector<unsigned int>>& vectorToWright)
{
	std::ofstream fout;
	fout.open(path);
	if (fout.is_open())
	{
		std::cout << "Writing information in to the file:\n" << path << " ... ";
		fout << vectorToWright.size() << std::endl;
		for (unsigned int i = 0; i < vectorToWright.size(); i++)
		{
			fout << vectorToWright[i].size() << std::endl;
			for (unsigned int j = 0; j < vectorToWright[i].size(); j++)
			{
				fout << vectorToWright[i][j] << " ";
			}
			fout << std::endl;
		}
		fout.close();
		printf("Done! \n");
	}
	else
		std::cout << "Unable to open file " << path << std::endl;
}

void RoadAndGroundComponent::readTriangles(std::string path, std::vector<std::vector<unsigned int>>& vectorToRead)
{
	std::ifstream fin;
	fin.open(path);
	if (fin.is_open())
	{

		std::cout << "Reading information from the file:\n" << path << " ... ";
		unsigned int numOfQuads;
		fin >> numOfQuads;
		while (!fin.eof())
		{

			for (unsigned int i = 0; i < numOfQuads; i++)
			{
				unsigned int numOfIndices;
				fin >> numOfIndices >> std::ws;
				std::vector<unsigned int> IndicesInOneVector;
				for (unsigned int j = 0; j < numOfIndices; j++)
				{
					unsigned int triIndex;
					fin >> triIndex >> std::ws;
					IndicesInOneVector.push_back(triIndex);
				}
				vectorToRead.push_back(IndicesInOneVector);
			}
		}
		fin.close();
		printf("Done! \n");
	}
	else
		std::cout << "Unable to open file " << path << std::endl;
}

void RoadAndGroundComponent::calcBoundingBox(std::vector<std::vector<glm::vec3>> triangles, std::vector<glm::vec3>& boundingBox)
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