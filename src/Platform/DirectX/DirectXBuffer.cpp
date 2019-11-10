#include "Platform/DirectX/DirectXBuffer.h"
#include "Platform/DirectX/DirectXContext.h"
#include "Application.h"

namespace Framework
{
	void UpdateBufferResources(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> a_commandList, ID3D12Resource** a_pDestinationResource, 
		ID3D12Resource** a_pIntermediateResource, size_t a_numElements,
		size_t a_elementSize, const void* a_bufferData, D3D12_RESOURCE_FLAGS a_flags)
		{
			DirectXContext* context = (DirectXContext*)Application::Get().GetWindow()->GetGraphicsContext()->GetNativeContext();
			auto device = context->m_device;

			int bufferSize = a_numElements * a_elementSize;

			ThrowIfFailed(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(bufferSize, a_flags),
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(a_pDestinationResource)));

			if (a_bufferData)
			{
				ThrowIfFailed(device->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(a_pDestinationResource)));
			}

			D3D12_SUBRESOURCE_DATA subresourceData = {};
			subresourceData.pData = a_bufferData;
			subresourceData.RowPitch = bufferSize;
			subresourceData.SlicePitch = subresourceData.RowPitch;

			UpdateSubresources(a_commandList.Get(), *a_pDestinationResource,
				*a_pIntermediateResource, 0,
				0, 1, &subresourceData);
		}

	DriectXVertexBuffer::DriectXVertexBuffer(float* aVertices, uint32_t aSize)
	{
	}

	DriectXVertexBuffer::DriectXVertexBuffer(Vertex* aVertices, uint32_t aSize)
	{
		// Upload vertex buffer data.
		ComPtr<ID3D12Resource> intermediateVertexBuffer;
		auto context = std::dynamic_pointer_cast<DirectXContext>(Application::Get().GetWindow()->GetGraphicsContext());
		UpdateBufferResources(context->m_commandQueue->GetCommandList(), &m_vertexBuffer, &intermediateVertexBuffer,
			aSize / sizeof(Vertex), sizeof(Vertex), aVertices, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE);

		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.SizeInBytes = aSize;
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	}

	DriectXVertexBuffer::~DriectXVertexBuffer()
	{
	}

	void DriectXVertexBuffer::Bind() const
	{
	}

	void DriectXVertexBuffer::Unbind() const
	{
	}

	void DriectXVertexBuffer::SetData(float* a_data, const int a_start, const int& a_end)
	{
	}

	void DriectXVertexBuffer::GetData(float* a_data, const int& a_start, const int& a_end)
	{
	}

	void DriectXVertexBuffer::GetData(Vertex* a_data, const int& a_start, const int& a_end)
	{
	}

	int DriectXVertexBuffer::GetBufferSize()
	{
		return 0;
	}

	DirectXIndexBuffer::DirectXIndexBuffer(uint32_t* indices, uint32_t count)
	{
		// Upload vertex buffer data.
		ComPtr<ID3D12Resource> intermediateIndexBuffer;
		auto context = std::dynamic_pointer_cast<DirectXContext>(Application::Get().GetWindow()->GetGraphicsContext());
		UpdateBufferResources(context->m_commandQueue->GetCommandList(), &m_indexBuffer, &intermediateIndexBuffer,
			sizeof(indices) / sizeof(WORD), sizeof(WORD), indices, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE);

		m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		m_indexBufferView.SizeInBytes = sizeof(indices);
	}

	DirectXIndexBuffer::~DirectXIndexBuffer()
	{
	}

	void DirectXIndexBuffer::Bind() const
	{
	}

	void DirectXIndexBuffer::Unbind() const
	{
	}
}