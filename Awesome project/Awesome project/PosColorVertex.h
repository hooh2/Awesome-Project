#pragma once

#include <common\bgfx_utils.h>

struct PosColorVertex
{

	float m_x;
	float m_y;
	float m_z;
	//uint32_t m_abgr;

	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexDecl ms_decl;

};