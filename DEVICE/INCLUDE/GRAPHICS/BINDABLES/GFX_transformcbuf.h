#pragma once

#include "GFX_constantbuffer.h"
#include "GFX_drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
protected:
	struct TransformsBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

public:
	void Bind(D3DClass*) override;

public:
	TransformCbuf(D3DClass*, const Drawable& parent, UINT slot = 0);

protected:
	void UpdateBindImpl(D3DClass*, const TransformsBuffer& tf);
	TransformsBuffer GetTransforms(D3DClass*);

private:
	static std::unique_ptr<VertexConstantBuffer<TransformsBuffer>> m_transformBufferVS;
	const Drawable& parent;
};

