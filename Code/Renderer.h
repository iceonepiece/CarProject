#pragma once

#include <learnopengl/shader_m.h>
#include "Camera.h"

class Renderer
{
public:
	Renderer();

	void BeginFrame(const Camera& camera) {}

private:
	void InitializeDepthMap();


	unsigned int depthMapFBO;
	unsigned int depthMap;

	Shader m_baseShader;
	Shader m_depthShader;
};