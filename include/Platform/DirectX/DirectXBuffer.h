#pragma once

#include "Buffer.h"

#include "Platform/DirectX/DirectXContext.h"
#include "Application.h"

namespace Framework
{

	class DirectXVertexBuffer : public VertexBuffer
	{
	public:
		DirectXVertexBuffer(float* aVertices, uint32_t aSize);
		DirectXVertexBuffer(VertexDX* aVertices, uint32_t aSize);
		virtual ~DirectXVertexBuffer();

		//Bind and unbind
		virtual void Bind()  const override;
		virtual void Unbind() const override;

		//Set the layout for this buffer
		virtual void SetLayout(const BufferLayout& aLayout) override { mLayout = aLayout; }
		//Get the layout for this buffer
		virtual const BufferLayout& GetLayout() const override { return mLayout; }

		D3D12_VERTEX_BUFFER_VIEW* GetView() { return &m_vertexBufferView; }

	private:
		uint32_t m_ID;
		BufferLayout mLayout;

		int m_numOfElements;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	};

	class DirectXIndexBuffer : public IndexBuffer
	{
	public:
		DirectXIndexBuffer(WORD* indices, WORD count);
		virtual ~DirectXIndexBuffer();

		//Bind and unbind
		virtual void Bind() const override;
		virtual void Unbind() const override;

		D3D12_INDEX_BUFFER_VIEW* GetView() { return &m_indexBufferView; }
	private:
		uint32_t m_ID;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	};
}