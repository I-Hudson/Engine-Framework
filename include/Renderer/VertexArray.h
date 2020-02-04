#pragma once

#include <memory>
#include "Renderer/Buffer.h"

namespace Framework
{
	namespace Renderer
	{
		///
		//DEFINE THE BASE VERTEX ARRAY. ANY VERTEX ARRAY MUST BE DERVIED FROM THIS.
		///
		class VertexArray
		{
		public:
			virtual ~VertexArray() {}

			//Bind and unbind
			virtual void Bind(void* object = nullptr) const = 0;
			virtual void Unbind(void* object = nullptr) const = 0;

			virtual void SetSubVertexData(const Vertex* a_vertices, const unsigned int& a_bufferSize) {}
			virtual void SetSubIndexData(const unsigned int* a_indices, const unsigned int& a_count) {}

			//Add a vertex buffer and index buffer
			virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& aVertexBuffer) = 0;
			virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& aIndexBuffer) = 0;

			//Get the vertex buffers and index buffer
			virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
			virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

			//Create
			static VertexArray* Create();
		};
	}
}