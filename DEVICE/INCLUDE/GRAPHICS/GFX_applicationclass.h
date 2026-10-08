#pragma once

#include <memory>

#include "SYS_inputclass.h"

#include "GFX_d3dclass.h"
#include "GFX_camera.h"
#include "GFX_model.h"
#include "GFX_plane.h"
#include "GFX_pointlight.h"
#include "GFX_directional_light.h"

#include "SYS_imgui_manager.h"

// GLOBALS
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class ApplicationClass
{
public:
	ApplicationClass(int screenWidth, int screenHeight, HWND hwnd);
	~ApplicationClass() = default;

	void Frame(InputClass*, float, bool);
	void Render(float);

private:
	ImguiManager imgui;

	void SpawnControlWindow();
private:
	std::unique_ptr<D3DClass> m_Direct3D;
	std::unique_ptr<Camera> m_Camera;
	
	Model m_nano;
	Plane m_plane;
	
	PointLight m_pointlight;
	DirectionalLight m_dirLight;

	float m_simulationSpeed = 1.0f;
};

