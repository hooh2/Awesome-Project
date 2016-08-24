#include "Model.h"

Model::Model()
{
	m_tex = BGFX_INVALID_HANDLE;
}


Model::~Model()
{
	bgfx::destroyIndexBuffer(m_ibh);
	bgfx::destroyVertexBuffer(m_vbh);
	bgfx::destroyProgram(m_program);
	if (bgfx::isValid(m_tex))
	{
		bgfx::destroyTexture(m_tex);
	}
}

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 nor;
};

void Model::init()
{

	std::vector<Vertex> vertices(mMesh->mNumVertices);

	for (size_t i = 0; i < mMesh->mNumVertices; i++)
	{
		auto vert = mMesh->mVertices[i];
		vertices[i].pos = glm::vec3(vert.x, vert.y, vert.z);

		auto norm = mMesh->mNormals[i];
		vertices[i].nor = glm::vec3(norm.x, norm.y, norm.z);

		if (mMesh->mTextureCoords[0])
		{
			auto tex = mMesh->mTextureCoords[0][i];
			vertices[i].tex = glm::vec2{ tex.x , tex.y };
		}
	}

	// Create static vertex buffer.
	m_vbh = bgfx::createVertexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::copy(&vertices[0], mMesh->mNumVertices * sizeof(Vertex)),
		PosColorVertex::ms_decl
		);

	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mMesh->mNumFaces; i++)
	{
		for (size_t j = 0; j < mMesh->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(mMesh->mFaces[i].mIndices[j]);
		}
	}

	// Create static index buffer.
	m_ibh = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::copy(indices.data(), indices.size() * sizeof(unsigned int)), BGFX_BUFFER_INDEX32);

	// Create program from shaders.
	m_program = loadProgram("vs_tree", "fs_tree");

}

void Model::update()
{
	mtx = this->entity->getMTX();

	// Set model matrix for rendering.
	bgfx::setTransform(&mtx[0][0]);

	// Set vertex and index buffer.
	bgfx::setVertexBuffer(m_vbh);
	bgfx::setIndexBuffer(m_ibh);

	// Set render states.
	bgfx::setState(0 
		| BGFX_STATE_RGB_WRITE
		| BGFX_STATE_ALPHA_WRITE
		| BGFX_STATE_DEPTH_TEST_LESS 
		| BGFX_STATE_DEPTH_WRITE 
		//| BGFX_STATE_CULL_CW 
		| BGFX_STATE_MSAA
		/*| BGFX_STATE_PT_POINTS*/);

	if (bgfx::isValid(m_tex))
	{
		auto texSampler = bgfx::createUniform("texDiffuse", bgfx::UniformType::Int1);
		bgfx::setTexture(0, texSampler, m_tex);
	}

	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_program);

}

void Model::getMeshVertexPos(std::vector<glm::vec3>& incPositions)
{
	std::vector<glm::vec3> pos(mMesh->mNumVertices);

	for (size_t i = 0; i < mMesh->mNumVertices; i++)
	{
		auto vert = mMesh->mVertices[i];
		pos[i] = glm::vec3(vert.x, vert.y, vert.z);
	}

	incPositions = pos;
}

void Model::getMeshVertexNor(std::vector<glm::vec3>& incNormals)
{
	std::vector<glm::vec3> nor(mMesh->mNumVertices);

	for (size_t i = 0; i < mMesh->mNumVertices; i++)
	{
		auto norm = mMesh->mNormals[i];
		nor[i] = glm::vec3(norm.x, norm.y, norm.z);
	}

	incNormals = nor;
}

void Model::getMeshIdices(std::vector<unsigned int>& incIndices)
{
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mMesh->mNumFaces; i++)
	{
		for (size_t j = 0; j < mMesh->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(mMesh->mFaces[i].mIndices[j]);
		}
	}

	incIndices = indices;
}

void Model::getMeshTris(std::vector<std::vector<glm::vec3>>& incTris)
{
	std::vector<std::vector<glm::vec3>> tris;

	for (size_t i = 0; i < mMesh->mNumFaces; i++)
	{
		std::vector<glm::vec3> triangl;

		for (size_t j = 0; j < mMesh->mFaces[i].mNumIndices; j++)
		{
			auto vert = mMesh->mVertices[mMesh->mFaces[i].mIndices[j]];
			glm::vec3 pos = glm::vec3(vert.x, vert.y, vert.z);
			triangl.push_back(pos);
		}

		tris.push_back(triangl);
	}

	incTris = tris;
}