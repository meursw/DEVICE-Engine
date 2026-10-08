#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <random>

#include "GFX_bindable.h"

// Every class that inherits from Drawable needs a number of bindables so it can be drawn.
// These bindables are created as shared_ptr and are shared between each drawable/mesh.
// "BindableCodex" is a class that stores all of the bindables that have been created
// and enables sharing bindables between drawables.

class Drawable
{
public:
	Drawable();
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

public:
	void Draw(D3DClass*) const;
	virtual DirectX::XMMATRIX GetTransformXM() const = 0;

protected:
	template<class T>
	T* QueryBindable()
	{
		for (auto& bind : m_binds)
		{
			if ( auto p = dynamic_cast<T*>(bind.get()) )
				return p;
		}

		return nullptr;
	}

	void AddBind(std::shared_ptr<Bindable>);

private:
	std::vector<std::shared_ptr<Bindable>> m_binds;
	const class IndexBuffer* m_indexBuffer = nullptr;
};

