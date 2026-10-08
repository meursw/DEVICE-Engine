#include "GFX_indexbuffer.h"
#include "GFX_bindablecodex.h"
#include <vector>

IndexBuffer::IndexBuffer(D3DClass* d3d, const std::vector<unsigned short>& indices)
	:
	IndexBuffer(d3d, "?", indices)
{}

IndexBuffer::IndexBuffer(D3DClass* d3d, std::string tag, const std::vector<unsigned short>& indices)
	:
	m_tag(tag),
	m_indexCount((UINT)indices.size())
{
	HRESULT hr;

	D3D11_BUFFER_DESC indexBufferDesc{};

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

	D3D_THROW(d3d->GetDevice()->CreateBuffer(
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

std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(D3DClass* d3d, const std::string& tag,
	const std::vector<unsigned short>& indices)
{
	assert(tag != "?");
	return BindableCodex::Resolve<IndexBuffer>(d3d, tag, indices);
}

std::string IndexBuffer::GenerateUID_(const std::string& tag)
{
	using namespace std::string_literals;
	return typeid(IndexBuffer).name() + "#"s + tag;
}

std::string IndexBuffer::GetUID() const
{
	return GenerateUID_(m_tag);
}
