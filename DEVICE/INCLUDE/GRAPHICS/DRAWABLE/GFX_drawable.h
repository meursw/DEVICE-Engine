#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <random>

#include "GFX_bindable.h"

class Drawable
{
public:
	Drawable(HWND);
	Drawable(const Drawable&) = delete;
	~Drawable() = default;

public:
	void Draw(D3DClass*) const;
	virtual void Update(float delta) = 0;

	void AddBind(std::unique_ptr<Bindable>);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer>);

	virtual DirectX::XMMATRIX GetTransformXM() const = 0;

private:
	std::vector<std::unique_ptr<Bindable>> binds;
	const IndexBuffer* m_indexBuffer = nullptr;

protected:
	// Store hwnd for showing shader error compilation.
	HWND m_hwnd;
};

