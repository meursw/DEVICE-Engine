#pragma once
#include "GFX_drawable.h"
class Cube : public Drawable
{
public:
	Cube(D3DClass*);

	void SetPos(DirectX::XMFLOAT3);
	DirectX::XMMATRIX GetTransformXM() const override;

public:
	// IMGUI
	void SpawnControlWindow(D3DClass* d3d);

private:
	DirectX::XMFLOAT3 m_pos = {};
	struct Rotation
	{
		float pitch;
		float yaw;
		float roll;
	};
	Rotation m_rot{};

private:
	struct NormalMapCbuf
	{
		bool normalMapEnabled = true;
		float padding[3];
	} cbData;
};

