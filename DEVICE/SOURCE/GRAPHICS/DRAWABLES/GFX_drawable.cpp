#include <cassert>
#include <typeinfo>

#include "GFX_drawable.h"
#include "GFX_indexbuffer.h"

Drawable::Drawable() : m_indexBuffer(nullptr) {}

void Drawable::Draw(D3DClass* d3d) const
{
	for (auto& b : binds)
		b->Bind(d3d);

	for (auto& b : GetStaticBinds())
	{
		b->Bind(d3d);
	}

	d3d->GetDeviceContext()->DrawIndexed(m_indexBuffer->GetIndexCount(), 0u, 0u);
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	assert("*MUST* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf)
{
	assert("Attempting to add index buffer a second time" && m_indexBuffer == nullptr);
	m_indexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}