#include "GFX_vertexlayout.h"

// VertexLayout

// Find element by index.
const VertexLayout::Element& VertexLayout::ResolveByIndex(size_t i) const
{
	return elements[i];
}

// Append element to the layout.
VertexLayout& VertexLayout::Append(ElementType type)
{
	if (!Has(type))
	{
		elements.emplace_back(type, Size());
	}
	return *this;
}

bool VertexLayout::Has(ElementType type) const
{
	for (auto& e : elements)
	{
		if (e.GetType() == type)
		{
			return true;
		}
	}
	return false;
}

size_t VertexLayout::Size() const
{
	return elements.empty() ? 0u : elements.back().GetOffsetAfter();
}

size_t VertexLayout::GetElementCount() const
{
	return elements.size();
}

// Create input element desc by combining all of the elements this vertex layout has.
std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	desc.reserve(GetElementCount());
	for (const auto& e : elements)
	{
		desc.push_back(e.GetDesc());
	}
	return desc;
}

// Get the code the vertex layout. The code is defined by the code of each element.
std::string VertexLayout::GetCode() const
{
	std::string code;
	for (const auto& e : elements)
	{
		code += e.GetCode();
	}
	return code;
}

// VertexLayout::Element

// Create an element.
VertexLayout::Element::Element(ElementType type, size_t offset) :
	type(type),
	offset(offset) {}

size_t VertexLayout::Element::GetOffsetAfter() const
{
	return offset + Size();
}

size_t VertexLayout::Element::GetOffset() const
{
	return offset;
}

size_t VertexLayout::Element::Size() const
{
	return SizeOf(type);
}

VertexLayout::ElementType VertexLayout::Element::GetType() const 
{
	return type;
}

template<VertexLayout::ElementType type>
struct SysSizeLookup
{
	static constexpr auto Exec() noexcept
	{
		return sizeof(VertexLayout::Map<type>::SysType);
	}
};

constexpr size_t VertexLayout::Element::SizeOf(ElementType type)
{
	return Bridge<SysSizeLookup>(type);
}

template<VertexLayout::ElementType type>
struct CodeLookup
{
	static constexpr auto Exec() 
	{
		return VertexLayout::Map<type>::code;
	}
};
const char* VertexLayout::Element::GetCode() const 
{
	return Bridge<CodeLookup>(type);
}

template<VertexLayout::ElementType type> struct DescGenerate {
	static constexpr D3D11_INPUT_ELEMENT_DESC Exec(size_t offset) noexcept {
		return {
			VertexLayout::Map<type>::semantic,0,
			VertexLayout::Map<type>::dxgiFormat,
			0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0
		};
	}
};
D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const
{
	return Bridge<DescGenerate>(type, GetOffset());
}