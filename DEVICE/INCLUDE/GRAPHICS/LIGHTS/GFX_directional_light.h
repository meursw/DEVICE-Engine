#pragma once

#include "GFX_d3dclass.h"
#include "GFX_constantbuffer.h"

class DirectionalLight
{
private:
	struct DirLightCbuf
	{
		DirectX::XMFLOAT3 lightDirection;
		float specularPower;
		DirectX::XMFLOAT4 dirDiffuseColor;
		DirectX::XMFLOAT4 dirAmbientColor;
		DirectX::XMFLOAT4 specularColor;
	};

	struct CameraCbuf
	{
		alignas(16) DirectX::XMFLOAT3 cameraPosition;
	};

public:
	DirectionalLight(D3DClass*);
	void Bind(D3DClass*, Camera*);

	// imgui
	void SpawnControlWindow();
	void Reset();

private:
	VertexConstantBuffer<CameraCbuf> m_vertexCameraCbuf;
	PixelConstantBuffer<DirLightCbuf> m_dirLightCbuf;

	// Directional light settings.
	DirLightCbuf cbData;

};

