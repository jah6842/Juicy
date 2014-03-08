#ifndef _TEXT_RENDERER_H
#define _TEXT_RENDERER_H

#define SHADER_PATH L"../Resources/Shaders/";
#define TEXTURE_PATH L"../Resources/Textures/";

#include "Utilities.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <WICTextureLoader.h>
#include <d3dcompiler.h>

class TextRenderer {
public:
	static void Setup();
	static void Cleanup();

private:
	static ID3D11ShaderResourceView*	fontTexture;
	static ID3D11SamplerState*			fontTextureSampler;
	static ID3D11Buffer*				fontVBuffer;
	static ID3D11VertexShader*			fontVShader;
	static ID3D11PixelShader*			fontPShader;
};

#endif // _TEXT_RENDERER_H