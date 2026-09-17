#pragma once

#include "GFX_bindable.h"
#include <directxmath.h>
#include <vector>

class VertexBuffer : public Bindable
{
public:
	void Bind(ID3D11DeviceContext*);

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	VertexBuffer(ID3D11Device*);
	~VertexBuffer() = default;

	int GetIndexCount() const;

	void UpdateVertexBuffer(ID3D11DeviceContext*, int, DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c);

	int GetVertexCount() const;

private:
	void Sierpinsky(int, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3, ID3D11DeviceContext*);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

	int m_vertexCount, m_indexCount;
};

