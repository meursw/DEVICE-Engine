#include "GFX_topology.h"
#include "GFX_bindablecodex.h"


Topology::Topology(D3DClass* d3d, D3D11_PRIMITIVE_TOPOLOGY topologyType)
	:
	m_topologyType(topologyType) 
{}

void Topology::Bind(D3DClass* d3d)
{
	d3d->GetDeviceContext()->IASetPrimitiveTopology(m_topologyType);
}

std::shared_ptr<Bindable> Topology::Resolve(D3DClass* d3d, D3D11_PRIMITIVE_TOPOLOGY type)
{
	return BindableCodex::Resolve<Topology>(d3d, type);
}

std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type)
{
	using namespace std::string_literals;
	return typeid(Topology).name() + "#"s + std::to_string(type);
}

std::string Topology::GetUID() const
{
	return GenerateUID(m_topologyType);
}


