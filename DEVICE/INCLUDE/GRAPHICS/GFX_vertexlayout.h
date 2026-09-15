#pragma once

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

// List definiton for X-Macro
#define LAYOUT_ELEMENT_TYPES \
	X( Position2D ) \
	X( Position3D ) \
	X( Texture2D ) \
	X( Normal ) \
	X( Tangent ) \
	X( Bitangent ) \
	X( Float3Color ) \
	X( Float4Color ) \
	X( Count )

class VertexLayout
{
public:
	// Expand list to generate declarations.
	enum ElementType
	{
		#define X(el) el, 
		LAYOUT_ELEMENT_TYPES
		#undef X
	};

	template<ElementType> struct Map;
	template<> struct Map<Position2D>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* semantic = "Position";
		static constexpr const char* code = "P2";
	};

	template<> struct Map<Position3D>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Position";
		static constexpr const char* code = "P3";
	};

	template<> struct Map<Texture2D>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* semantic = "Texcoord";
		static constexpr const char* code = "T2";
	};

	template<> struct Map<Normal>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Normal";
		static constexpr const char* code = "N";
	};

	template<> struct Map<Tangent>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Tangent";
		static constexpr const char* code = "Nt";
	};

	template<> struct Map<Bitangent>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Bitangent";
		static constexpr const char* code = "Nb";
	};

	template<> struct Map<Float3Color>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Color";
		static constexpr const char* code = "C3";
	};

	template<> struct Map<Float4Color>
	{
		using SysType = DirectX::XMFLOAT4;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		static constexpr const char* semantic = "Color";
		static constexpr const char* code = "C4";
	};

	template<> struct Map<Count>
	{
		using SysType = long double;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_UNKNOWN;
		static constexpr const char* semantic = "!INVALID!";
		static constexpr const char* code = "!INV!";
	};


	// No idea what this does.
	template<template<VertexLayout::ElementType> class F, typename... Args>
	static constexpr auto Bridge(VertexLayout::ElementType type, Args&&... args)
	{
		switch (type)
		{
			#define X(el) case VertexLayout::el: return F<VertexLayout::el>::Exec( std::forward<Args>( args )... );
			LAYOUT_ELEMENT_TYPES
			#undef X
		}
		assert("Invalid element type" && false);
		return F<VertexLayout::Count>::Exec(std::forward<Args>(args)...);
	}


	// Declare an element class for each element.
	class Element
	{
	public:
		Element(ElementType, size_t offset);
		size_t GetOffsetAfter() const;
		size_t GetOffset() const;
		size_t Size() const;

		static constexpr size_t SizeOf(ElementType type);
		ElementType GetType() const;

		D3D11_INPUT_ELEMENT_DESC GetDesc() const;
		const char* GetCode() const;
	
	private:
		ElementType type;
		size_t offset;
	};

	public:
		template<ElementType Type>

		const Element& Resolve() const
		{
			for (auto& e : elements)
			{
				if (e.GetType() == Type)
				{
					return e;
				}
			}
			assert("Could not resolve element type" && false);
			return elements.front();
		}

		const Element& ResolveByIndex(size_t i) const;
		VertexLayout& Append(ElementType type);
		size_t Size() const;
		size_t GetElementCount() const;
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const;
		std::string GetCode() const;
		bool Has(ElementType type) const;
	
private:
		std::vector<Element> elements;
};

