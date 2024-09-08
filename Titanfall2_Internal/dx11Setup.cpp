#pragma once
#include "Includes.h"

Device::Device() {
	logger->PrintError(L"Device Initialized");
}

void Device::Disable() {
	d3ddev->Release();
	d3dcon->Release();
	d3dIL->Release();
	backBuffer->Release();
	d3dRTV->Release();
	vBuf->Release();
	pShader->Release();
	vShader->Release();
}

bool Device::CreateDevice(void** pTable) {
	IDXGISwapChain* sc;
	ID3D11Device* dev;
	const D3D_FEATURE_LEVEL fl[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	HWND hWnd = ::CreateWindowA("STATIC", "FORTNITE", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.OutputWindow = hWnd;
	scd.BufferCount = 1;
	scd.Windowed = TRUE;
	scd.SampleDesc.Count = 1;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, fl, 2, D3D11_SDK_VERSION, &scd, &sc, &dev, nullptr, nullptr) != S_OK) {
		scd.Windowed = FALSE;
		if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, fl, 2, D3D11_SDK_VERSION, &scd, &sc, &dev, nullptr, nullptr) != S_OK) {		
			return false;
		}
	}
	void** vTable = *reinterpret_cast<void***>(sc);
	pTable[0] = vTable[8];
	dev->Release();
	sc->Release();

	return true;
}

bool Device::CreateRenderTarget(IDXGISwapChain* sc) {
	ID3D11Texture2D* pbackBuffer = nullptr;
	sc->GetDevice(__uuidof(ID3D11Device), (void**)&d3ddev);
	d3ddev->GetImmediateContext(&d3dcon);

	//TESTING
	DXGI_SWAP_CHAIN_DESC sd;
	sc->GetDesc(&sd);
	sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pbackBuffer);
	d3ddev->CreateRenderTargetView(pbackBuffer, nullptr, &d3dRTV);
	pbackBuffer->Release();
	
	//d3dcon->OMGetRenderTargets(1, &d3dRTV, nullptr);

	d3dcon->OMSetRenderTargets(1, &d3dRTV, nullptr);
	return true;
}


bool Device::CreateInputLayout() {
	ID3DBlob* blob = nullptr;
	ID3DBlob* errorMsg = nullptr;
	HRESULT hr = NULL; 

	hr = D3DCompile(shader, strlen(shader), NULL, nullptr, nullptr, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &blob, &errorMsg);
	if (FAILED(hr)) {
		logger->PrintError(L"VS_Shader Compile Failed");
		logger->LogError((wchar_t*)errorMsg->GetBufferPointer());
		errorMsg->Release();
	}

	D3D11_INPUT_ELEMENT_DESC ied[2] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	d3ddev->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vShader);
	d3ddev->CreateInputLayout(ied, 2, blob->GetBufferPointer(), blob->GetBufferSize(), &d3dIL);
	if (blob) blob->Release();

	
	D3DCompile(shader, strlen(shader), NULL, nullptr, nullptr, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &blob, &errorMsg);
	if (FAILED(hr)) {
		logger->PrintError(L"PS_Shader Compile Failed");
		logger->LogError((wchar_t*)errorMsg->GetBufferPointer());
		errorMsg->Release();
	}

	d3ddev->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pShader);
	if (blob) blob->Release();
	d3dcon->IASetInputLayout(d3dIL);
	return true;
}

bool Device::CreateViewport() {
	D3D11_RECT rect;
	UINT numVP = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	D3D11_VIEWPORT tempVP[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	d3dcon->RSGetViewports(&numVP, tempVP);
	if (!numVP || tempVP->Width == 0) {
		logger->PrintError(L"GetViewport Creation Failed... Creating Viewport");
		GetClientRect(GetForegroundWindow(), &rect);
		d3dVP.TopLeftX = rect.left;
		d3dVP.TopLeftY = rect.top;
		d3dVP.Width = rect.right;
		d3dVP.Height = rect.bottom;
		d3dVP.MinDepth = 0.f;
		d3dVP.MaxDepth = 1.f;

		d3dcon->RSSetViewports(1, &d3dVP);
		return true;
	}
	d3dVP = tempVP[0];
	d3dcon->RSSetViewports(1, &d3dVP);
	return true;
}

bool Device::CreateOrtho() {

	ortho = DirectX::XMMatrixOrthographicOffCenterLH(0.f, d3dVP.Width, d3dVP.Height, 0.f, 0.f, 1.f);
	ConstantBuffer cb;
	cb.mProjection = ortho;
	
	D3D11_BUFFER_DESC bd{ 0 };
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &cb;

	d3ddev->CreateBuffer(&bd, &srd, &cBuf);
	d3dcon->VSSetConstantBuffers(0, 1, &cBuf);
	return true;
}

bool Device::GetD3D11(IDXGISwapChain* sc) {
	//if (!CreateDevice(pTable)) return false;
	if (!CreateRenderTarget(sc)) return false;
	if (!CreateInputLayout()) return false;
	if (!CreateViewport()) return false;
	if (!CreateOrtho()) return false;
	d3dinti = true;
	return true;
}

bool Device::DrawLine() {
	Shader vert[2]{
		{DirectX::XMFLOAT3(100.0f,100.0f,1.f), Green},
		{DirectX::XMFLOAT3(600.2f,600.8f,1.f), Green}
	};

	d3dcon->VSSetConstantBuffers(0, 1, &cBuf);
	d3dcon->RSSetViewports(1, &d3dVP);
	d3dcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Shader) * ARRAYSIZE(vert);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &vert;

	d3ddev->CreateBuffer(&bd, &srd, &vBuf);

	UINT stride = sizeof(Shader);
	UINT offset = 0;

	d3dcon->IASetInputLayout(d3dIL);
	d3dcon->IASetVertexBuffers(0, 1, &vBuf, &stride, &offset);
	d3dcon->VSSetShader(vShader, nullptr, 0);
	d3dcon->PSSetShader(pShader, nullptr, 0);
	d3dcon->OMSetRenderTargets(1, &d3dRTV, nullptr);

	d3dcon->Draw(2, 0);
	return true;
}

