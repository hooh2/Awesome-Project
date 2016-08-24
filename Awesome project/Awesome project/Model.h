#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Entity.h"
#include "Component.h"
#include "PosColorVertex.h"
#include <bgfx\bgfx.h>
#include <common\entry\input.h>



class Model: public Component
{

public:

	aiMesh* mMesh;
	bgfx::TextureHandle m_tex;
	
	Model();
	~Model();

	void init();
	void update() override;
	void getMeshVertexPos(std::vector<glm::vec3>& incPositions);
	void getMeshVertexNor(std::vector<glm::vec3>& incNormals);
	void getMeshIdices(std::vector<unsigned int>& incIndices);
	void getMeshTris(std::vector<std::vector<glm::vec3>>& incTris);

private:

	glm::mat4 mtx;
	glm::vec3 pos;

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	bgfx::ProgramHandle m_program;

};

