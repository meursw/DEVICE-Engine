#include "GFX_transformcbuf.h"

using namespace DirectX;


TransformCbuf::TransformCbuf(ID3D11Device* device, const Drawable& parent)
	: 
	parent(parent)
{
	if (!m_transformBuffer)
		m_transformBuffer = std::make_unique<VertexConstantBuffer<TransformsBuffer>>(device);
}

void TransformCbuf::Bind(D3DClass* d3d)
{
	XMMATRIX world, view, projection;
	world = XMMatrixTranspose(parent.GetTransformXM());

	d3d->GetCamera()->GetViewMatrix(view);
	view = XMMatrixTranspose(view);

	d3d->GetCamera()->GetProjectionMatrix(projection);
	projection = XMMatrixTranspose(projection);

	TransformsBuffer mvp{ world, view, projection };

	m_transformBuffer->Update(d3d->GetDeviceContext(), mvp);

	m_transformBuffer->Bind(d3d);
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::TransformsBuffer>> TransformCbuf::m_transformBuffer;