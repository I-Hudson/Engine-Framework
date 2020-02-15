#include "Platform\DirectX\DirectXRendererAPI.h"
#include "Platform/DirectX/DirectXContext.h"
#include "Application.h"

namespace Framework
{
	void DirectXRendererAPI::BeginRender(Renderer::GBuffer* gBuffer)
	{
	}
	void DirectXRendererAPI::EndRender(Renderer::GBuffer* gBuffer)
	{
	}
	void DirectXRendererAPI::SetClearColor(const glm::vec4& a_color)
	{
		m_clearColor[0] = a_color.x;
		m_clearColor[1] = a_color.y;
		m_clearColor[2] = a_color.z;
		m_clearColor[3] = a_color.w;
	}

	void DirectXRendererAPI::Clear()
	{
		auto context = (DirectXContext*)(Application::Get().GetWindow()->GetGraphicsContext());
		auto commandQueue = context->m_commandQueue.get();
		auto commandList = commandQueue->GetCommandList();

		auto currentBackbufferIndex = context->m_currentBackBufferIndex;
		auto backBuffer = context->m_backBuffers[currentBackbufferIndex];

		//Clear the rneder target
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			commandList->ResourceBarrier(1, &barrier);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(context->m_rtvDescriptionHeap->GetCPUDescriptorHandleForHeapStart(),
				context->m_currentBackBufferIndex, context->m_rtvDescriptionSize);

			commandList->ClearRenderTargetView(rtv, m_clearColor, 0, nullptr);
		}

		//Present
		{
			//CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(),
			//	D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			//
			//commandList->ResourceBarrier(1, &barrier);
			//
			//context->m_frameFenceValues[currentBackbufferIndex] = commandQueue->ExecuteCommandList(commandList);
			//
			//currentBackbufferIndex = context->Present();
			//
			//commandQueue->WaitForFenceValue(context->m_frameFenceValues[currentBackbufferIndex]);
		}
	}

	void DirectXRendererAPI::SetVSync(bool a_state)
	{
	}

	void DirectXRendererAPI::SetDepthTest(bool a_state)
	{
	}

	void DirectXRendererAPI::SetCullFace(bool a_state)
	{
	}

	void DirectXRendererAPI::DrawIndexed(const Renderer::VertexArray* a_vertexArray, Renderer::GBuffer* gBuffer)
	{
	}

	void DirectXRendererAPI::ClearDepth(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, FLOAT depth)
	{
	}
}