#include "Platform/DirectX/CommandQueue.h"
#include <Platform\DirectX\Helpers.h>
#include <cassert>

Framework::CommandQueue::CommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> a_device, D3D12_COMMAND_LIST_TYPE a_type)
	: m_fenceValue(0), m_commandListType(a_type), m_d3d12Device(a_device)
{
	// Create command queue description
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = a_type;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	// Create a command queue and create a fence 
	ThrowIfFailed(a_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_d3d12CommandQueue)));
	ThrowIfFailed(m_d3d12Device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_d3d12Fence)));

	m_fenceEvent = ::CreateEvent(nullptr, false, false, nullptr);
	assert(m_fenceEvent && "Failed to create fence event handle.");
}

Framework::CommandQueue::~CommandQueue()
{
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> Framework::CommandQueue::GetCommandList()
{
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList;

	if (!m_commandAllocatorQueue.empty() && IsFenceComplete(m_commandAllocatorQueue.front().fenceValue))
	{
		commandAllocator = m_commandAllocatorQueue.front().commandAllocator;
		m_commandAllocatorQueue.pop();

		ThrowIfFailed(commandAllocator->Reset());
	}
	else
	{
		commandAllocator = CreateCommandAllocator();
	}

	if (!m_commandListQueue.empty())
	{
		commandList = m_commandListQueue.front();
		m_commandListQueue.pop();

		ThrowIfFailed(commandList->Reset(commandAllocator.Get(), nullptr));
	}
	else
	{
		commandList = CreateCommandList(commandAllocator);
	}

	// Associate the command allocator with the command list so that it can be
	// retrieved when the command list is executed.
	ThrowIfFailed(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));

	return commandList;
}

uint64_t Framework::CommandQueue::ExecuteCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> a_commandList)
{
	a_commandList->Close();

	ID3D12CommandAllocator* commandAllocator;
	unsigned int dataSize = sizeof(commandAllocator);
	ThrowIfFailed(a_commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));

	ID3D12CommandList* const ppCommandList[] =
	{
		a_commandList.Get()
	};

	m_d3d12CommandQueue->ExecuteCommandLists(1, ppCommandList);
	uint64_t fenceValue = Signal();

	m_commandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
	m_commandListQueue.push(a_commandList);

	// The ownership of the command allocator has been transferred to the ComPtr
	// in the command allocator queue. It is safe to release the reference 
	// in this temporary COM pointer here.
	commandAllocator->Release();

	return fenceValue;
}

uint64_t Framework::CommandQueue::Signal()
{
	uint64_t fenceValue = ++m_fenceValue;
	m_d3d12CommandQueue->Signal(m_d3d12Fence.Get(), fenceValue);
	return fenceValue;
}

bool Framework::CommandQueue::IsFenceComplete(uint64_t a_fenceValue)
{
	return m_d3d12Fence->GetCompletedValue() >= a_fenceValue;
}

void Framework::CommandQueue::WaitForFenceValue(uint64_t a_fenceValue)
{
	if (!IsFenceComplete(a_fenceValue))
	{
		m_d3d12Fence->SetEventOnCompletion(a_fenceValue, m_fenceEvent);
		::WaitForSingleObject(m_fenceEvent, DWORD_MAX);
	}
}

void Framework::CommandQueue::Flush()
{
	WaitForFenceValue(Signal());
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> Framework::CommandQueue::GetD3D12CommandQueue() const
{
	return m_d3d12CommandQueue;
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> Framework::CommandQueue::CreateCommandAllocator()
{
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	ThrowIfFailed(m_d3d12Device->CreateCommandAllocator(m_commandListType, IID_PPV_ARGS(&commandAllocator)));
	return commandAllocator;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> Framework::CommandQueue::CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> a_allocator)
{
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList;
	ThrowIfFailed(m_d3d12Device->CreateCommandList(0, m_commandListType, a_allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
	return commandList;
}
