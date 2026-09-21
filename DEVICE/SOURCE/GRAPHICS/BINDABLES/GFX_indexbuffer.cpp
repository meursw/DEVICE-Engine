#include "GFX_indexbuffer.h"
#include <vector>

IndexBuffer::IndexBuffer(ID3D11Device* device, const std::vector<unsigned short>& indices)
{
	HRESULT hr;

	D3D11_BUFFER_DESC indexBufferDesc{};

	m_indexCount = indices.size();

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	D3D_THROW(device->CreateBuffer(
		&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf()
	));
}

void IndexBuffer::Bind(D3DClass* d3d)
{
	d3d->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

UINT IndexBuffer::GetIndexCount() const
{
	return m_indexCount;
}
