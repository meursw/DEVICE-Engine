#pragma once
#include "GFX_drawablebase.h"

class Sphere : public DrawableBase<Sphere>
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
	DirectX::XMMATRIX GetTransformXM() const;
	
private:
	DirectX::XMFLOAT3 m_pos = {};
	DirectX::XMFLOAT3 m_color;
};

