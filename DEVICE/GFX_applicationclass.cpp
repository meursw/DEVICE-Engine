#include "GFX_applicationclass.h"

ApplicationClass::ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Initialize the direct 3D object
	m_Direct3D = std::make_unique<D3DClass>(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	m_vertexShader = std::make_unique<VertexShader>(ShaderType::VERTEX_SHADER);
	if (!m_vertexShader->InitializeShader(m_Direct3D->GetDevice(), hwnd, L"vert.vs", "VertEntry"))
		return false;

	m_pixelShader = std::make_unique<PixelShader>(ShaderType::PIXEL_SHADER);
	if (!m_pixelShader->InitializeShader(m_Direct3D->GetDevice(), hwnd, L"pix.ps", "PixEntry"))
		return false;

	VertexLayout vl;
	vl.Append(VertexLayout::Position3D);
	vl.Append(VertexLayout::Normal);

	m_inputLayout = std::make_unique<InputLayout>();
	m_inputLayout->SetVertexLayout(vl);
	if (!m_inputLayout->Initialize(m_Direct3D->GetDevice(), m_vertexShader->GetBytecode()))
		return false;


	return true;
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
