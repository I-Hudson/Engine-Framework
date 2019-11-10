#include "Platform/DirectX/DirectXShader.h"

#include <array>
#include <fstream>

namespace Framework
{
	DirectXShader::DirectXShader(const std::string& a_shaderFile)
	{
		auto context = std::dynamic_pointer_cast<DirectXContext>(Application::Get().GetWindow()->GetGraphicsContext());
		auto device = context->m_device;

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DSVHeap)));

	}

	DirectXShader::DirectXShader(const std::string& a_name, const std::string& a_vertexSrc, const std::string& a_fragSrc)
	{
		auto context = std::dynamic_pointer_cast<DirectXContext>(Application::Get().GetWindow()->GetGraphicsContext());
		auto device = context->m_device;

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DSVHeap)));

		// Load the vertex shader.
		ComPtr<ID3DBlob> vertexShaderBlob;
		ThrowIfFailed(D3DReadFileToBlob(L"./shader/VertexShader.cso", &vertexShaderBlob));

		// Load the pixel shader.
		ComPtr<ID3DBlob> pixelShaderBlob;
		ThrowIfFailed(D3DReadFileToBlob(L"./shader/PixelShader.cso", &pixelShaderBlob));

		// Create the vertex input layout
		//D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//	{ "TEXCOORD1", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//};
		//
		//// Create a root signature.
		//D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
		//featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		//if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		//{
		//	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		//}
		//
		//// Allow input layout and deny unnecessary access to certain pipeline stages.
		//D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		//	D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		//	D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		//	D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		//	D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		//	D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
		//
		//// A single 32-bit constant root parameter that is used by the vertex shader.
		//CD3DX12_ROOT_PARAMETER1 rootParameters[1];
		//rootParameters[0].InitAsConstants(sizeof(DirectX::XMMATRIX) / 4, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
		//
		//CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
		//rootSignatureDescription.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);
		//
		//// Serialize the root signature.
		//ComPtr<ID3DBlob> rootSignatureBlob;
		//ComPtr<ID3DBlob> errorBlob;
		//ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDescription,
		//	featureData.HighestVersion, &rootSignatureBlob, &errorBlob));
		//// Create the root signature.
		//ThrowIfFailed(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
		//	rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
		//
		//struct PipelineStateStream
		//{
		//	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
		//	CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
		//	CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
		//	CD3DX12_PIPELINE_STATE_STREAM_VS VS;
		//	CD3DX12_PIPELINE_STATE_STREAM_PS PS;
		//	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
		//	CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
		//} pipelineStateStream;
		//
		//D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		//rtvFormats.NumRenderTargets = 1;
		//rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		//
		//pipelineStateStream.pRootSignature = m_rootSignature.Get();
		//pipelineStateStream.InputLayout = { inputLayout, _countof(inputLayout) };
		//pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		//pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(vertexShaderBlob.Get());
		//pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(pixelShaderBlob.Get());
		//pipelineStateStream.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		//pipelineStateStream.RTVFormats = rtvFormats;
		//
		//D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
		//	sizeof(PipelineStateStream), &pipelineStateStream
		//};
		//ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&m_pipelineState)));
		//
		//auto fenceValue = context->m_commandQueue->ExecuteCommandList(context->m_commandQueue->GetCommandList());
		//context->m_commandQueue->WaitForFenceValue(fenceValue);
	}

	DirectXShader::~DirectXShader()
	{
	}

	void DirectXShader::Bind() const
	{
	}

	void DirectXShader::Unbind() const
	{
	}

	const std::string& DirectXShader::GetName()
	{
		// TODO: insert return statement here
		return m_name;
	}

	void DirectXShader::UploadUniformBool(const std::string& a_name, const bool& a_value)
	{

	}
	void DirectXShader::UploadUniformInt(const std::string& a_name, const int& a_value)
	{
	}

	void DirectXShader::UploadUniformFloat(const std::string& a_name, const float& a_value)
	{
	}

	void DirectXShader::UploadUniformVec2(const std::string& a_name, const glm::vec2& a_value)
	{
	}

	void DirectXShader::UploadUniformVec3(const std::string& a_name, const glm::vec3& a_value)
	{
	}

	void DirectXShader::UploadUniformVec4(const std::string& a_name, const glm::vec4& a_value)
	{
	}

	void DirectXShader::UploadUniformMat2(const std::string& a_name, const glm::mat2& a_value)
	{
	}

	void DirectXShader::UploadUniformMat3(const std::string& a_name, const glm::mat3& a_value)
	{
	}

	void DirectXShader::UploadUniformMat4(const std::string& a_name, const glm::mat4& a_value)
	{
	}

	void DirectXShader::UploadTexture(const std::string& a_name, const std::shared_ptr<Texture> a_texture)
	{
	}

	void DirectXShader::Release()
	{
	}

	std::string DirectXShader::ReadFromFile(const std::string& a_filePath)
	{
		std::string result;
		std::ifstream in(a_filePath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> DirectXShader::PreProcess(const std::string& a_source)
	{
		return std::unordered_map<GLenum, std::string>();
	}

	void DirectXShader::Compile(const std::unordered_map<GLenum, std::string>& a_shaderSources)
	{
	}
}