bool Device::Startup() {
	sc->GetDevice(__uuidof(ID3D11Device), (void**)&d3ddev);
	d3ddev->GetImmediateContext(&d3dcon);
	old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	d3dcon->RSGetViewports(&old.ViewportsCount, old.Viewports);
	old.PSInstancesCount = old.VSInstancesCount = 256;
	d3dcon->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
	d3dcon->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
	d3dcon->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
	d3dcon->IAGetPrimitiveTopology(&old.PrimitiveTopology);
	d3dcon->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
	d3dcon->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
	d3dcon->IAGetInputLayout(&old.InputLayout);
	return true;
}
bool Device::Shutdown() {
	d3dcon->RSSetViewports(old.ViewportsCount, old.Viewports);
	d3dcon->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
	for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
	d3dcon->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
	d3dcon->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
	for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
	d3dcon->IASetPrimitiveTopology(old.PrimitiveTopology);
	d3dcon->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
	d3dcon->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
	d3dcon->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
	return true;
}


void Device::ClearScreen() {
	float clearColor[4] = { 1.f, 1.f, 1.f, 0.f }; 
	d3dcon->ClearRenderTargetView(d3dRTV, clearColor);
}

void Device::DrawTriangle() {
	Shader vertices[3] = {
		{ DirectX::XMFLOAT3(1.0f, 0.f, 1.0f), Red},
		{ DirectX::XMFLOAT3(1.f, 1.f, 1.0f), Green },
		{ DirectX::XMFLOAT3(0.5f, 1.f, 1.0f), Blue }
	};

	// Create vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Shader) * ARRAYSIZE(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertices;

	HRESULT hr = d3ddev->CreateBuffer(&bd, &initData, &vBuf);
	if (FAILED(hr)) {
		logger->PrintError(L"Failed to create vertex buffer");
		return;
	}

	UINT stride = sizeof(Shader);
	UINT offset = 0;
	
	d3dcon->OMSetRenderTargets(1, &d3dRTV, nullptr);

	//ConstantBuffer cb;
	//cb.mProjection = ortho;
	//d3dcon->UpdateSubresource(cBuf, 0, nullptr, &cb, 0, 0);

	d3dcon->IASetVertexBuffers(0, 1, &vBuf, &stride, &offset);
	d3dcon->IASetInputLayout(d3dIL);



	d3dcon->VSSetShader(vShader, nullptr, 0);
	d3dcon->PSSetShader(pShader, nullptr, 0);

	d3dcon->RSSetViewports(1, &d3dVP);

	d3dcon->VSSetConstantBuffers(0, 1, &cBuf);
	d3dcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dcon->Draw(3, 0);
	vBuf->Release();
	cBuf->Release();
}





/*

	static const char* pixelShader =
		"struct PS_INPUT\
			{\
			float4 pos : SV_POSITION;\
			float4 col : COLOR0;\
			float2 uv  : TEXCOORD0;\
			};\
			sampler sampler0;\
			Texture2D texture0;\
			\
			float4 main(PS_INPUT input) : SV_Target\
			{\
			float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
			return out_col; \
			}";


	static const char* vertexShader =
		"cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";

*/

//HWND window = NULL;
//WNDPROC oWndProc;
//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
//		return true;
//
//	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
//}

/*bool Device::imguitest(IDXGISwapChain* sc) {
	if (!d3dinti) {
		ID3D11Texture2D* pbackBuffer = nullptr;
		sc->GetDevice(__uuidof(ID3D11Device), (void**)&d3ddev);
		d3ddev->GetImmediateContext(&d3dcon);
		DXGI_SWAP_CHAIN_DESC scd;
		if (sc->GetDesc(&scd) != S_OK) {
			logger->PrintError(L"GetDesc Poo");
		}
		window = scd.OutputWindow;
		d3dcon->OMGetRenderTargets(1, &d3dRTV, nullptr);

		oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(d3ddev, d3dcon);
		d3dinti = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	ImGui::EndFrame();
	ImGui::Render();
	d3dcon->OMSetRenderTargets(1, &d3dRTV, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return true;
}*/