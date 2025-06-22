// 참고 자료 : https://rastertek.com/

#pragma once

#include <d3d11.h>
#include <vector>
#include <string>
#include <wrl/client.h>
#include <stdio.h>

#include "../core/direct3d.h"

using namespace std;
using Microsoft::WRL::ComPtr;

/// <summary>
/// 텍스처 클래스
/// </summary>
class Texture {
private:
    struct TargaHeader {
        unsigned char data1[12];
        unsigned short width;
        unsigned short height;
        unsigned char bpp;
        unsigned char data2;
    };

public:
    Texture(string);

    ComPtr<ID3D11ShaderResourceView> GetTexture();

    int GetWidth();
    int GetHeight();

private:
    vector<unsigned char> m_targaData;

    ComPtr<ID3D11Texture2D> m_texture;
    ComPtr<ID3D11ShaderResourceView> m_textureView;
    int m_width, m_height;
};
