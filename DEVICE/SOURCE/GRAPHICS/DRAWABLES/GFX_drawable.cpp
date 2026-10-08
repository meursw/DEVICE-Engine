#include <cassert>
#include <typeinfo>

#include "GFX_drawable.h"
#include "GFX_indexbuffer.h"

Drawable::Drawable() : m_indexBuffer(nullptr) {}

void Drawable::Draw(D3DClass* d3d) const
{
	for (auto& b : m_binds)
		b->Bind(d3d);

	d3d->GetDeviceContext()->DrawIndexed(m_indexBuffer->GetIndexCount(), 0u, 0u);
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind)
{	 
	// Take a pointer to the index buffer.
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Binding multiple index buffers is not allowed" && m_indexBuffer == nullptr);
		m_indexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	m_binds.push_back(std::move(bind));
}