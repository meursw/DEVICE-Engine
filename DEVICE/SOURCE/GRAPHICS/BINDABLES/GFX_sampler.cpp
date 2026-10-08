#include "GFX_sampler.h"
#include "GFX_bindablecodex.h"


Sampler::Sampler(D3DClass* d3d)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 1000.0f;

	HRESULT hr;

	D3D_THROW(d3d->GetDevice()->CreateSamplerState(
		&samplerDesc,
		m_sampler.GetAddressOf()
	));
}

std::shared_ptr<Sampler> Sampler::Resolve(D3DClass* d3d)
{
	return BindableCodex::Resolve<Sampler>(d3d);
}

std::string Sampler::GenerateUID()
{
	return typeid(Sampler).name();
}

std::string Sampler::GetUID() const
{
	return GenerateUID();
}

void Sampler::Bind(D3DClass* d3d)
{
	d3d->GetDeviceContext()->PSSetSamplers(0u, 1u, m_sampler.GetAddressOf());
}
