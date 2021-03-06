$input v_pos, v_view, v_normal, v_texcoord0

/*
 * Copyright 2013 Milos Tosic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../common/common.sh"

SAMPLER2D(s_texColor, 0);
SAMPLER2D(s_texStipple, 1);

vec2 blinn(vec3 _lightDir, vec3 _normal, vec3 _viewDir)
{
	float ndotl = dot(_normal, _lightDir);
	vec3 reflected = _lightDir - 2.0*ndotl*_normal; // reflect(_lightDir, _normal);
	float rdotv = dot(reflected, _viewDir);
	return vec2(ndotl, rdotv);
}

void main()
{
	vec4 color = texture2D(s_texColor, v_texcoord0);


	vec3 lightDir = vec3(0.0, -1.0, 0.0);
	vec3 normal = normalize(v_normal);
	vec3 view = normalize(v_view);
	vec2 bln = blinn(lightDir, normal, view);
	float l = saturate(bln.y) + 0.12;

	color.xyz = toLinear(color.xyz)*l;
	gl_FragColor = toGamma(color);
}

