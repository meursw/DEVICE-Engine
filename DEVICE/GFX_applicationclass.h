#pragma once

#include <memory>

#include "SYS_inputclass.h"

#include "GFX_d3dclass.h"
#include "GFX_vertexshader.h"
#include "GFX_pixelshader.h"
#include "GFX_inputlayout.h"

// GLOBALS
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class ApplicationClass
{
public:
	ApplicationClass();
	~ApplicationClass() = default;

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);

	bool Frame(InputClass*);
	bool Render();

private:
	std::unique_ptr<D3DClass> m_Direct3D;
	std::unique_ptr<VertexShader> m_vertexShader;
	std::unique_ptr<PixelShader> m_pixelShader;
	std::unique_ptr<InputLayout> m_inputLayout;
};

