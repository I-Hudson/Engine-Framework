#include "Platform/DirectX/DirectXContext.h"

namespace Framework
{
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
	{
		return TRUE;
	}

	void DirectXContext::ParseCommandLineArgumnets()
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

	DirectXContext::DirectXContext(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen)
	{
		Init(a_width, a_height, a_title, a_fullscreen);
	}

	DirectXContext::~DirectXContext()
	{
	}

	void DirectXContext::Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen)
	{
		GetAdapter(m_useWarp);
	}

	void DirectXContext::SwapBuffers()
	{
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
	void DirectXContext::RegisterWindowClass(HINSTANCE a_hInst, const wchar_t* a_windowClassname)
	{
		WNDCLASSEXW windowClass = {};

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = &WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = a_hInst;
		windowClass.hIcon = ::LoadIcon(a_hInst, nullptr);
		windowClass.hCursor = ::LoadCursor(a_hInst, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = a_windowClassname;
		windowClass.hIconSm = ::LoadIcon(a_hInst, nullptr);

		static ATOM atom = ::RegisterClassExW(&windowClass);
		assert(atom > 0);
	}
	HWND DirectXContext::CreateWindow(const wchar_t* a_windowClassName, HINSTANCE a_hInst, const wchar_t* a_windowTitle, const uint32_t& a_width, const uint32_t& a_height)
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
			a_hInst,
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
						D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
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
		ThrowIfFailed(D3D12CreateDevice(a_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&a_adapter)));

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
}