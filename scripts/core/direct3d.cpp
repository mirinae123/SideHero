// 참고 자료 : https://rastertek.com/

#include "direct3d.h"

bool Direct3D::m_vsync_enabled;
int Direct3D::m_videoCardMemory;
char Direct3D::m_videoCardDescription[128];

float Direct3D::m_screenWidth;
float Direct3D::m_screenHeight;

ComPtr<IDXGISwapChain> Direct3D::m_swapChain;
ComPtr<ID3D11Device> Direct3D::m_device;
ComPtr<ID3D11DeviceContext> Direct3D::m_deviceContext;
ComPtr<ID3D11RenderTargetView> Direct3D::m_renderTargetView;
ComPtr<ID3D11Texture2D> Direct3D::m_depthStencilBuffer;
ComPtr<ID3D11DepthStencilState> Direct3D::m_depthStencilState;
ComPtr<ID3D11DepthStencilView> Direct3D::m_depthStencilView;
ComPtr<ID3D11DepthStencilState> Direct3D::m_depthDisabledStencilState;
ComPtr<ID3D11RasterizerState> Direct3D::m_rasterState;

ComPtr<ID3D11BlendState> Direct3D::m_alphaEnableBlendingState;
ComPtr<ID3D11BlendState> Direct3D::m_alphaDisableBlendingState;

XMMATRIX Direct3D::m_projectionMatrix;
XMMATRIX Direct3D::m_worldMatrix;
XMMATRIX Direct3D::m_orthoMatrix;

D3D11_VIEWPORT Direct3D::m_viewport;

bool Direct3D::Start(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear) {
	HRESULT result;

	unsigned int numberOfModes, numerator, denominator;
	unsigned long long stringLength;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	float fieldOfView, screenAspect;

	m_vsync_enabled = vsync;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	ComPtr<IDXGIFactory> factory;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
	if (FAILED(result)) {
		return false;
	}

	ComPtr<IDXGIAdapter> adapter;
	result = factory->EnumAdapters(0, adapter.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	ComPtr<IDXGIOutput> adapterOutput;
	result = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numberOfModes, NULL);
	if (FAILED(result)) {
		return false;
	}

	
	vector<DXGI_MODE_DESC> displayModeList(numberOfModes);
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numberOfModes, displayModeList.data());
	if (FAILED(result)) {
		return false;
	}

	for (int i = 0; i < numberOfModes; i++) {
		if (displayModeList[i].Width == (unsigned int)screenWidth) {
			if (displayModeList[i].Height == (unsigned int)screenHeight) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) {
		return false;
	}

	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	result = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (result != 0) {
		return false;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	if (m_vsync_enabled) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	if (fullscreen) {
		swapChainDesc.Windowed = false;
	}
	else {
		swapChainDesc.Windowed = true;
	}

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, m_swapChain.GetAddressOf(), m_device.GetAddressOf(), NULL, m_deviceContext.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	ComPtr<ID3D11Texture2D> backBufferPtr;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBufferPtr.GetAddressOf()));
	if (FAILED(result)) {
		return false;
	}

	result = m_device->CreateRenderTargetView(backBufferPtr.Get(), NULL, m_renderTargetView.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result)) {
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result)) {
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, m_depthStencilView.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterDesc, m_rasterState.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	m_deviceContext->RSSetState(m_rasterState.Get());

	m_viewport.Width = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	m_deviceContext->RSSetViewports(1, &m_viewport);

	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	m_worldMatrix = XMMatrixIdentity();
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, m_depthDisabledStencilState.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = m_device->CreateBlendState(&blendStateDescription, m_alphaEnableBlendingState.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	result = m_device->CreateBlendState(&blendStateDescription, m_alphaDisableBlendingState.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	return true;
}

void Direct3D::BeginScene(float red, float green, float blue, float alpha) {
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void Direct3D::EndScene() {
	if (m_vsync_enabled)
	{
		m_swapChain->Present(1, 0);
	}
	else
	{
		m_swapChain->Present(0, 0);
	}

	return;
}

ComPtr<ID3D11Device> Direct3D::GetDevice() {
	return m_device;
}


ComPtr<ID3D11DeviceContext> Direct3D::GetDeviceContext() {
	return m_deviceContext;
}

float Direct3D::GetScreenWidth() {
	return m_screenWidth;
}

float Direct3D::GetScreenHeight() {
	return m_screenHeight;
}

XMMATRIX Direct3D::GetProjectionMatrix() {
	return m_projectionMatrix;
}


XMMATRIX Direct3D::GetWorldMatrix() {
	return m_worldMatrix;
}


XMMATRIX Direct3D::GetOrthoMatrix() {
	return m_orthoMatrix;
}

void Direct3D::SetBackBufferRenderTarget() {
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	return;
}


void Direct3D::ResetViewport() {
	m_deviceContext->RSSetViewports(1, &m_viewport);

	return;
}

void Direct3D::TurnZBufferOn() {
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
	return;

}

void Direct3D::TurnZBufferOff() {
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState.Get(), 1);
	return;
}

void Direct3D::ClearZBuffer() {
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EnableAlphaBlending() {
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState.Get(), blendFactor, 0xffffffff);
}

void Direct3D::DisableAlphaBlending() {
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState.Get(), blendFactor, 0xffffffff);
}
