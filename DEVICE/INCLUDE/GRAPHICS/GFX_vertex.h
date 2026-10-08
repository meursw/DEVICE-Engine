#pragma once

#include <vector>
#include <type_traits>
#include "GFX_d3dclass.h"

namespace DEVICE_VERTEX
{
	struct BGRAColor
	{
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	// Descrobes the structure of a single vertex.
	class VertexLayout
	{
	public:
		// ENUM OF ELEMENT TYPES
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count,
		};
		// Define a templated struct.
		template<ElementType> struct Map;
		template<> struct Map<Position2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "POSITION";
			static constexpr const char* code = "P2";
		};
		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "POSITION";
			static constexpr const char* code = "P3";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "TEXCOORD";
			static constexpr const char* code = "T2";
		};
		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "NORMAL";
			static constexpr const char* code = "N";
		};
		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "COLOR";
			static constexpr const char* code = "C3";
		};
		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "COLOR";
			static constexpr const char* code = "C4";
		};
		template<> struct Map<BGRAColor>
		{
			using SysType = DEVICE_VERTEX::BGRAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "COLOR";
			static constexpr const char* code = "C8";
		};

		// START OF ELEMENT CLASS
		class Element
		{
		public:
			Element(ElementType type, size_t offset)
				:
				m_type(type),
				m_offset(offset)
			{}

			// Get the offset into the vertex buffer after this element.
			size_t GetOffsetAfter() const
			{
				return m_offset + Size();
			}

			size_t GetOffset() const
			{
				return m_offset;
			}

			size_t Size() const
			{
				return SizeOf(m_type);
			}

			static constexpr size_t SizeOf(ElementType type)
			{
				using namespace DirectX;
				switch (type)
				{
				case Position2D:
					return sizeof(Map<Position2D>::SysType);

				case Position3D:
					return sizeof(Map<Position3D>::SysType);

				case Texture2D:
					return sizeof(Map<Texture2D>::SysType);

				case Normal:
					return sizeof(Map<Normal>::SysType);

				case Float3Color:
					return sizeof(Map<Float3Color>::SysType);

				case Float4Color:
					return sizeof(Map<Float4Color>::SysType);

				case BGRAColor:
					return sizeof(Map<BGRAColor>::SysType);
				}

				assert("Invalid element type" && false);
				return 0u;
			}

			ElementType GetType() const
			{
				return m_type;
			}

			D3D11_INPUT_ELEMENT_DESC GetDesc() const
			{
				switch (m_type)
				{
				case Position2D:
					return GenerateDesc<Position2D>(GetOffset());
				case Position3D:
					return GenerateDesc<Position3D>(GetOffset());
				case Texture2D:
					return GenerateDesc<Texture2D>(GetOffset());
				case Normal:
					return GenerateDesc<Normal>(GetOffset());
				case Float3Color:
					return GenerateDesc<Float3Color>(GetOffset());
				case Float4Color:
					return GenerateDesc<Float4Color>(GetOffset());
				case BGRAColor:
					return GenerateDesc<BGRAColor>(GetOffset());
				}
				assert("Invalid element type" && false);
				return { "INVALID",0,DXGI_FORMAT_UNKNOWN,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
			}

			const char* GetCode() const
			{
				switch (m_type)
				{
				case Position2D:
					return Map<Position2D>::code;
				case Position3D:
					return Map<Position3D>::code;
				case Texture2D:
					return Map<Texture2D>::code;
				case Normal:
					return Map<Normal>::code;
				case Float3Color:
					return Map<Float3Color>::code;
				case Float4Color:
					return Map<Float4Color>::code;
				case BGRAColor:
					return Map<BGRAColor>::code;
				}
				assert("Invalid element type" && false);
				return "Invalid";
			}

		private:
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(size_t offset)
			{
				return { Map<type>::semantic, 0, Map<type>::dxgiFormat, 0, (UINT)offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		
		private:
			ElementType m_type;
			size_t m_offset;
		};
		// END OF ELEMENT CLASS

		// START OF VERTEX LAYOUT METHODS
	public:
		template<ElementType Type>
		const Element& Resolve() const
		{
			for (auto& e : m_elements)
			{
				if (e.GetType() == Type)
					return e;
			}
			assert("Could not resolve element type" && false);
			return m_elements.front();
		}

		const Element& ResolveByIndex(size_t i) const
		{
			return m_elements[i];
		}

		VertexLayout& Append(ElementType type)
		{
			m_elements.emplace_back(type, Size());
			return *this;
		}

		size_t Size() const
		{
			return m_elements.empty() ? 0u : m_elements.back().GetOffsetAfter(); // Get the last element and get its size + offset.
		}

		size_t GetElementCount() const
		{
			return m_elements.size();
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
			layout.reserve(GetElementCount());
			for (const auto& e : m_elements)
			{
				layout.push_back(e.GetDesc());
			}

			return layout;
		}

		std::string GetCode() const
		{
			std::string code;
			for (const auto& e : m_elements)
			{
				code += e.GetCode();
			}
			return code;
		}


	private:
		std::vector<Element> m_elements;
	};

	// START OF VERTEX CLASS
	// This class exits as a view into a vertex that exists in the vertex buffer class. 
	// It doesn't contain any actual data.
	// 
	// This provides creation and a return to a vertex inside the buffer for usage.
	// The VertexBuffer looks into its layout and gets the size of a single vertex in bytes.
	// Then based on the vertex we are indexing and the size of it, it can retrieve the vertex.

	// Once we retrieve a vertex, we can access the elements of that vertex and change it.
	// This is done using the templated Attr method.

	// Example usage: vbuffer[2].Attr<Pos3D>() = {x,y,z};
	class Vertex
	{
		friend class VertexBuffer;

	public:
		template<VertexLayout::ElementType Type>
		// Return an attribute of the vertex.
		auto& Attr() noexcept
		{
			auto pAttribute = pData + m_layout.Resolve<Type>().GetOffset();
			// Look inside the map based on the Type and cast the type of data to pAttribute. Then return pAttribute.
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}

		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val)
		{
			const auto& element = m_layout.ResolveByIndex(i);
			auto pAttribute = pData + element.GetOffset();
			switch (element.GetType())
			{
			case VertexLayout::Position2D:
				SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Position3D:
				SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::BGRAColor:
				SetAttribute<VertexLayout::BGRAColor>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad element type" && false);
			}
		}

	protected:
		Vertex(char* pData, const VertexLayout& layout)
			:
			pData(pData),
			m_layout(layout)
		{
			assert(pData != nullptr);
		}

	private:
		// enables parameter pack setting of multiple parameters by element index
		template<typename First, typename ...Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}

		// helper to reduce code duplication in SetAttributeByIndex
		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val) noexcept
		{
			using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
			{
				assert("Parameter attribute type mismatch" && false);
			}
		}
	private:
		// A pointer to the data of a SINGLE vertex inside the VertexBuffer.
		char* pData = nullptr;
		// A reference to the layout of the VertexBuffer.
		const VertexLayout& m_layout;
	};

	// A const view into a vertex.
	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v)
			:
			m_vertex(v)
		{}

		template<VertexLayout::ElementType Type>
		const auto& Attr() const
		{
			return const_cast<Vertex&>(m_vertex).Attr<Type>();
		}

	private:
		Vertex m_vertex;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout)
			:
			m_layout(layout)
		{}

		const char* GetData() const
		{
			return m_buffer.data();
		}

		const VertexLayout& GetLayout() const
		{
			return m_layout;
		}

		// Get the amount of vertices.
		size_t Size() const
		{
			return m_buffer.size() / m_layout.Size();
		}

		// Get the size of the vertex buffer in bytes.
		size_t SizeBytes() const
		{
			return m_buffer.size();
		}

		template<typename ...Params>
		// Constructs a new vertex and places it back at the end of the buffer.
		// A vertex contains information about the data and how that data is structured.
		void EmplaceBack(Params&&... params)
		{
			assert(sizeof...(params) == m_layout.GetElementCount() && "Param count doesn't match number of vertex elements");
			m_buffer.resize(m_buffer.size() + m_layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		Vertex Back()
		{
			assert(m_buffer.size() != 0u);
			return Vertex{ m_buffer.data() + m_buffer.size() - m_layout.Size(), m_layout };
		}
	
		Vertex Front()
		{
			assert(m_buffer.size() != 0u);
			return Vertex{ m_buffer.data(), m_layout };
		}

		Vertex operator[](size_t i)
		{
			assert(i < Size());
			return Vertex{ m_buffer.data() + m_layout.Size() * i, m_layout };
		}

		ConstVertex Back() const
		{
			return const_cast<VertexBuffer*>(this)->Back();
		}

		ConstVertex Front() const
		{
			return const_cast<VertexBuffer*>(this)->Front();
		}

		ConstVertex operator[](size_t i) const
		{
			return const_cast<VertexBuffer&>(*this)[i];
		}

	private:
		// Store the bytes of the vertices.
		std::vector<char> m_buffer;
		// The dynamic vertex is the layout of the vertex buffer.
		// It describes the structure of the un-structured m_buffer.
		VertexLayout m_layout;
	};
}

