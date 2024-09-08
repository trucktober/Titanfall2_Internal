#pragma once
#include "Includes.h"

struct BACKUP_DX11_STATE
{
	UINT                        ViewportsCount;
	D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	//ID3D11RasterizerState* RS;
	//ID3D11BlendState* BlendState;
	//FLOAT                       BlendFactor[4];
	//UINT                        SampleMask;
	//UINT                        StencilRef;
	//ID3D11DepthStencilState* DepthStencilState;
	//ID3D11ShaderResourceView* PSShaderResource;
	//ID3D11SamplerState* PSSampler;
	ID3D11PixelShader* PS;
	ID3D11VertexShader* VS;
	ID3D11GeometryShader* GS;
	UINT                        PSInstancesCount, VSInstancesCount;// , GSInstancesCount;
	ID3D11ClassInstance* PSInstances[256], * VSInstances[256];// , * GSInstances[256];   // 256 is max according to PSSetShader documentation
	D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
	ID3D11Buffer* IndexBuffer, * VertexBuffer, * VSConstantBuffer;
	UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
	DXGI_FORMAT                 IndexBufferFormat;
	ID3D11InputLayout* InputLayout;
};


class Device {
private:
	static Device* instance;
	HWND hWnd = NULL;
	ID3D11Device* d3ddev = NULL;

	ID3D11DeviceContext* d3dcon = NULL;
	ID3D11RenderTargetView* d3dRTV = NULL;
	ID3D11Texture2D* backBuffer = NULL;
	ID3D11Buffer* vBuf = NULL;
	ID3D11Buffer* iBuf = NULL;
	ID3D11Buffer* cBuf = NULL;
	ID3D11InputLayout* d3dIL = NULL;
	ID3D11VertexShader* vShader = NULL;
	ID3D11PixelShader* pShader = NULL;
	D3D11_VIEWPORT d3dVP;
	DirectX::XMMATRIX ortho;
	ErrorLogger* logger = ErrorLogger::getinst();
	BACKUP_DX11_STATE old = {};
	
public:
	
	bool d3dinti = false;
	IDXGISwapChain* sc = NULL;
	Device(Device&) = delete;
	Device& operator=(const Device&) = delete;
	static Device* getinst() {
		if (instance == nullptr) instance = new Device();
		return instance;
	}

	Device();
	bool CreateDevice(void** pTable);
	bool CreateRenderTarget(IDXGISwapChain* sc);
	bool CreateInputLayout();
	bool CreateViewport();
	bool CreateOrtho();
	bool DrawLine();
	bool GetD3D11(IDXGISwapChain* sc);
	void Disable();
	bool Shutdown();
	bool Startup();
	void ClearScreen();
	void DrawTriangle();
	//bool imguitest(IDXGISwapChain* sc);

};

#ifndef D3DCOLORVALUE_DEFINED
typedef struct _D3DCOLORVALUE {
	float r;
	float g;
	float b;
	float a;
} D3DCOLORVALUE;

#define D3DCOLORVALUE_DEFINED
#endif

const D3DCOLORVALUE Red = { 1.f, 0.f, 0.f, 1.f };
const D3DCOLORVALUE Green = { 0.f, 1.f, 0.f, 1.f };
const D3DCOLORVALUE Blue = { 0.f, 0.f, 1.f, 1.f };
const D3DCOLORVALUE White = { 1.f, 1.f, 1.f, 1.f };
const D3DCOLORVALUE Black = { 0.f, 0.f, 0.f, 1.f };
const D3DCOLORVALUE Yellow = { 1.f, 1.f, 0.f, 1.f };
const D3DCOLORVALUE Cyan = { 0.f, 1.f, 1.f, 1.f };
const D3DCOLORVALUE Magenta = { 1.f, 0.f, 1.f, 1.f };

struct Shader {
	DirectX::XMFLOAT3 pos;
	D3DCOLORVALUE color;
};

struct ConstantBuffer
{
	DirectX::XMMATRIX mProjection;
};
