#include "GFX_transformcbuf.h"
#include "GFX_bindablecodex.h"

using namespace DirectX;


TransformCbuf::TransformCbuf(D3DClass* d3d, const Drawable& parent, UINT slot)
	: 
	parent(parent)
{
	if (!m_transformBufferVS)
		m_transformBufferVS = std::make_unique<VertexConstantBuffer<TransformsBuffer>>(d3d, slot);
}

void TransformCbuf::Bind(D3DClass* d3d)
{
	UpdateBindImpl(d3d, GetTransforms(d3d));
}


void TransformCbuf::UpdateBindImpl(D3DClass* d3d, const TransformsBuffer& tf)
{

	m_transformBufferVS->Update(d3d->GetDeviceContext(), tf);

	m_transformBufferVS->Bind(d3d);
}

TransformCbuf::TransformsBuffer TransformCbuf::GetTransforms(D3DClass* d3d)
{
	XMMATRIX world, view, projection;
	world = XMMatrixTranspose(parent.GetTransformXM());

	d3d->GetCamera()->GetViewMatrix(view);
	view = XMMatrixTranspose(view);

	d3d->GetCamera()->GetProjectionMatrix(projection);
	projection = XMMatrixTranspose(projection);

	return { world, view, projection };
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::TransformsBuffer>> TransformCbuf::m_transformBufferVS;