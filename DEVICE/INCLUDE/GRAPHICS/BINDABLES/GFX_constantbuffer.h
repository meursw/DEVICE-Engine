#pragma once
#include "GFX_bindable.h"
#include <directxmath.h>

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(ID3D11Device* device, UINT slot) : m_slot(slot)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC cBufferDesc{};

		cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cBufferDesc.ByteWidth = sizeof(C);
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;

		D3D_THROW(device->CreateBuffer(
			&cBufferDesc, NULL, m_constantBuffer.GetAddressOf()
		));
	}

	ConstantBuffer(ID3D11Device* device, const C& consts, UINT slot) : m_slot(slot)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC cBufferDesc{};

		cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cBufferDesc.ByteWidth = sizeof(C);
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA cBufferData{};
		cBufferData.pSysMem = &consts;
		
		D3D_THROW(device->CreateBuffer(
			&cBufferDesc, &cBufferData, m_constantBuffer.GetAddressOf()
		));
	}

	void Update(ID3D11DeviceContext* deviceContext, const C& consts)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		D3D_THROW(deviceContext->Map(
			m_constantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&mappedResource
		));

		memcpy(mappedResource.pData, &consts, sizeof(consts));

		deviceContext->Unmap(m_constantBuffer.Get(), 0u);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	UINT m_slot;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::m_constantBuffer;
	using ConstantBuffer<C>::m_slot;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind(D3DClass* d3d) override
	{
		d3d->GetDeviceContext()->VSSetConstantBuffers(m_slot, 1u, m_constantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::m_constantBuffer;
	using ConstantBuffer<C>::m_slot;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind(D3DClass* d3d) override
	{
		d3d->GetDeviceContext()->PSSetConstantBuffers(m_slot, 1u, m_constantBuffer.GetAddressOf());
	}
};
