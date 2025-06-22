// 참고 자료 : https://rastertek.com/

#include "textureshader.h"

ComPtr<ID3D11VertexShader> TextureShader::m_vertexShader;
ComPtr<ID3D11PixelShader> TextureShader::m_pixelShader;
ComPtr<ID3D11InputLayout> TextureShader::m_layout;
ComPtr<ID3D11Buffer> TextureShader::m_matrixBuffer;
ComPtr<ID3D11Buffer> TextureShader::m_colorBuffer;
ComPtr<ID3D11SamplerState> TextureShader::m_sampleState;

bool TextureShader::Start() {
	HRESULT result;

	ComPtr<ID3D10Blob> vertexShaderBuffer;
	ComPtr<ID3D10Blob> pixelShaderBuffer;
	ComPtr<ID3D10Blob> errorMessage;

	result = D3DCompileFromFile(L"scripts\\shader\\texture.vs", NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
	if (FAILED(result)) {
		MessageBox(GetActiveWindow(), L"Failed to load vertex shader", L"Error", MB_OK);
		return false;
	}

	result = D3DCompileFromFile(L"scripts\\shader\\texture.ps", NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
	if (FAILED(result)) {
		MessageBox(GetActiveWindow(), L"Failed to load pixel shader", L"Error", MB_OK);
		return false;
	}

	result = Direct3D::GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, m_vertexShader.GetAddressOf());
	if (FAILED(result)) {
		MessageBox(GetActiveWindow(), L"Failed to create vertex shader", L"Error", MB_OK);
		return false;
	}

	result = Direct3D::GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, m_pixelShader.GetAddressOf());
	if (FAILED(result)) {
		MessageBox(GetActiveWindow(), L"Failed to create pixel shader", L"Error", MB_OK);
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	result = Direct3D::GetDevice()->CreateInputLayout(polygonLayout, sizeof(polygonLayout) / sizeof(polygonLayout[0]), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), m_layout.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	D3D11_BUFFER_DESC matrixBufferDesc;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = Direct3D::GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, m_matrixBuffer.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	D3D11_BUFFER_DESC colorBufferDesc;

	colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	colorBufferDesc.ByteWidth = sizeof(ColorBufferType);
	colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colorBufferDesc.MiscFlags = 0;
	colorBufferDesc.StructureByteStride = 0;

	result = Direct3D::GetDevice()->CreateBuffer(&colorBufferDesc, NULL, m_colorBuffer.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = Direct3D::GetDevice()->CreateSamplerState(&samplerDesc, m_sampleState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool TextureShader::Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ComPtr<ID3D11ShaderResourceView> texture, XMFLOAT4 additiveColor, XMFLOAT4 multiplicativeColor) {
	HRESULT result;

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* matrixPtr;
	ColorBufferType* colorPtr;

	result = Direct3D::GetDeviceContext()->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	matrixPtr = (MatrixBufferType*)mappedResource.pData;

	matrixPtr->world = worldMatrix;
	matrixPtr->view = viewMatrix;
	matrixPtr->projection = projectionMatrix;

	Direct3D::GetDeviceContext()->Unmap(m_matrixBuffer.Get(), 0);
	Direct3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, m_matrixBuffer.GetAddressOf());

	result = Direct3D::GetDeviceContext()->Map(m_colorBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	colorPtr = (ColorBufferType*)mappedResource.pData;

	colorPtr->additiveColor = additiveColor;
	colorPtr->multiplicateColor = multiplicativeColor;

	Direct3D::GetDeviceContext()->Unmap(m_colorBuffer.Get(), 0);
	Direct3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, m_colorBuffer.GetAddressOf());

	Direct3D::GetDeviceContext()->PSSetShaderResources(0, 1, texture.GetAddressOf());

	Direct3D::GetDeviceContext()->IASetInputLayout(m_layout.Get());

	Direct3D::GetDeviceContext()->VSSetShader(m_vertexShader.Get(), NULL, 0);
	Direct3D::GetDeviceContext()->PSSetShader(m_pixelShader.Get(), NULL, 0);

	Direct3D::GetDeviceContext()->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());
	Direct3D::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

	return true;
}
