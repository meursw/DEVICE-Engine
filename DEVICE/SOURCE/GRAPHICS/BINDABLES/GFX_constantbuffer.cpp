#include "GFX_constantbuffer.h"

using namespace DirectX;

ConstantBuffer::ConstantBuffer(ID3D11Device* device)
{
	HRESULT hr;
	D3D11_BUFFER_DESC cbufferDesc{};

	cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbufferDesc.ByteWidth = sizeof(MatrixBufferType);
	cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbufferDesc.MiscFlags = 0;
	cbufferDesc.StructureByteStride = 0;

	D3D_THROW(device->CreateBuffer(
		&cbufferDesc,
		NULL,
		m_constantBuffer.GetAddressOf()
	));
}

void ConstantBuffer::UpdateConstantBuffer(
	ID3D11DeviceContext* deviceContext,
	XMMATRIX world,
	XMMATRIX view,
	XMMATRIX projection)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	D3D_THROW(deviceContext->Map(
		m_constantBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource
	));

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	deviceContext->Unmap(m_constantBuffer.Get(), 0);
}

void ConstantBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
}

