#include "GFX_applicationclass.h"

ApplicationClass::ApplicationClass(int screenWidth, int screenHeight, HWND hwnd)
{
	// Initialize the direct 3D object
	m_Direct3D = std::make_unique<D3DClass>(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	m_vertexShader = std::make_unique<VertexShader>(
		ShaderType::VERTEX_SHADER,
		m_Direct3D->GetDevice(),
		hwnd,
		L"SHADERS/vert.vs", "VertEntry"
	);

	m_pixelShader = std::make_unique<PixelShader>(
		ShaderType::PIXEL_SHADER,
		m_Direct3D->GetDevice(),
		hwnd,
		L"SHADERS/pix.ps",
		"PixEntry"
	);

	VertexLayout vl;
	vl.Append(VertexLayout::Position3D);
	vl.Append(VertexLayout::Normal);

	m_inputLayout = std::make_unique<InputLayout>(m_Direct3D->GetDevice(), vl, m_vertexShader->GetBytecode());
}

bool ApplicationClass::Frame(InputClass* m_Input)
{
	if (m_Input->IsEscapePressed())
		return false;

	return true;
}

bool ApplicationClass::Render()
{

	return true;
} 
