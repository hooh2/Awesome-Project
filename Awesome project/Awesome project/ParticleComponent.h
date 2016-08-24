#pragma once
#include "Component.h"

#include <common\common.h>
#include <common\bgfx_utils.h>

//--- GLM Functions ---
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#include "Entity.h"
#include "Model.h"
#include "PosColorVertex.h"
#include <memory>

class ParticleComponent :
	public Component
{
public:

	bgfx::TextureHandle m_tex;

	ParticleComponent();
	~ParticleComponent();

	void init();
	void update() override;

private:

	bgfx::DynamicVertexBufferHandle m_vbh;
	bgfx::ProgramHandle m_program;
};

