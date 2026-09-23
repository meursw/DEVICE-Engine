#pragma once
#include "GFX_bindable.h"
class Sampler: public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	Sampler(ID3D11Device*);

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;
};

