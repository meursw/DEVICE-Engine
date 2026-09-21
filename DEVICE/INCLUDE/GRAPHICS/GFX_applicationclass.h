#pragma once

#include <memory>

#include "SYS_inputclass.h"

#include "GFX_d3dclass.h"
#include "GFX_camera.h"
#include "GFX_cube.h"

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
	std::unique_ptr<D3DClass> m_Direct3D;
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<Camera> m_Camera2;
	std::vector<std::unique_ptr<Cube>> m_Cubes;
};

