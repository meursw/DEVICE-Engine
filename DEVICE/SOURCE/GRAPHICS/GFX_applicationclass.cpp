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
	m_nano(m_Direct3D.get(), "../DEVICE/ASSETS/MODELS/DRYBONES.glb", hwnd),
	m_simulationSpeed(1.0f)
{
	// Create the camera.
	m_Camera = std::make_unique<Camera>(screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	m_Camera->SetPosition({ 0.0, 8.0, 12.0f });
	m_Camera->SetRotation({ 0.0f, -90.0f, 0.0f });
	m_Camera->UpdateViewMatrix();

	m_Direct3D->SetCamera(m_Camera.get());
}

void ApplicationClass::Frame(InputClass* m_Input, float delta, bool cursorEnabled)
{
	m_Camera->Update(m_Input, delta, cursorEnabled);
	Render(delta * m_simulationSpeed);
}

void ApplicationClass::Render(float delta)
{
	static float elapsedTime{ 0.0 };
	elapsedTime += delta;

	m_Direct3D->BeginScene(0.0, 0.0, 0.0, 1.0);

	auto d3d = m_Direct3D.get();

	m_pointlight.Bind(d3d);
	m_dirLight.Bind(d3d, m_Camera.get());

	m_nano.Draw(d3d);

	//m_pointlight.Draw(d3d);
	
	m_Camera->SpawnControlWindow();
	m_pointlight.SpawnControlWindow();
	m_dirLight.SpawnControlWindow();
	m_nano.ShowWindow("Model");
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
