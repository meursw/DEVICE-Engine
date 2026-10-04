#include "GFX_vertexbuffer.h"

using namespace DirectX;

void VertexBuffer::Bind(D3DClass* d3d)
{
	unsigned int offset{};

	d3d->GetDeviceContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &offset);
}

int VertexBuffer::GetVertexCount() const
{
	return m_vertexCount;
}