/*
// Example Usage
void example_of_dynamic_vertex_system()
{

// FIRST EXAMPLE
// Create and set a vertex layout that describes the structure of a single vertex

	VertexLayout v1;
	v1.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Normal)

// Create a vertex buffer with this layout.

	VertexBuffer vb(std::move(v1));

// Add vertices to the vertex buffer based on the layout above.

	vb.EmplaceBack(DirectX::XMFLOAT3{ 1.0f,1.0f,1.0f }, DirectX::XMFLOAT3{ 2.0f,1.0f,4.0f });

// Access the first vertex -> access it's first element (pos3D).

	auto pos = vb[0].Attr<VertexLayout::Position3D>();


// SECOND EXAMPLE
// Create the vertex buffer with a layout.
	VertexBuffer vb(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Normal)
		.Append(VertexLayout::Texture2D)
	));

// Add the first vertex.
	vb.EmplaceBack(
		XMFLOAT3(0.0f,0.0f,0.0f),
		XMFLOAT3(0.0f,0.0f,0.0f),
		XMFLOAT2(0.0f,0.0f)
	);

// Add the second vertex.
	vb.EmplaceBack(
		XMFLOAT3(1.0f, 1.0f, 1.0f),
		XMFLOAT3(1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f, 1.0f)
	);

// Get the position of the first vertex.
	auto pos = vb[0].Attr<VertexLayout::Position3D>();

// Get the texture coordinates of the second vertex.
	auto tex = vb[1].Attr<VertexLayout::Texture2D>();

// Get the last vertex (vertex #2) and set it's position.z to 10.0f.
	vb.Back().Attr<VertexLayout::Position3D>().z = 10.0f;

// Get the position of the last vertex.
	pos = vb.Back().Attr<VertexLayout::Position3D>();

// CONST VERTEX EXAMPLE
// Get a constant reference to the vertex buffer.
	const auto& cvb = vb;
	pos = cvb[1].Attr<VertexLayout::Position3D>();

}
*/