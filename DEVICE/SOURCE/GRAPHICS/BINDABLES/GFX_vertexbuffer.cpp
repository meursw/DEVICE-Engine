#include "GFX_vertexbuffer.h"
#include "GFX_bindablecodex.h"

using namespace DirectX;

void VertexBuffer::Bind(D3DClass* d3d)
{
	const UINT offset{};

	d3d->GetDeviceContext()->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &m_stride, &offset);
}

VertexBuffer::VertexBuffer(D3DClass* d3d, const DEVICE_VERTEX::VertexBuffer& vbuf)
	:
	VertexBuffer(d3d, "?", vbuf)
{}

VertexBuffer::VertexBuffer(D3DClass* d3d, const std::string & tag, const DEVICE_VERTEX::VertexBuffer & vbuf)
	:
	m_vertexCount((UINT)vbuf.Size()),
	m_stride((UINT)vbuf.GetLayout().Size()),
	m_tag(tag)
{
	HRESULT hr;

	D3D11_BUFFER_DESC vbufDesc{};

	vbufDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufDesc.ByteWidth = UINT(vbuf.SizeBytes());
	vbufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufDesc.CPUAccessFlags = 0;
	vbufDesc.MiscFlags = 0;
	vbufDesc.StructureByteStride = m_stride;

	D3D11_SUBRESOURCE_DATA vertexData{};
	vertexData.pSysMem = vbuf.GetData();

	D3D_THROW(d3d->GetDevice()->CreateBuffer(
		&vbufDesc, &vertexData, m_vertexBuffer.GetAddressOf()
	));
}

int VertexBuffer::GetVertexCount() const
{
	return m_vertexCount;
}

std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(
	D3DClass* d3d,
	const std::string& tag, 
	const DEVICE_VERTEX::VertexBuffer& vbuf
)
{
	assert(tag != "?");
	return BindableCodex::Resolve<VertexBuffer>(d3d, tag, vbuf);
}

std::string VertexBuffer::GenerateUID_(const std::string& tag)
{
	using namespace std::string_literals;
	return typeid(VertexBuffer).name() + "#"s + tag;
}

std::string VertexBuffer::GetUID() const
{
	return GenerateUID(m_tag);
}