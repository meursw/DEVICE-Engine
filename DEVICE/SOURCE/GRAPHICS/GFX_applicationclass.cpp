#include "GFX_applicationclass.h"

ApplicationClass::ApplicationClass(int screenWidth, int screenHeight, HWND hwnd)
{
	// Initialize the direct 3D object.
	m_Direct3D = std::make_unique<D3DClass>(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	// Create the camera.
	m_Camera = std::make_unique<Camera>();
	m_Camera->SetPosition({ 0.0, 0.0, -2.0f });

	// Create the timer.
	m_Timer = std::make_unique<TimerClass>();

	m_vertexBuffer = std::make_unique<VertexBuffer>(m_Direct3D->GetDevice());

	m_constantMatrixBuffer = std::make_unique<ConstantBuffer>(m_Direct3D->GetDevice());

	m_vertexShader = std::make_unique<VertexShader>(
		ShaderType::VERTEX_SHADER,
		m_Direct3D->GetDevice(),
		hwnd,
		L"SHADERS/triangle.vs", 
		"TriangleVertexEntry"
	);

	m_pixelShader = std::make_unique<PixelShader>(
		ShaderType::PIXEL_SHADER,
		m_Direct3D->GetDevice(),
		hwnd,
		L"SHADERS/triangle.ps",
		"TrianglePixelEntry"
	);

	m_inputLayout = std::make_unique<InputLayout>(m_Direct3D->GetDevice(), m_vertexShader->GetBytecode());



}

void ApplicationClass::Frame(InputClass* m_Input)
{
	if (m_Input->IsEscapePressed()) {
		PostQuitMessage(0);
		return;
	}

	Render();
}

void ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	m_Camera->UpdateViewMatrix();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	ID3D11DeviceContext* deviceContext = m_Direct3D->GetDeviceContext();

	m_Direct3D->BeginScene(0.0, 0.0, 0.0, 1.0);

	m_constantMatrixBuffer->UpdateConstantBuffer(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	m_constantMatrixBuffer->Bind(deviceContext);
	
	m_inputLayout->Bind(deviceContext);

	m_vertexShader->Bind(deviceContext);
	m_pixelShader->Bind(deviceContext);


	static float elapsedTime{ 0.0 };
	static int depth{ 0 };
	static int sign{ 1 };

	m_Timer->Frame();
	elapsedTime += m_Timer->GetDeltaTime();

	if(elapsedTime >= 0.7f)
	{
		if (depth == 7)
			sign = -1;
		else if (depth == 0)
			sign = 1;
		depth += sign;

		elapsedTime = 0.0f;
	}

	m_vertexBuffer->UpdateVertexBuffer(
		m_Direct3D->GetDeviceContext(),
		depth,
		XMFLOAT3(-1.0f, -1.0f, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT3(1.0f, -1.0f, 0.0f)
	);



	m_Direct3D->EndScene();
}