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
}