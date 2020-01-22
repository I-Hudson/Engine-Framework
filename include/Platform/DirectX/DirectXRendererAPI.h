#pragma once

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "dxgi.lib")

// D3D12 extension library.
#include <Platform/DirectX/d3dx12.h>

// STL Headers
#include <algorithm>
#include <cassert>
#include <chrono>

//Helper functions
#include <Platform/DirectX/Helpers.h>

#include "Renderer/RendererAPI.h"

namespace Framework
{
	class DirectXRendererAPI : public Renderer::RendererAPI
	{
		virtual void SetClearColor(const glm::vec4& a_color) override;
		virtual void Clear() override;
		virtual void SetVSync(bool a_state) override;
		virtual void SetDepthTest(bool a_state) override;
		virtual void SetCullFace(bool a_state) override;

		virtual void SetContext(Framework::Renderer::GraphicsContext* context) override { }

		virtual void DrawIndexed(const std::shared_ptr<Renderer::VertexArray>& a_vertexArray) override;

		void ClearDepth(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList,
			D3D12_CPU_DESCRIPTOR_HANDLE dsv, FLOAT depth = 1.0f);

		float m_clearColor[4];
	};
};