#pragma once
#include "GFX_bindable.h"
#include <directxmath.h>

class ConstantBuffer : public Bindable
{
public:
	void Bind(ID3D11DeviceContext*);

private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	ConstantBuffer(ID3D11Device*);
	~ConstantBuffer() = default;

	void UpdateConstantBuffer(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
};

