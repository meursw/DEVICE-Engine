#pragma once
#include "GFX_bindable.h"

class Topology : public Bindable
{
public:
	Topology(D3D11_PRIMITIVE_TOPOLOGY topologyType);
	void Bind(D3DClass*);

protected:
	D3D11_PRIMITIVE_TOPOLOGY m_topologyType;
};

