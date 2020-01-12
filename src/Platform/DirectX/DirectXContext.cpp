#include "Platform/DirectX/DirectXContext.h"
#include "RenderCommand.h"
#include "Application.h"

#include <string>
#include <sstream>
#include <iostream>

#include "Events/ApplicationEvent.h"

namespace Framework
{
	DirectXContext* DirectXContext::m_context = nullptr;

	std::wstring widen(const std::string& str)
	{
		std::wostringstream wstm;
		const std::ctype<wchar_t>& ctfacet =
			std::use_facet<std::ctype<wchar_t> >(wstm.getloc());
		for (size_t i = 0; i < str.size(); ++i)
			wstm << ctfacet.widen(str[i]);
		return wstm.str();
	}

	LRESULT CALLBACK WndProc(HWND a_hwnd, UINT a_message, WPARAM a_wParam, LPARAM a_lParam)
	{
		auto context = DirectXContext::m_context;

		if (context == nullptr)
		{
			return 0;
		}

		if (context->m_isInitialized)
		{
			switch (a_message)
			{
			case WM_PAINT:
				//RenderCommand::Clear();
				break;

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				WPARAM keyPressed = a_wParam;
				bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
				switch (a_wParam)
				{
				case 'V':
					context->m_vSync = !context->m_vSync;
					break;

				case '1':
					RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1.0f });
					break;
				case '2':
					RenderCommand::SetClearColor({ 0.0f, 1.0f, 0.0f, 1.0f });
					break; 
				case '3':
					RenderCommand::SetClearColor({ 0.0f, 0.0f, 1.0f, 1.0f });
					break; 
				case '4':
					RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
					break;

				case VK_ESCAPE:
					::PostQuitMessage(0);
					break;
				case VK_RETURN:
					if (alt)
					{
				case VK_F11:
					context->SetFullScreen(!context->m_fullScreen);
					break;
					}
					break;
				}
			}
				break;

			case WM_QUIT:
				break;
			
