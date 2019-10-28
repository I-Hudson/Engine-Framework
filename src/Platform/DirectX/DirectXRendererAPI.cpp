#include "Platform\DirectX\DirectXRendererAPI.h"
#include "Platform/DirectX/DirectXContext.h"
#include "Application.h"

namespace Framework
{
	void DirectXRendererAPI::SetClearColor(const glm::vec4& a_color)
	{
		m_clearColor[0] = a_color.x;
		m_clearColor[1] = a_color.y;
		m_clearColor[2] = a_color.z;
		m_clearColor[3] = a_color.w;
	}

	void DirectXRendererAPI::Clear()
	{
		auto context = (DirectXContext*)(Application::Get().GetWindow().GetContext());
		auto commandAllocator = context->m_commandAllocators[context->m_currentBackBufferIndex];
		auto backBuffer = context->m_backBuffers[context->m_currentBackBufferIndex];

		commandAllocator->Reset();
		context->m_commandList->Reset(commandAllocator.Get(), nullptr);

		//Clear the rneder target
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			context->m_commandList->ResourceBarrier(1, &barrier);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(context->m_rtvDescriptionHeap->GetCPUDescriptorHandleForHeapStart(),
				context->m_currentBackBufferIndex, context->m_rtvDescriptionSize);

			context->m_commandList->ClearRenderTargetView(rtv, m_clearColor, 0, nullptr);
		}

		//Present
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

			context->m_commandList->ResourceBarrier(1, &barrier);

			ThrowIfFailed(context->m_commandList->Close());

			ID3D12CommandList* const commandList[] =
			{
				context->m_commandList.Get()
			};
			context->m_commandQueue->ExecuteCommandLists(_countof(commandList), commandList);

			UINT syncInterval = context->m_vSync ? 1 : 0;
			UINT presentFlags = context->m_tearingSupport && !context->m_vSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
			ThrowIfFailed(context->m_swapChain->Present(syncInterval, presentFlags));

			context->m_frameFenceValues[context->m_currentBackBufferIndex] = context->Single(context->m_commandQueue, context->m_fence, context->m_fenceValue);

			context->m_currentBackBufferIndex = context->m_swapChain->GetCurrentBackBufferIndex();
			context->WaitForFenceValue(context->m_fence, context->m_frameFenceValues[context->m_currentBackBufferIndex], context->m_fenceEvent);
		}
	}

	void DirectXRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& a_vertexArray)
	{
	}
}