#include "GFX_applicationclass.h"

ApplicationClass::ApplicationClass(int screenWidth, int screenHeight, HWND hwnd)
{
	// Initialize the direct 3D object.
	m_Direct3D = std::make_unique<D3DClass>(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	// Create the camera.
	m_Camera = std::make_unique<Camera>(screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	m_Camera->SetPosition({ 0.0, 0.0, -60.0f });
	m_Camera->UpdateViewMatrix();
	
	m_Camera2 = std::make_unique<Camera>(screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	m_Camera2->SetPosition({ 0.0, 160.0, 0.0 });
	m_Camera2->SetRotation({ 90.0f,0.0,0.0 });
	m_Camera2->UpdateViewMatrix();

	m_Direct3D->SetCamera(m_Camera.get());

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> a(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> b(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> c(0.0f, 3.1415f * 0.2f);
	std::uniform_real_distribution<float> d(6.0f, 75.0f);
	std::uniform_real_distribution<float> scale(0.5f, 3.0f);

	for(int i = 0; i < 250; i++)
		m_Cubes.push_back(std::make_unique<Cube>(
			m_Direct3D->GetDevice(),
			hwnd,
			rng,
			a,b,c,d,scale
		));
}

void ApplicationClass::Frame(InputClass* m_Input, float delta)
{
	Render(delta);
}

void ApplicationClass::Render(float delta)
{
	static float elapsedTime{ 0.0 };
	elapsedTime += delta;

	if (elapsedTime >= 5.0f)
	{
		if (m_Direct3D->GetCamera() == m_Camera.get())
			m_Direct3D->SetCamera(m_Camera2.get());
		else
			m_Direct3D->SetCamera(m_Camera.get());
		elapsedTime -= 5.0f;
	}

	m_Direct3D->UpdateCurrentCamera();

	m_Direct3D->BeginScene(0.0, 0.0, 0.0, 1.0);

	for (int i = 0; i < 250; i++)
	{
		m_Cubes[i]->Update(delta);
		m_Cubes[i]->Draw(m_Direct3D.get());
	}

	m_Direct3D->EndScene();
}

