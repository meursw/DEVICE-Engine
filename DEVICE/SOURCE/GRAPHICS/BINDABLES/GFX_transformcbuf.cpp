#include "GFX_transformcbuf.h"
#include "GFX_bindablecodex.h"

using namespace DirectX;


TransformCbuf::TransformCbuf(D3DClass* d3d, const Drawable& parent, UINT slot)
	: 
	parent(parent)
{
	if (!m_transformBuffer)
		m_transformBuffer = std::make_unique<VertexConstantBuffer<TransformsBuffer>>(d3d, slot);
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