#pragma once
#include "GFX_bindable.h"

class Topology : public Bindable
{
public:
	Topology(D3DClass*, D3D11_PRIMITIVE_TOPOLOGY topologyType);
	void Bind(D3DClass*);

public:
	static std::shared_ptr<Bindable> Resolve(D3DClass* d3d, D3D11_PRIMITIVE_TOPOLOGY type);
	static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
	std::string GetUID() const override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY m_topologyType;
};

