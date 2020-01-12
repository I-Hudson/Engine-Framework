#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Framework
{
	//Define all data types
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	//Return the size of the data type
	static uint32_t ShaderDataTypeSize(ShaderDataType aType)
	{
		switch (aType)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		//EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	///
	// DEFINE A BUFFER ELEMENT.
	///
	struct BufferElement
	{
		//Name of the element. Position, Normal, Colour, UV
		std::string Name;
		//Type of data. Float, Float 2, Int
		ShaderDataType Type;
		//Size of the data.
		uint32_t Size;
		//Offset from the start of to layout
		uint32_t Offset;
		//If data normalized
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType aType, const std::string& aName, bool aNormalized = false)
			:Name(aName), Type(aType), Size(ShaderDataTypeSize(aType)), Offset(0), Normalized(aNormalized)
		{
		}

		//Return the size of data
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}
			//EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	///
	// DEFINE THE LAYOUT OF A BUFFER.
	///
	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& aElements)
			:mElements(aElements)
		{
			CalculateOffsetsAndStride();
		}

		//
		inline uint32_t GetStride() const { return mStride; }
		//Get all layout elements
		inline const std::vector<BufferElement>& GetElements() const { return mElements; }

		//Iterators for the elements vector
		std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
		std::vector<BufferElement>::iterator end() { return mElements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

	private:
		//Calculate the offset and stride for each buffer elements in mElements,
		// and the overall stride from one element in the buffer to the next 
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			mStride = 0;
			for (auto& element : mElements)
			{
				element.Offset = offset;
				offset += element.Size;
				mStride += element.Size;
			}
		}

		//All elements of this layout. E.X. [0] = Position, [1] = Normal
		std::vector<BufferElement> mElements;
		//The stride from one Buffer elements to the next in the buffer
		uint32_t mStride = 0;
	};

	struct Vertex
	{
		glm::vec4 Position;
		glm::vec4 Color;
		glm::vec4 Normal;
		glm::vec2 UV;


		Vertex()
		{
		}

		Vertex(const glm::vec4& a_position, const glm::vec4& a_color, const glm::vec4& a_normal, const glm::vec2& a_uv)
			: Position(a_position), Color(a_color), Normal(a_normal), UV(a_uv)
		{
		}

		Vertex(const float& a_positionX, const float& a_positionY, const float& a_positionZ, const float& a_positionW,
			const float& a_colorR, const float& a_colorG, const float& a_colorB, const float& a_colorA,
			const float& a_normalX, const float& a_normalY, const float& a_normalZ, const float& a_normalW,
			const float& a_uvX, const float& a_uxY)
			: Position(glm::vec4(a_positionX, a_positionY, a_positionZ, a_positionW)), Color(glm::vec4(a_colorR, a_colorG, a_colorB, a_colorA)),
			Normal(glm::vec4(a_normalX, a_normalY, a_normalZ, a_normalW)), UV(glm::vec2(a_uvX, a_uxY))
		{
		}
	};

	struct VertexDX
	{
		glm::vec3 Position;
		glm::vec3 Color;

		VertexDX(const glm::vec3& a_position, const glm::vec3& a_colour)
			: Position(a_position), Color(a_colour)
		{
		}

		VertexDX(const float& a_positionX, const float& a_positionY, const float& a_positionZ,
			const float& a_colorR, const float& a_colorG, const float& a_colorB)
			: Position(glm::vec3(a_positionX, a_positionY, a_positionZ)), Color(glm::vec3(a_colorR, a_colorG, a_colorB))
		{
		}
	};

	///
	// BASE VERTEX BUFFER. ALL VERTEX BUFFER CLASS MUST DERIVE FROM THIS.
	///
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		//Bind and unbind the buffer
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		//Set the internal layout of this buffer
		virtual void SetLayout(const BufferLayout& aLayout) = 0;
		//Get the layout of this buffer 
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void SetSubData(const Vertex* a_vertices, const unsigned int& a_bufferSize) {}

		virtual Vertex* GetData() const { return m_vertics; }
		virtual unsigned int GetBufferSize() { return m_bufferSize; }
		unsigned int GetCount() const { return m_count; }

		Vertex* GetVertex(const int& index);

		//Create this buffer
		static VertexBuffer* Create(float* aVertices, uint32_t aSize);
		static VertexBuffer* Create(Vertex* aVertices, uint32_t aSize);

	protected:
		unsigned int m_bufferSize;
		Vertex* m_vertics;
		unsigned int m_count;
	};

	///
	// BASE INDEX BUFFER. ALL INDEX BUFFER CLASS MUST DERIVE FROM THIS.
	///
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		//Bind and unbind the buffer
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetSubData(const unsigned int* a_indices, const unsigned int& a_count) {}

		virtual void* GetData() const { return m_indices; }
		//Get the number of indices this buffer holds
		virtual unsigned int GetCount() const { return m_count; }
		unsigned int GetIndice(const int& index);

		//Create this buffer
		static IndexBuffer* Create(unsigned int* aIndices, unsigned int aSize);

	protected:
		unsigned int m_count;
		unsigned int* m_indices;
	};
}