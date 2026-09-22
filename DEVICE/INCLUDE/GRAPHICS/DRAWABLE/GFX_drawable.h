#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <random>

#include "GFX_bindable.h"

class Drawable
{
	template<class T>
	friend class DrawableBase;

public:
	Drawable(HWND);
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

public:
	void Draw(D3DClass*) const;
	virtual void Update(float) = 0;
	virtual DirectX::XMMATRIX GetTransformXM() const = 0;

protected:
	void AddBind(std::unique_ptr<Bindable>);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer>);

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const = 0;

private:
	std::vector<std::unique_ptr<Bindable>> binds;
	const class IndexBuffer* m_indexBuffer = nullptr;

protected:
	// Store hwnd for showing shader error compilation.
	HWND m_hwnd;
};

