#pragma once

#include "GFX_d3dclass.h"
#include "GFX_constantbuffer.h"

class DirectionalLight
{
private:
	struct DirLightCbuf
	{
		alignas(16) DirectX::XMFLOAT3 lightDir;
		alignas(16) DirectX::XMFLOAT4 dirDiffuseColor;
		alignas(16) DirectX::XMFLOAT4 dirAmbientColor;
		alignas(16) DirectX::XMFLOAT4 specularColor;
		float specularPower;
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

