#include "GFX_vertexbuffer.h"
#include <memory>

using namespace DirectX;

VertexBuffer::VertexBuffer(ID3D11Device* device)
{
	HRESULT hr;

	D3D11_BUFFER_DESC vertexBufferDesc{}, indexBufferDesc{};

	m_vertexCount = 3;
	m_indexCount = 3;

	// Fill vertex buffer desc
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Create vertex buffer.
	D3D_THROW(device->CreateBuffer(
		&vertexBufferDesc, NULL, m_vertexBuffer.GetAddressOf()
	));

	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D_THROW(device->CreateBuffer(
		&indexBufferDesc, NULL, m_indexBuffer.GetAddressOf()
	));

}

void VertexBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

int VertexBuffer::GetIndexCount() const
{
	return m_indexCount;
}

int VertexBuffer::GetVertexCount() const
{
	return m_vertexCount;
}


void VertexBuffer::UpdateVertexBuffer(ID3D11DeviceContext* deviceContext, int depth, XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c)
{
	Sierpinsky(depth, a, b, c, deviceContext);
}

void VertexBuffer::Sierpinsky(int depth, XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, ID3D11DeviceContext* deviceContext)
{
	XMFLOAT4 red	{ 0.022f, 0.929f, 0.412f, 1.0f };
	XMFLOAT4 green	{ 0.022f, 0.929f, 0.412f, 1.0f };
	XMFLOAT4 blue	{ 0.022f, 0.929f, 0.412f, 1.0f };

	if (depth <= 0)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		std::vector<VertexType> vertices;
		vertices.reserve(3);

		vertices.push_back({ a, red });
		vertices.push_back({ b, green });
		vertices.push_back({ c, blue });

		m_vertexCount = 3;

		D3D_THROW(deviceContext->Map(
			m_vertexBuffer.Get(), 0,
			D3D11_MAP_WRITE_DISCARD, 0,
			&mappedResource
		));

		memcpy(mappedResource.pData, vertices.data(), sizeof(VertexType) * m_vertexCount);

		deviceContext->Unmap(m_vertexBuffer.Get(), 0);

		Bind(deviceContext);

		deviceContext->Draw(m_vertexCount, 0);

		return;
	}
	
	depth -= 1;

	XMFLOAT3 p1{ (a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f, (a.z + b.z) * 0.5f };
	XMFLOAT3 p2{ (b.x + c.x) * 0.5f, (b.y + c.y) * 0.5f, (b.z + c.z) * 0.5f };
	XMFLOAT3 p3{ (a.x + c.x) * 0.5f, (a.y + c.y) * 0.5f, (a.z + c.z) * 0.5f };
	

	Sierpinsky(depth, a, p1, p3, deviceContext);
	Sierpinsky(depth, p1, b, p2, deviceContext);
	Sierpinsky(depth, p3, p2, c, deviceContext);
}
