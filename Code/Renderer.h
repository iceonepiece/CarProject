#pragma once

#include <learnopengl/shader_m.h>
#include "Camera.h"
#include "Light.h"

#include <vector>

class Renderer
{
public:
	Renderer();
	~Renderer();

	void BeginFrame(Camera& camera);

	void DrawPlane();
	void DrawCube();

	void RenderDepthMap(const glm::mat4& lightSpaceMatrix);
	void RenderLighting(const glm::vec3& lightPosition, const glm::mat4& lightSpaceMatrix);

	Shader m_baseShader;
	Shader m_depthShader;

	unsigned int m_depthMapFBO;
	unsigned int m_depthMap;

	unsigned int m_planeVBO;
	unsigned int m_planeVAO;

	unsigned int m_cubeVAO = 0;
	unsigned int m_cubeVBO = 0;

private:
	void SetupDepthMap();
	void SetupPlane();
	void SetupCube();

	Camera* m_camera = nullptr;
	static Camera s_defaultCamera;

	std::vector<Light> m_lights;
};