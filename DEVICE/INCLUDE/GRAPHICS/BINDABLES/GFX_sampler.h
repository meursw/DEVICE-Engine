#pragma once
#include "GFX_bindable.h"
class Sampler: public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	Sampler(D3DClass* d3d);

public:
	static std::shared_ptr<Sampler> Resolve(D3DClass* d3d);
	static std::string GenerateUID();
	std::string GetUID() const override;

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;
};

