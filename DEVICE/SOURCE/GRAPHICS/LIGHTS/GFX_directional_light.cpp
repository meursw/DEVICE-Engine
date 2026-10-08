#include "GFX_directional_light.h"
#include "IMGUI/imgui.h"

DirectionalLight::DirectionalLight(D3DClass* d3d) 
	:
	m_vertexCameraCbuf(d3d, 1u),
	m_dirLightCbuf(d3d)
{
	Reset();
}

void DirectionalLight::Bind(D3DClass* d3d, Camera* camera)
{
	m_vertexCameraCbuf.Update(d3d->GetDeviceContext(), { camera->GetPosition() });
	m_vertexCameraCbuf.Bind(d3d);

	m_dirLightCbuf.Update(d3d->GetDeviceContext(), cbData);
	m_dirLightCbuf.Bind(d3d);
}

void DirectionalLight::SpawnControlWindow()
{
	if (ImGui::Begin("Directional Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &cbData.lightDirection.x, -1.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("Y", &cbData.lightDirection.y, -1.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("Z", &cbData.lightDirection.z, -1.0f, 1.0f, "%.1f");

		ImGui::Text("Color/Specular");
		ImGui::ColorEdit4("Diffuse Color", &cbData.dirDiffuseColor.x);
		ImGui::ColorEdit4("Ambient Color", &cbData.dirAmbientColor.x);
		ImGui::ColorEdit4("Specular Color", &cbData.specularColor.x);
		ImGui::SliderFloat("Specular Power", &cbData.specularPower, 0.0f, 256.0f, "%.1f");

		if (ImGui::Button("Reset"))
			Reset();
	}
	ImGui::End();
}

void DirectionalLight::Reset()
{
	cbData = {
		{0.0f, -1.0f, 0.0f },
		8.0f,
		{1.0f,1.0f,1.0f,1.0f},
		{0.05f,0.05f,0.05f,1.0f},
		{1.0f,1.0f,1.0f,1.0f}
	};
}

