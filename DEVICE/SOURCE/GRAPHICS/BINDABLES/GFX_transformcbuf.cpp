#include "GFX_transformcbuf.h"

using namespace DirectX;

TransformCbuf::TransformCbuf(ID3D11Device* device, const Drawable& parent)
	: 
	vcbuf(device),
	parent(parent)
{}

void TransformCbuf::Bind(D3DClass* d3d)
{
	XMMATRIX world, view, projection;
	world = XMMatrixTranspose(parent.GetTransformXM());

	d3d->GetCamera()->GetViewMatrix(view);
	view = XMMatrixTranspose(view);

	d3d->GetCamera()->GetProjectionMatrix(projection);
	projection = XMMatrixTranspose(projection);

	MatrixBufferType mvp{ world, view, projection };
	vcbuf.Update(d3d->GetDeviceContext(), mvp);

	vcbuf.Bind(d3d);
}