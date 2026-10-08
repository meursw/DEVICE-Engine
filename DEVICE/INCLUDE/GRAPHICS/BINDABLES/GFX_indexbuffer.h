#pragma once
#include "GFX_bindable.h"
#include <vector>

class IndexBuffer : public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	IndexBuffer(D3DClass* d3d, const std::vector<unsigned short>&);
	IndexBuffer(D3DClass* d3d, std::string tag, const std::vector<unsigned short>&);
	~IndexBuffer() = default;

	UINT GetIndexCount() const;

public:
	static std::shared_ptr<IndexBuffer> Resolve(D3DClass* d3d, const std::string&, const std::vector<unsigned short>&);
	template<typename...Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
	{
		return GenerateUID_(tag);
	}
	std::string GetUID() const override;

private:
	static std::string GenerateUID_(const std::string& tag);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indexCount;
	// Used for the UID in the BindableCodex
	std::string m_tag;
};

