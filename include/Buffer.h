#pragma once

#include <string>
#include <cstdint>
#include <vector>

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

		virtual void SetData(float* a_data, const int a_start, const int& a_end) = 0;
		virtual void GetData(float* a_data, const int& a_start = 0, const int& a_end = -1) = 0;
		virtual int GetBufferSize() = 0;

		//Create this buffer
		static VertexBuffer* Create(float* aVertices, uint32_t aSize);
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

		//Get the number of indices this buffer holds
		virtual uint32_t GetCount() const = 0;

		//Create this buffer
		static IndexBuffer* Create(uint32_t* aIndices, uint32_t aSize);
	};
}