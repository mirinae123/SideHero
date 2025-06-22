// 참고 자료 : https://rastertek.com/

#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
#include <wrl/client.h>
#include <vector>

using namespace DirectX;
using namespace std;
using Microsoft::WRL::ComPtr;

/// <summary>
/// DirectX 11 클래스
/// </summary>
class Direct3D
{
public:
    static bool Start(int, int, bool, HWND, bool, float, float);

    static void BeginScene(float, float, float, float);
    static void EndScene();

    static ComPtr<ID3D11Device> GetDevice();
    static ComPtr<ID3D11DeviceContext> GetDeviceContext();

    static float GetScreenWidth();
    static float GetScreenHeight();

    static XMMATRIX GetProjectionMatrix();
    static XMMATRIX GetWorldMatrix();
    static  XMMATRIX GetOrthoMatrix();

    static void SetBackBufferRenderTarget();
    static void ResetViewport();

    static void TurnZBufferOn();
    static void TurnZBufferOff();
    static void ClearZBuffer();

    static void EnableAlphaBlending();
    static void DisableAlphaBlending();

private:
    static bool m_vsync_enabled;
    static int m_videoCardMemory;
    static char m_videoCardDescription[128];

    static float m_screenWidth;
    static float m_screenHeight;

    static ComPtr<IDXGISwapChain> m_swapChain;
    static ComPtr<ID3D11Device> m_device;
    static ComPtr<ID3D11DeviceContext> m_deviceContext;
    static ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    static ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    static ComPtr<ID3D11DepthStencilState> m_depthStencilState;
    static ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    static ComPtr<ID3D11DepthStencilState> m_depthDisabledStencilState;
    static ComPtr<ID3D11RasterizerState> m_rasterState;

    static ComPtr<ID3D11BlendState> m_alphaEnableBlendingState;
    static ComPtr<ID3D11BlendState> m_alphaDisableBlendingState;

    static XMMATRIX m_projectionMatrix;
    static XMMATRIX m_worldMatrix;
    static XMMATRIX m_orthoMatrix;

    static D3D11_VIEWPORT m_viewport;
};

inline XMFLOAT3 operator+(XMFLOAT3 a, XMFLOAT3 b) {
    XMFLOAT3 ret{ 0.0f, 0.0f, 0.0f };

    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;

    return ret;
}