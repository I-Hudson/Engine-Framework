#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <shellapi.h>

// The min/max macros conflict with like-named member functions.
// Only use std::min and std::max defined in <algorithm>.
#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

// In order to define a function called CreateWindow, the Windows macro needs to
// be undefined.
#if defined(CreateWindow)
#undef CreateWindow
#endif

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

#include <initguid.h>

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

#include "Platform/DirectX/CommandQueue.h"
#include "GraphicsContext.h"

namespace Framework
{
	const uint8_t m_numFrames = 3;

	class DirectXContext : public GraphicsContext
	{
	public:
		DirectXContext(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen = false, void* window = nullptr);
		~DirectXContext();

		virtual void Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData) override;
		virtual void Destroy() override;

		virtual void SwapBuffers() override;

		virtual void* GetNativeContext() override { return this; }

	public:

		void ParseCommandLineArguments();
		void EnableDebugLayer();
		void RegisterWindowClass(const wchar_t* a_windowClassname);
		
		HWND CreateWindow(const wchar_t* a_windowClassName, HWND a_hInst,
			const wchar_t* a_windowTitle, const uint32_t& a_width, const uint32_t& a_height);
		
		ComPtr<IDXGIAdapter4> GetAdapter(bool a_useWarp);
		ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4> a_adapter);
		ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> a_device, D3D12_COMMAND_LIST_TYPE a_type);
		
		bool CheckTearingSupport();
		
		ComPtr<IDXGISwapChain4> CreateSwapChain(HWND a_hwnd, ComPtr <ID3D12CommandQueue> a_commandQueue, uint32_t a_width, uint32_t a_height, uint32_t a_bufferCount);
		ComPtr<ID3D12DescriptorHeap> CreateDescriptionHeap(ComPtr<ID3D12Device2> a_device,
			D3D12_DESCRIPTOR_HEAP_TYPE a_type, uint32_t a_numDescriptors);
		
		void UpdateRenderTargetView(ComPtr<ID3D12Device2> a_device, ComPtr<IDXGISwapChain4> a_swapChain, ComPtr<ID3D12DescriptorHeap> a_descriptionHeap);
		ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> a_device);

		void TransitionResource(ComPtr<ID3D12GraphicsCommandList2> a_commandList, ComPtr<ID3D12Resource> a_resource,
			D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCurrentRenderTargetView()
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvDescriptionHeap->GetCPUDescriptorHandleForHeapStart(),
				m_currentBackBufferIndex, m_rtvDescriptionSize);
		}

		void Resize(uint32_t a_width, uint32_t a_height);
		void SetFullScreen(bool a_fullscreen);

		UINT Present();
		void Flush();

		int GetClientWidth() { return m_clientWidth; }
		int GetClientHeight() { return m_clientHeight; }

		// The number of swap chain back buffers.
		//Use WRAP adapter
		bool m_useWarp = false;

		uint32_t m_clientWidth = 1280;
		uint32_t m_clientHeight = 720;

		// Set to true once the DX12 objects have been initialized.
		bool m_isInitialized = false;

		//Window handle
		HWND m_hwnd;
		//Window rect
		RECT m_windowRect;

		//DriectX 12 objects
		ComPtr<ID3D12Device2> m_device;

		std::unique_ptr<CommandQueue> m_commandQueue;

		ComPtr<IDXGISwapChain4> m_swapChain;
		ComPtr<ID3D12Resource> m_backBuffers[m_numFrames];
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		ComPtr<ID3D12DescriptorHeap> m_rtvDescriptionHeap;
		UINT m_rtvDescriptionSize;
		UINT m_currentBackBufferIndex;

		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_scissorRect;

		float m_FoV;

		DirectX::XMMATRIX m_ModelMatrix;
		DirectX::XMMATRIX m_ViewMatrix;
		DirectX::XMMATRIX m_ProjectionMatrix;

		bool m_ContentLoaded;

		uint64_t m_frameFenceValues[m_numFrames] = {};

		// By default, enable V-Sync.
		// Can be toggled with the V key.
		bool m_vSync = true;
		bool m_tearingSupport = false;
		// By default, use windowed mode.
		// Can be toggled with the Alt+Enter or F11
		bool m_fullScreen = false;

		static DirectXContext* m_context;
	};
}