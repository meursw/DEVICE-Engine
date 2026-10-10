#pragma once
#include "GFX_drawable.h"

class Sphere : public Drawable
{
private:
	struct PSColorBuf
	{
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f, 1.0f};
	};

public:
	Sphere(D3DClass*, HWND, float radius);
	void Update(D3DClass*, DirectX::XMFLOAT4);
	void SetPos(DirectX::XMFLOAT3);
	DirectX::XMMATRIX GetTransformXM() const override;
	
private:
	DirectX::XMFLOAT3 m_pos = {};
	DirectX::XMFLOAT3 m_color;
};

