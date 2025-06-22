// 참고 자료 : https://rastertek.com/

#include "texture.h"

Texture::Texture(string fileName) {
	FILE* filePtr;

	fopen_s(&filePtr, fileName.c_str(), "rb");

	TargaHeader targaFileHeader;
	unsigned int count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);

	m_height = (int)targaFileHeader.height;
	m_width = (int)targaFileHeader.width;

	int bpp = (int)targaFileHeader.bpp;

	if (bpp != 32) {
		return;
	}

	int imageSize = m_width * m_height * 4;

	vector<unsigned char> targaImage(imageSize);

	count = (unsigned int)fread(targaImage.data(), 1, imageSize, filePtr);

	fclose(filePtr);

	m_targaData = vector<unsigned char>(imageSize);

	int index = 0;
	int k = (m_width * m_height * 4) - (m_width * 4);

	for (int j = 0; j < m_height; j++) {
		for (int i = 0; i < m_width; i++) {
			m_targaData[index + 0] = targaImage[k + 2];
			m_targaData[index + 1] = targaImage[k + 1];
			m_targaData[index + 2] = targaImage[k + 0];
			m_targaData[index + 3] = targaImage[k + 3];

			k += 4;
			index += 4;
		}

		k -= (m_width * 8);
	}

	D3D11_TEXTURE2D_DESC textureDesc;

	textureDesc.Height = m_height;
	textureDesc.Width = m_width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	Direct3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, m_texture.GetAddressOf());

	unsigned int rowPitch = (m_width * 4) * sizeof(unsigned char);

	Direct3D::GetDeviceContext()->UpdateSubresource(m_texture.Get(), 0, NULL, m_targaData.data(), rowPitch, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	Direct3D::GetDevice()->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_textureView.GetAddressOf());

	Direct3D::GetDeviceContext()->GenerateMips(m_textureView.Get());
}

ComPtr<ID3D11ShaderResourceView> Texture::GetTexture() {
	return m_textureView;
}

int Texture::GetWidth() {
	return m_width;
}

int Texture::GetHeight() {
	return m_height;
}