			// The default window procedure will play a system notification sound 
			// when pressing the Alt+Enter keyboard combination if this message is 
			// not handled.
			case WM_SYSCHAR:
				break;
			case WM_SIZE:
			{
				RECT clientRect = {};
				::GetClientRect(context->m_hwnd, &clientRect);

				int width = clientRect.right - clientRect.left;
				int height = clientRect.bottom - clientRect.top;

				context->Resize(width, height);
				break;
			}
			case WM_DESTROY:
				::PostQuitMessage(0);
				break;
			default:
				return ::DefWindowProcW(a_hwnd, a_message, a_wParam, a_lParam);
			}
		}
		else
		{
			return ::DefWindowProcW(a_hwnd, a_message, a_wParam, a_lParam);
		}
		return 0;
	}

	void DirectXContext::ParseCommandLineArguments()
	{
		int argc;
		wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

		for (size_t i = 0; i < argc; ++i)
		{
			if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0)
			{
				m_clientWidth = ::wcstol(argv[++i], nullptr, 10);
			}
			if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0)
			{
				m_clientHeight = ::wcstol(argv[++i], nullptr, 10);
			}
			if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0)
			{
				m_useWarp = true;
			}
		}

		//Frre memory allocated by CommandLineToArgvW
		::LocalFree(argv);
	}

	DirectXContext::DirectXContext(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, void* window)
	{
		Init(a_width, a_height, a_title, a_fullscreen, window, nullptr);
	}

	DirectXContext::~DirectXContext()
	{
	}

	void DirectXContext::Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData)
	{
		//m_hwnd = (HWND)a_window;
		m_context = this;

		m_scissorRect = CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX);
		m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(a_width), static_cast<float>(a_height));
		m_FoV = 45.0f;

		// Windows 10 Creators update adds Per Monitor V2 DPI awareness context.
		// Using this awareness context allows the client area of the window 
		// to achieve 100% scaling while still allowing non-client window content to 
		// be rendered in a DPI sensitive fashion.
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

		// Window class name. Used for registering / creating the window.
		const wchar_t* windowClassName = widen(a_title).c_str();
		ParseCommandLineArguments();

		EnableDebugLayer();

		m_tearingSupport = CheckTearingSupport();

		RegisterWindowClass(windowClassName);
		m_hwnd = CreateWindow(windowClassName, m_hwnd, L"Learning DirectX 12", m_clientWidth, m_clientHeight);

		//SetParent(m_hwnd, hwnd);
		//long style = GetWindowLong(m_hwnd, GWL_STYLE);
		//style &= ~WS_POPUP; // remove popup style
		//style |= WS_CHILDWINDOW; // add childwindow style
		//SetWindowLong(m_hwnd, GWL_STYLE, style);

		// Initialize the global window rect variable.
		::GetWindowRect(m_hwnd, &m_windowRect);

		ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetAdapter(m_useWarp);

		m_device = CreateDevice(dxgiAdapter4);

		m_commandQueue = std::make_unique<CommandQueue>(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
		
		m_swapChain = CreateSwapChain(m_hwnd, m_commandQueue->GetD3D12CommandQueue(), m_clientWidth, m_clientHeight, m_numFrames);

		m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

		m_rtvDescriptionHeap = CreateDescriptionHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, m_numFrames);
		m_rtvDescriptionSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		UpdateRenderTargetView(m_device, m_swapChain, m_rtvDescriptionHeap);

		m_isInitialized = true;

		ShowWindow(m_hwnd, SW_SHOW);
	}
	
	void DirectXContext::Destroy()
	{
		m_commandQueue->Flush();
		::CloseHandle(m_commandQueue->GetEventHandle());
	}

	void DirectXContext::SwapBuffers()
	{
		MSG msg = {};
		//while (msg.message != WM_QUIT)
		//{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		//}
		// Make sure the command queue has finished all commands before closing.
		//Flush(m_commandQueue, m_fence, m_fenceValue, m_fenceEvent);

		//::CloseHandle(m_fenceEvent);
	}

	void DirectXContext::EnableDebugLayer()
	{
#if defined(_DEBUG)
		// Always enable the debug layer before doing anything DX12 related
		// so all possible errors generated while creating DX12 objects
		// are caught by the debug layer.
		ComPtr<ID3D12Debug> debugInterface;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
		debugInterface->EnableDebugLayer();
#endif
	}

	void DirectXContext::RegisterWindowClass(const wchar_t* a_windowClassname)
	{
		WNDCLASSEXW windowClass = {};

		HINSTANCE hInstance = GetModuleHandle(nullptr);

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = &WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = hInstance;
		windowClass.hIcon = ::LoadIcon(hInstance, nullptr);
		windowClass.hCursor = ::LoadCursor(hInstance, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = a_windowClassname;
		windowClass.hIconSm = ::LoadIcon(hInstance, nullptr);

		static ATOM atom = ::RegisterClassExW(&windowClass);
		assert(atom > 0);
	}

	HWND DirectXContext::CreateWindow(const wchar_t* a_windowClassName, HWND a_hInst, const wchar_t* a_windowTitle, const uint32_t& a_width, const uint32_t& a_height)
	{
		int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

		RECT windowRect = { 0,0, static_cast<LONG>(a_width), static_cast<LONG>(a_height) };
		::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		int windowWidth = windowRect.right - windowRect.left;
		int windowHeight = windowRect.bottom - windowRect.top;
	
		int windowX = std::max<int>(0, (screenWidth - windowWidth) * 0.5f);
		int windowY = std::max<int>(0, (screenHeight - windowHeight) * 0.5f);

		HWND hwnd = ::CreateWindowExW(
			NULL,
			a_windowClassName,
			a_windowTitle,
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			GetModuleHandle(nullptr),
			nullptr
		);

		assert(hwnd && "Failed to create window");

		return hwnd;
	}

	ComPtr<IDXGIAdapter4> DirectXContext::GetAdapter(bool a_useWarp)
	{
		ComPtr<IDXGIFactory4> dxgiFactory;
		UINT createFactoryFlags = 0;
#if defined(_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

		ComPtr<IDXGIAdapter1> dxgiAdapter1;
		ComPtr<IDXGIAdapter4> dxgiAdapter4;
		if (a_useWarp)
		{
			ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
			ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
		}
		else
		{
			SIZE_T maxDedicatedVideoMemory = 0;
			for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
			{
				DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
				dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);
				// Check to see if the adapter can create a D3D12 device without actually 
				// creating it. The adapter with the largest dedicated video memory
				// is favored.
				if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
					SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
						D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), nullptr)) &&
					dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
				{
					maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
					ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
				}
			}
		}
		return dxgiAdapter4;
	}

	ComPtr<ID3D12Device2> DirectXContext::CreateDevice(ComPtr<IDXGIAdapter4> a_adapter)
	{
		ComPtr<ID3D12Device2> device;
		ThrowIfFailed(D3D12CreateDevice(a_adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));

		// Enable debug messages in debug mode.
#if defined(_DEBUG)
		ComPtr<ID3D12InfoQueue> infoQueue;
		if (SUCCEEDED(device.As(&infoQueue)))
		{
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

			// Suppress whole categories of messages
			//D3D12_MESSAGE_CATEGORY Categories[] = {};

			// Suppress messages based on their severity level
			D3D12_MESSAGE_SEVERITY severities[] =
			{
				D3D12_MESSAGE_SEVERITY_INFO
			};

			// Suppress individual messages by their ID
			D3D12_MESSAGE_ID denyIds[] =
			{
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
			};

			D3D12_INFO_QUEUE_FILTER NewFilter = {};
			//NewFilter.DenyList.NumCategories = _countof(Categories);
			//NewFilter.DenyList.pCategoryList = Categories;
			NewFilter.DenyList.NumSeverities = _countof(severities);
			NewFilter.DenyList.pSeverityList = severities;
			NewFilter.DenyList.NumIDs = _countof(denyIds);
			NewFilter.DenyList.pIDList = denyIds;

			ThrowIfFailed(infoQueue->PushStorageFilter(&NewFilter));
		}
#endif

		return device;
	}

	ComPtr<ID3D12CommandQueue> DirectXContext::CreateCommandQueue(ComPtr<ID3D12Device2> a_device, D3D12_COMMAND_LIST_TYPE a_type)
	{
		ComPtr<ID3D12CommandQueue> commandQueue;
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = a_type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		ThrowIfFailed(a_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));

		return commandQueue;
	}

	bool DirectXContext::CheckTearingSupport()
	{
		bool allowTearing = false;

		// Rather than create the DXGI 1.5 factory interface directly, we create the
		// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
		// graphics debugging tools which will not support the 1.5 factory interface 
		// until a future update.
		ComPtr<IDXGIAdapter4> factory4;
		if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
		{
			ComPtr<IDXGIFactory5> factory5;
			if (SUCCEEDED(factory4.As(&factory5)))
			{
				if (FAILED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))))
				{
					allowTearing = false;
				}
			}
		}
		return allowTearing == true;
	}

	ComPtr<IDXGISwapChain4> DirectXContext::CreateSwapChain(HWND a_hwnd, ComPtr<ID3D12CommandQueue> a_commandQueue, uint32_t a_width, uint32_t a_height, uint32_t a_bufferCount)
	{
		ComPtr<IDXGISwapChain4> swapChain4;
		ComPtr<IDXGIFactory4> factory4;
		UINT createFactoryFlags = 0;
#if defined(_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory4)));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = a_width;
		swapChainDesc.Height = a_height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = a_bufferCount;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		// It is recommended to always allow tearing if tearing support is available.
		swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		//Create swap chain
		ComPtr<IDXGISwapChain1> swapChain1;
		ThrowIfFailed(factory4->CreateSwapChainForHwnd(
			a_commandQueue.Get(),
			a_hwnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain1
		));

		// Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
		// will be handled manually.
		ThrowIfFailed(factory4->MakeWindowAssociation(a_hwnd, DXGI_MWA_NO_ALT_ENTER));

		ThrowIfFailed(swapChain1.As(&swapChain4));

		return swapChain4;
	}

	ComPtr<ID3D12DescriptorHeap> DirectXContext::CreateDescriptionHeap(ComPtr<ID3D12Device2> a_device, D3D12_DESCRIPTOR_HEAP_TYPE a_type, uint32_t a_numDescriptors)
	{
		ComPtr<ID3D12DescriptorHeap> desciprtionHeap;

		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = a_numDescriptors;
		desc.Type = a_type;

		ThrowIfFailed(a_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&desciprtionHeap)));
		return desciprtionHeap;
	}

	void DirectXContext::UpdateRenderTargetView(ComPtr<ID3D12Device2> a_device, ComPtr<IDXGISwapChain4> a_swapChain, ComPtr<ID3D12DescriptorHeap> a_descriptionHeap)
	{
		auto dtvDescriptorSize = a_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(a_descriptionHeap->GetCPUDescriptorHandleForHeapStart());
		for (int i = 0; i < m_numFrames; i++)
		{
			ComPtr<ID3D12Resource> backBuffer;
			ThrowIfFailed(a_swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

			a_device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
			m_backBuffers[i] = backBuffer;
			rtvHandle.Offset(m_rtvDescriptionSize);
		}


	}

	ComPtr<ID3D12Fence> DirectXContext::CreateFence(ComPtr<ID3D12Device2> a_device)
	{
		ComPtr<ID3D12Fence> fence;

		ThrowIfFailed(a_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

		return fence;
	}

	void DirectXContext::TransitionResource(ComPtr<ID3D12GraphicsCommandList2> a_commandList, ComPtr<ID3D12Resource> a_resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(a_resource.Get(), beforeState, afterState);

		a_commandList->ResourceBarrier(1, &barrier);
	}

	void DirectXContext::Resize(uint32_t a_width, uint32_t a_height)
	{
		if (m_clientWidth != a_width || m_clientHeight != a_height)
		{
			//Don't allow 0 size swap chain back buffers.
			m_clientWidth = std::max(1u, a_width);
			m_clientHeight = std::max(1u, a_height);

			// Flush the GPU queue to make sure the swap chain's back buffers
			// are not being referenced by an in-flight command list.
			m_commandQueue->Flush();

			//for (int i = 0; i < m_numFrames; ++i)
			//{
			//	// Any references to the back buffers must be released
			//	// before the swap chain can be resized.
			//	m_backBuffers[i].Reset();
			//	m_frameFenceValues[i] = m_frameFenceValues[m_currentBackBufferIndex];
			//}

			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			ThrowIfFailed(m_swapChain->GetDesc(&swapChainDesc));
			ThrowIfFailed(m_swapChain->ResizeBuffers(m_numFrames, m_clientWidth, m_clientHeight, 
				swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));
			m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
			UpdateRenderTargetView(m_device, m_swapChain, m_rtvDescriptionHeap);
		}
	}

	void DirectXContext::SetFullScreen(bool a_fullscreen)
	{
		if (m_fullScreen != a_fullscreen)
		{
			m_fullScreen = a_fullscreen;
			//Switching to fullscreen
			if (m_fullScreen)
			{
				// Store the current window dimensions so they can be restored 
				// when switching out of fullscreen state.
				::GetWindowRect(m_hwnd, &m_windowRect);

				// Set the window style to a borderless window so the client area fills
				// the entire screen.
				UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

				::SetWindowLongW(m_hwnd, GWL_STYLE, windowStyle);

				// Query the name of the nearest display device for the window.
				// This is required to set the fullscreen dimensions of the window
				// when using a multi-monitor setup.
				HMONITOR hMonitor = ::MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFOEX monitorInfo = {};
				monitorInfo.cbSize = sizeof(MONITORINFOEX);
				::GetMonitorInfo(hMonitor, &monitorInfo);

				::SetWindowPos(m_hwnd, HWND_TOP,
					monitorInfo.rcMonitor.left,
					monitorInfo.rcMonitor.top,
					monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
					monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
					SWP_FRAMECHANGED | SWP_NOACTIVATE);

				::ShowWindow(m_hwnd, SW_MAXIMIZE);
			}
			else
			{
				// Restore all the window decorators.
				::SetWindowLong(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

				::SetWindowPos(m_hwnd, HWND_NOTOPMOST,
					m_windowRect.left,
					m_windowRect.top,
					m_windowRect.right - m_windowRect.left,
					m_windowRect.bottom - m_windowRect.top,
					SWP_FRAMECHANGED | SWP_NOACTIVATE);

				::ShowWindow(m_hwnd, SW_NORMAL);
			}
		}
	}

	UINT DirectXContext::Present()
	{
		UINT syncInterval = m_vSync ? 1 : 0;
		UINT presentFlags = m_tearingSupport && !m_vSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
		ThrowIfFailed(m_swapChain->Present(syncInterval, presentFlags));
		m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

		return m_currentBackBufferIndex;
	}

	void DirectXContext::Flush()
	{
		m_commandQueue->Flush();
	}
}