#include "ParticleComponent.h"



ParticleComponent::ParticleComponent()
{

}

ParticleComponent::~ParticleComponent()
{

}

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 tex;
};

void ParticleComponent::init()
{
	//std::vector<Vertex> vertices(mMesh->mNumVertices);

	//for (size_t i = 0; i < mMesh->mNumVertices; i++)
	//{
	//	auto vert = mMesh->mVertices[i];
	//	vertices[i].pos = glm::vec3(vert.x, vert.y, vert.z);

	//	if (mMesh->mTextureCoords[0])
	//	{
	//		auto tex = mMesh->mTextureCoords[0][i];
	//		vertices[i].tex = glm::vec2{ tex.x , tex.y };
	//	}
	//}

	//// Create static vertex buffer.
	//m_vbh = bgfx::createDynamicVertexBuffer(
	//	// Static data can be passed with bgfx::makeRef
	//	bgfx::copy(&vertices[0], mMesh->mNumVertices * sizeof(Vertex)),
	//	PosColorVertex::ms_decl
	//	);

	//std::vector<unsigned int> indices;

	//for (size_t i = 0; i < mMesh->mNumFaces; i++)
	//{
	//	for (size_t j = 0; j < mMesh->mFaces[i].mNumIndices; j++)
	//	{
	//		indices.push_back(mMesh->mFaces[i].mIndices[j]);
	//	}
	//}

	//// Create program from shaders.
	//m_program = loadProgram("vs_tree", "fs_tree");
}

void ParticleComponent::update()
{
	//std::vector<glm::vec3> positions(m_particles.size());


	//for (size_t i = 0; i < m_particles.size(); i++)
	//{
	//	m_particles[i].pos += m_particles[i].vel * 0.016f;
	//	positions[i] = m_particles[i].pos;
	//}

	//bgfx::updateDynamicVertexBuffer(m_vbh, 0, bgfx::copy(positions.data, positions.size() * sizeof(glm::vec3)));
}