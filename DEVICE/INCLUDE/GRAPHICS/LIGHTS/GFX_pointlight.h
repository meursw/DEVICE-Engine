#pragma once

#include "GFX_d3dclass.h"
#include "GFX_constantbuffer.h"
#include "GFX_sphere.h"

class PointLight
{
private:
	struct PointLightCbuf
	{
		alignas(16) DirectX::XMFLOAT3 lightPos;
		alignas(16) DirectX::XMFLOAT4 pointDiffuseColor;
		DirectX::XMFLOAT4 ambientColor;

		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};

public:
	PointLight(D3DClass*, float radius = 0.5f);
	void Draw(D3DClass*);
	void Bind(D3DClass*);

	// imgui
	void SpawnControlWindow();
	void Reset();

private:
	PixelConstantBuffer<PointLightCbuf> m_pointLightCbuf;

	// Point light settings.
	PointLightCbuf cbData;
private:
	Sphere m_mesh;
};

