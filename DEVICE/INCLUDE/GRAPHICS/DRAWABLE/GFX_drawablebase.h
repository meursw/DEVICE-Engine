#pragma once
#include "GFX_drawable.h"
#include "GFX_indexbuffer.h"

template<class T>
class DrawableBase : public Drawable
{
protected:

	DrawableBase() = default;

	void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf)
	{
		assert("Attempting to add index buffer a second time" && m_indexBuffer == nullptr);
		m_indexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}

	bool IsStaticInitialized() const
	{
		return !staticBinds.empty();
	}

	void SetIndexFromStatic()
	{
		assert("Attempting to add index buffer a second time" && m_indexBuffer == nullptr);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				m_indexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && m_indexBuffer == nullptr);
	}

private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const
	{
		return staticBinds;
	}

private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;