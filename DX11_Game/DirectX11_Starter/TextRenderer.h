#ifndef _TEXT_RENDERER_H
#define _TEXT_RENDERER_H

#define SHADER_PATH L"../Resources/Shaders/";
#define TEXTURE_PATH L"../Resources/Textures/";

#include "Utilities.h"
#include "Vertex.h"
#include "Camera.h"
#include "Constants.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <WICTextureLoader.h>
#include <d3dcompiler.h>

class TextRenderer {
public:
	static void Setup();
	static void Cleanup();

	// Draws a string on screen
	// Text to draw, x/y pixel coordinate, font size, color
	static void DrawString(char* text, float x, float y, float size = 24, XMFLOAT4 color = XMFLOAT4(1,1,1,1));

private:
	static ID3D11ShaderResourceView*	fontTexture;
	static ID3D11SamplerState*			fontTextureSampler;
	static ID3D11Buffer*				fontVBuffer;
	static ID3D11VertexShader*			fontVShader;
	static ID3D11InputLayout*			fontInputLayout;
	static ID3D11PixelShader*			fontPShader;
	static ID3D11Buffer*				fontCBuffer;
	static int							maxStringLength;

	static ID3D11BlendState* enableBlendingState;
	static ID3D11BlendState* disableBlendingState;

	static ID3D11DepthStencilState* disableStencil;
	static ID3D11DepthStencilState* enableStencil;
};

#endif // _TEXT_RENDERER_H