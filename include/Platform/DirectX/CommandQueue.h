#pragma once

#include <d3d12.h>  // For ID3D12CommandQueue, ID3D12Device2, and ID3D12Fence
#include <wrl.h>    // For Microsoft::WRL::ComPtr

#include <cstdint>  // For uint64_t
#include <queue>    // For std::queue

namespace Framework
{
	class CommandQueue
	{
	public:
		CommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> a_device, D3D12_COMMAND_LIST_TYPE a_type);
		virtual ~CommandQueue();

		// Get an available command list from the command queue.
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> GetCommandList();

		// Execute a command list.
		// Returns the fence value to wait for for this command list.
		uint64_t ExecuteCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> a_commandList);

		uint64_t Signal();
		bool IsFenceComplete(uint64_t a_fenceValue);
		void WaitForFenceValue(uint64_t a_fenceValue);
		void Flush();
		HANDLE* GetEventHandle() { return &m_fenceEvent; }

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue() const;
	protected:

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> a_allocator);
	
	private:
		// Keep track of command allocators that are "in-flight"
		struct CommandAllocatorEntry
		{
			uint64_t fenceValue;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
		};

		using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
		using CommandListQueue = std::queue< Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> >;

		D3D12_COMMAND_LIST_TYPE                     m_commandListType;
		Microsoft::WRL::ComPtr<ID3D12Device2>       m_d3d12Device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>  m_d3d12CommandQueue;
		Microsoft::WRL::ComPtr<ID3D12Fence>         m_d3d12Fence;
		HANDLE                                      m_fenceEvent;
		uint64_t                                    m_fenceValue;

		CommandAllocatorQueue                       m_commandAllocatorQueue;
		CommandListQueue                            m_commandListQueue;
	};
}