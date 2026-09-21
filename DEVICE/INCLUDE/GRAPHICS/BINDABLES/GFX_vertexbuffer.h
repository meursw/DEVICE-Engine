#pragma once

#include "GFX_bindable.h"
#include <directxmath.h>
#include <vector>

class VertexBuffer : public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	template<class V>
	VertexBuffer(ID3D11Device* device, const std::vector<V>& vertices)
		:
		m_stride(sizeof(V))
	{
		HRESULT hr;

		D3D11_BUFFER_DESC vertexBufferDesc{};
		
		m_vertexCount = vertices.size();
		
		// Fill vertex buffer desc.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(V) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = sizeof(V);

		D3D11_SUBRESOURCE_DATA vertexData{};
		// Fill vertex data.
		vertexData.pSysMem = vertices.data();
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Create vertex buffer.
		D3D_THROW(device->CreateBuffer(
			&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf()
		));
	}
	~VertexBuffer() = default;

	int GetVertexCount() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

	UINT m_vertexCount;
	UINT m_stride;
};

