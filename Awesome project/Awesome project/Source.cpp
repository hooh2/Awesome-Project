#include <common\common.h>
#include <common\entry\input.h>
#include <common\bgfx_utils.h>
#include <bx\uint32_t.h>

//--- GLM Functions ---
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#include "Entity.h"
#include "Model.h"
#include "CarComponent.h"
#include "CarBody.h"
#include "CarTireRear.h"
#include "CarTireFrontLeft.h"
#include "CarTireFrontRight.h"
#include "BuildingComponent.h"
#include "RoadAndGroundComponent.h"
#include "PosColorVertex.h"
#include <memory>

glm::mat4 mtx;

CarComponent car;
//bgfx::ProgramHandle program;

std::vector<EntityPtr> entities;
std::vector<bgfx::TextureHandle> textures;

bgfx::VertexDecl PosColorVertex::ms_decl;

class MyGame : public entry::AppI
{
	void init(int _argc, char** _argv) BX_OVERRIDE
	{
		Args args(_argc, _argv);

		m_width = 1280;
		m_height = 720;
		m_debug = BGFX_DEBUG_TEXT;// | BGFX_DEBUG_WIREFRAME;
		m_reset = BGFX_RESET_VSYNC;

		bgfx::init(bgfx::RendererType::Enum::OpenGL);
		//bgfx::init(bgfx::RendererType::Enum::Direct3D11);
		bgfx::reset(m_width, m_height, m_reset);

		// Enable debug text.
		bgfx::setDebug(m_debug);

		// Set view 0 clear state.
		bgfx::setViewClear(0, 
			BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
			0x303030ff,
			1.0f,
			0
			);

		// Create vertex stream declaration.
		PosColorVertex::init();

		//program = loadProgram("vs_tree", "fs_tree");

		car.init();

		auto carBody = std::make_shared<Entity>();
		entities.push_back(carBody);
		carBody->addComponent(std::make_shared<CarBody>());

		auto carTireRear = std::make_shared<Entity>();
		entities.push_back(carTireRear);
		carTireRear->addComponent(std::make_shared<CarTireRear>());

		auto carTireFrontLeft = std::make_shared<Entity>();
		entities.push_back(carTireFrontLeft);
		carTireFrontLeft->addComponent(std::make_shared<CarTireFrontLeft>());

		auto carTireFrontRight = std::make_shared<Entity>();
		entities.push_back(carTireFrontRight);
		carTireFrontRight->addComponent(std::make_shared<CarTireFrontRight>());

		auto buildings = std::make_shared<Entity>();
		entities.push_back(buildings);
		buildings->addComponent(std::make_shared<BuildingComponent>());

		auto roadAndGround = std::make_shared<Entity>();
		entities.push_back(roadAndGround);
		roadAndGround->addComponent(std::make_shared<RoadAndGroundComponent>());

		m_timeOffset = bx::getHPCounter();

		// Update all entities
		for (auto& entity : entities)
		{
			entity->setCarObject(car);
			entity->init();
			entity->getCarObject(car);		}
	}

	virtual int shutdown() BX_OVERRIDE
	{
		// Cleanup.
		entities.clear();

		// Shutdown bgfx.
		bgfx::shutdown();

		return 0;
	}

	bool update() BX_OVERRIDE
	{
		if (!entry::processEvents(m_width, m_height, m_debug, m_reset))
		{
			int64_t now = bx::getHPCounter();
			static int64_t last = now;
			const int64_t frameTime = now - last;
			last = now;
			const double freq = double(bx::getHPFrequency());
			const double toMs = 1000.0 / freq;

			float time = (float)((now - m_timeOffset) / double(bx::getHPFrequency()));

			// Use debug font to print information about this example.
			bgfx::dbgTextClear();
			bgfx::dbgTextPrintf(2, 1, 0x0f, "Frame: % 7.3f[ms]", double(frameTime)*toMs);

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, m_width, m_height);

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			//--
			car.update();

			glm::mat4 playerRotationMat;
			car.getRotationMTX(playerRotationMat);
			glm::vec3 ViewTarget;
			car.getCarPos(ViewTarget);
			static glm::vec4 View1OFFe;
			//View1OFFe = playerRotationMat * glm::vec4(0, 15.0f, -25.0f, 1);
			//View1OFFe = playerRotationMat * glm::vec4(0, 300.0f, -300.0f, 1); //testing set
			View1OFFe = glm::vec4(0, 15.0f, -15.0f, 1); //testing set
			glm::vec3 View1eye = glm::vec3(ViewTarget.x + View1OFFe.x, ViewTarget.y + View1OFFe.y, ViewTarget.z + View1OFFe.z);

			float at[3] = { ViewTarget.x, ViewTarget.y,  ViewTarget.z };
			float eye[3] = { View1eye.x, View1eye.y, View1eye.z };

			float view[16];
			bx::mtxLookAt(view, eye, at);

			float proj[16];
			bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 10000.0f);
			bgfx::setViewTransform(0, view, proj);

			// Update all entities
			for (auto& entity : entities)
			{
				entity->setCarObject(car);
				entity->update();
				entity->getCarObject(car);
			}

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();

			if (inputGetKeyState(entry::Key::Esc))
			{
				return false;
			}

			return true;
		}

		return false;
	}

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
	int64_t m_timeOffset;
};

ENTRY_IMPLEMENT_MAIN(MyGame);
