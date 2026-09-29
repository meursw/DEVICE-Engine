#include "GFX_applicationclass.h"

#include "IMGUI/imgui.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

ApplicationClass::ApplicationClass(int screenWidth, int screenHeight, HWND hwnd)
	:
	// Initialize the direct 3D object.
	m_Direct3D(std::make_unique<D3DClass>(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)),
	m_pointlight(m_Direct3D.get()),
	m_dirLight(m_Direct3D.get()),
	m_simulationSpeed(1.0f)
{
	// Create the camera.
	m_Camera = std::make_unique<Camera>(screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	m_Camera->SetPosition({ 0.0, 0.0, -10.0f });
	m_Camera->UpdateViewMatrix();

	m_Direct3D->SetCamera(m_Camera.get());

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> a(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> b(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> c(0.0f, 3.1415f * 0.2f);
	std::uniform_real_distribution<float> d(20.0f, 70.0f);
	std::uniform_real_distribution<float> scale(0.5f, 3.0f);
	std::uniform_real_distribution<float> color(0.0f, 1.0f);

	for (int i = 0; i < 1300; i++)
	{
		DirectX::XMFLOAT4 materialColor = {1.0f,1.0f, 1.0f, 1.0f};

		m_Boxes.push_back(std::make_unique<Box>(
			m_Direct3D.get(),
			hwnd,
			rng,
			a, b, c, d, scale, materialColor
		));
	}
}

void ApplicationClass::Frame(InputClass* m_Input, float delta, bool m_cursorLocked)
{
	m_Camera->Update(m_Input, delta, m_cursorLocked);
	Render(delta * m_simulationSpeed);
}

void ApplicationClass::Render(float delta)
{
	static float elapsedTime{ 0.0 };
	elapsedTime += delta;

	m_Direct3D->BeginScene(0.0, 0.0, 0.0, 1.0);

	m_pointlight.Bind(m_Direct3D.get());
	m_dirLight.Bind(m_Direct3D.get(), m_Camera.get());
	for (int i = 0; i < 300; i++)
	{
		m_Boxes[i]->Update(delta);
		m_Boxes[i]->Draw(m_Direct3D.get());
	}
	m_pointlight.Draw(m_Direct3D.get());
	
	m_Camera->SpawnControlWindow();
	m_pointlight.SpawnControlWindow();
	m_dirLight.SpawnControlWindow();
	SpawnControlWindow();

	// Present.
	m_Direct3D->EndScene();
}

void ApplicationClass::SpawnControlWindow()
{
	if (ImGui::Begin("Application"))
	{
		ImGui::Text("Simulation");
		ImGui::SliderFloat("Speed Factor", &m_simulationSpeed, 0.0f, 2.0f);
		ImGui::Text("Performance");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0 / double(ImGui::GetIO().Framerate), double(ImGui::GetIO().Framerate));
	}
	ImGui::End();
}

