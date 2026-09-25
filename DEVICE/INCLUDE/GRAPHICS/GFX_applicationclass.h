#pragma once

#include <memory>

#include "SYS_inputclass.h"

#include "GFX_d3dclass.h"
#include "GFX_camera.h"
#include "GFX_box.h"

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

	void Frame(InputClass*, float);
	void Render(float);

private:
	ImguiManager imgui;

private:
	std::unique_ptr<D3DClass> m_Direct3D;
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<Camera> m_Camera2;
	std::vector<std::unique_ptr<Box>> m_Boxes;

	float speed_factor = 1.0f;
};

