#pragma once

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Buffer.h"

namespace Framework
{
	class DriectXVertexBuffer : VertexBuffer
	{
		DriectXVertexBuffer(float* aVertices, uint32_t aSize);
		DriectXVertexBuffer(Vertex* aVertices, uint32_t aSize);
		virtual ~DriectXVertexBuffer();

		//Bind and unbind
		virtual void Bind()  const override;
		virtual void Unbind() const override;

		//Set the layout for this buffer
		virtual void SetLayout(const BufferLayout& aLayout) override { mLayout = aLayout; }
		//Get the layout for this buffer
		virtual const BufferLayout& GetLayout() const override { return mLayout; }

		virtual void SetData(float* a_data, const int a_start, const int& a_end) override;
		virtual void GetData(float* a_data, const int& a_start = 0, const int& a_end = -1) override;
		virtual void GetData(Vertex* a_data, const int& a_start = 0, const int& a_end = -1) override;

		virtual int GetBufferSize() override;

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
		DirectXIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~DirectXIndexBuffer();

		//Bind and unbind
		virtual void Bind() const override;
		virtual void Unbind() const override;

		//Get the bumber of indices this buffer holds
		virtual uint32_t GetCount() const override { return m_count; }
	private:
		uint32_t m_ID;
		uint32_t m_count;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	};
}