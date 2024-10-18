#pragma once

#include <learnopengl/shader_m.h>
#include "Camera.h"
#include "Light.h"

#include <vector>

class Renderer
{
public:
	Renderer();

	void BeginFrame(Camera camera);

	void DrawPlane(glm::mat4& model, Shader& shader);

	Shader m_baseShader;
	Shader m_depthShader;


	unsigned int depthMapFBO;
	unsigned int depthMap;


private:
	void InitializeDepthMap();

	Camera m_camera;


	std::vector<Light> m_lights;

};