#pragma once
#include "GFX_constantbuffer.h"
#include "GFX_drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
	struct TransformsBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

public:
	void Bind(D3DClass*) override;

public:
	TransformCbuf(ID3D11Device*, const Drawable& parent);

private:
	static std::unique_ptr<VertexConstantBuffer<TransformsBuffer>> m_transformBuffer;
	const Drawable& parent;
};

