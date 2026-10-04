#include "GFX_topology.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY topologyType) 
	:
	m_topologyType(topologyType) 
{}

void Topology::Bind(D3DClass* d3d)
{
	d3d->GetDeviceContext()->IASetPrimitiveTopology(m_topologyType);
}


