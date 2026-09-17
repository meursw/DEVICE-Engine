#pragma once

#include <memory>

#include "SYS_inputclass.h"

#include "GFX_d3dclass.h"
#include "GFX_camera.h"

#include "GFX_vertexbuffer.h"
#include "GFX_inputlayout.h"
#include "GFX_constantbuffer.h"

#include "GFX_vertexshader.h"
#include "GFX_pixelshader.h"

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

	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<InputLayout> m_inputLayout;
	std::unique_ptr<ConstantBuffer> m_constantMatrixBuffer;

	std::unique_ptr<VertexShader> m_vertexShader;
	std::unique_ptr<PixelShader> m_pixelShader;
};

