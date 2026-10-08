#pragma once
#include <directxmath.h>
#include <vector>

#include "GFX_bindable.h"
#include "GFX_vertex.h"

class VertexBuffer : public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	VertexBuffer(D3DClass* d3d, const DEVICE_VERTEX::VertexBuffer& vbuf);
	VertexBuffer(D3DClass* d3d, const std::string& tag, const DEVICE_VERTEX::VertexBuffer& vbuf);
	~VertexBuffer() = default;

	int GetVertexCount() const;

public:
	static std::shared_ptr<VertexBuffer> Resolve(D3DClass* d3d, const std::string& tag,
		const DEVICE_VERTEX::VertexBuffer& vbuf);
	
	template<typename ...Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
	{
		return GenerateUID_(tag);
	}

	std::string GetUID() const;

private:
	static std::string GenerateUID_(const std::string& tag);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

	UINT m_vertexCount;
	UINT m_stride;
	std::string m_tag;
};

