#pragma once
#include "GFX_constantbuffer.h"
#include "GFX_drawable.h"
#include "GFX_camera.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
private:
	struct MatrixBufferType
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
	VertexConstantBuffer<MatrixBufferType> vcbuf;
	const Drawable& parent;
};

