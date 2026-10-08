#include "GFX_pointlight.h"
#include "IMGUI/imgui.h"

PointLight::PointLight(D3DClass* d3d, float radius)
	:
	m_pointLightCbuf(d3d, 1u),
	m_mesh(d3d, NULL, radius)
{
	Reset();
}

void PointLight::Draw(D3DClass* d3d)
{
	m_mesh.SetPos(cbData.lightPos);
	m_mesh.Draw(d3d);
}

void PointLight::Bind(D3DClass* d3d)
{
	m_mesh.Update(d3d, cbData.pointDiffuseColor);

	m_pointLightCbuf.Update(d3d->GetDeviceContext(), cbData);
	m_pointLightCbuf.Bind(d3d);
}

void PointLight::SpawnControlWindow()
{
	if (ImGui::Begin("Point Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &cbData.lightPos.x, -5.0f, 5.0f, "%.1f");
		ImGui::SliderFloat("Y", &cbData.lightPos.y, -5.0f, 5.0f, "%.1f");
		ImGui::SliderFloat("Z", &cbData.lightPos.z, -5.0f, 5.0f, "%.1f");

		ImGui::Text("Color/Intensity");
		ImGui::ColorEdit4("Diffuse Color", &cbData.pointDiffuseColor.x);
		ImGui::SliderFloat("Intensity", &cbData.diffuseIntensity, 0.0f, 3.0f, "%.1f");

		ImGui::Text("Attenuation");
		ImGui::SliderFloat("Constant", &cbData.attConst, 1.0f, 5.0f, "%.1f");
		ImGui::SliderFloat("Linear", &cbData.attLin, 0.0f, 1.0f, "%.3f");
		ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.0f, 1.0f, "%.4f");

		if (ImGui::Button("Reset"))
			Reset();
	}
	ImGui::End();
}

void PointLight::Reset()
{
	cbData = {
		{1.0f,1.0f,-2.0f},
		0.0f,
		{1.0f,1.0f,1.0f,1.0f},
		1.0f,
		1.0f,
		0.001f,
		0.00075f
	};
}