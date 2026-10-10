#pragma once

#include "GFX_constantbuffer.h"
#include "GFX_drawable.h"
#include "GFX_transformcbuf.h"
#include <DirectXMath.h>

class TransformCbufDouble : public TransformCbuf
{
public:
	void Bind(D3DClass*) override;

public:
	TransformCbufDouble(D3DClass*, const Drawable& parent, UINT slotVS = 0, UINT slotPS = 0);

protected:
	void UpdateBindImpl(D3DClass* d3d, const TransformsBuffer& tf);
private:
	static std::unique_ptr<PixelConstantBuffer<TransformsBuffer>> m_transformBufferPS;
};

