#include "GFX_bindablecodex.h"
#include "GFX_transformcbufdouble.h"

using namespace DirectX;

TransformCbufDouble::TransformCbufDouble(D3DClass* d3d, const Drawable& parent, UINT slotVS, UINT slotPS)
	:
	TransformCbuf(d3d, parent, slotVS)
{
	if (!m_transformBufferPS)
		m_transformBufferPS = std::make_unique<PixelConstantBuffer<TransformsBuffer>>(d3d, slotPS);
}


void TransformCbufDouble::Bind(D3DClass* d3d)
{
	const auto tf = GetTransforms(d3d);
	TransformCbuf::UpdateBindImpl(d3d, tf);
	UpdateBindImpl(d3d, tf);
}

void TransformCbufDouble::UpdateBindImpl(D3DClass* d3d, const TransformsBuffer& tf)
{
	m_transformBufferPS->Update(d3d->GetDeviceContext(), tf);
	m_transformBufferPS->Bind(d3d);
}

std::unique_ptr<PixelConstantBuffer<TransformCbuf::TransformsBuffer>> TransformCbufDouble::m_transformBufferPS;