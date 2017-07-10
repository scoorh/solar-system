#include "dx.h"
#include "rtv.h"


LRESULT CALLBACK winproc(HWND w, UINT m, WPARAM wp, LPARAM lp) {
	switch (m){
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(w, m, wp, lp);
}


DX* dx = 0;

bool DX::init() {
	dx = new DX();
	if (!dx) return false;
	if (!dx->_init()) return false;

	return true;
}


void DX::destroy() {
	if (dx) delete dx;
}

void DX::set_blending(const bool& b) {
	float bf[4];
	bf[0] = 0.0f;
	bf[1] = 0.0f;
	bf[2] = 0.0f;
	bf[3] = 0.0f;
	if (b) dc->OMSetBlendState(blend, bf, 0xffffffff);
	else dc->OMSetBlendState(noblend, bf, 0xffffffff);
}

void DX::set_culling(const bool& b) {
	if (b) dc->RSSetState(cull);
	else dc->RSSetState(nocull);
}

void DX::set_depth(const bool& b) {
	if (b) dc->OMSetDepthStencilState(depth,1);
	else dc->OMSetDepthStencilState(nodepth,1);
}

void DX::set_mirror_samlper(const int& s) {
	dc->PSSetSamplers(s, 1, &mirror);
}

void DX::set_wrap_samlper(const int& s) {
	dc->PSSetSamplers(s, 1, &wrap);
}

void DX::set_clamp_samlper(const int& s) {
	dc->PSSetSamplers(s, 1, &clamp);
}


void DX::restore_rtv() {
	dc->OMSetRenderTargets(1, &rtv, dsv);
}

void DX::restore_vp() {
	dc->RSSetViewports(1, &vp);
}


HWND DX::get_wnd() { return wnd; }
HINSTANCE DX::get_inst() { return inst; }


ID3D11Device* dev = 0;
ID3D11DeviceContext *devc = 0;


DX::DX() : d(0), dc(0), sc(0), rtv(0), dsv(0), nodepth(0), depth(0), dsb(0), cull(0), nocull(0), wrap(0), clamp(0)
, wnd(0), inst(0), blend(0), noblend(0), mirror(0)
{}


DX::~DX() {
	if (sc) sc->SetFullscreenState(false, NULL); 

	if (mirror) mirror->Release();
	if (blend) blend->Release();
	if (noblend) noblend->Release();
	if (clamp) clamp->Release();
	if (wrap) wrap->Release();
	if (cull)	cull->Release();
	if (nocull) nocull->Release();
	if (dsv) dsv->Release();
	if (depth) depth->Release();
	if (nodepth) nodepth->Release();
	if (dsb) dsb->Release(); 
	if (rtv) rtv->Release(); 
	if (dc) dc->Release(); 
	if (d) d->Release();
	if (sc) sc->Release();

	DestroyWindow(wnd);
	UnregisterClass("solarsystemwnd", inst);
}


bool DX::_init() {
	if (!init_wnd()) return false;


	DXGI_SWAP_CHAIN_DESC scd;
	memset(&scd, 0, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = wndw;
	scd.BufferDesc.Height = wndh;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = wnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = true;

	D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &scd, &sc, &d, NULL, &dc))) return false;
	dev = d;
	devc = dc;


	ID3D11Texture2D *bb;
	if (FAILED(sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bb))) return false;
	if (FAILED(d->CreateRenderTargetView(bb, NULL, &rtv))) { bb->Release(); return false; }
	bb->Release();


	D3D11_TEXTURE2D_DESC dbd;
	dbd.Width = wndw;
	dbd.Height = wndh;
	dbd.MipLevels = 1;
	dbd.ArraySize = 1;
	dbd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dbd.SampleDesc.Count = 1;
	dbd.SampleDesc.Quality = 0;
	dbd.Usage = D3D11_USAGE_DEFAULT;
	dbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dbd.CPUAccessFlags = 0;
	dbd.MiscFlags = 0;
	if (FAILED(d->CreateTexture2D(&dbd, NULL, &dsb))) return false;

	
	D3D11_DEPTH_STENCIL_DESC dsd;
	memset(&dsd, 0, sizeof(dsd));
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilEnable = true;
	dsd.StencilReadMask = 0xFF;
	dsd.StencilWriteMask = 0xFF;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	if (FAILED(d->CreateDepthStencilState(&dsd, &depth))) { return false; }
	dc->OMSetDepthStencilState(depth, 1);

	dsd.DepthEnable = false;
	if (FAILED(d->CreateDepthStencilState(&dsd, &nodepth))) { return false; }

	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	memset(&dsvd, 0, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	if (FAILED(d->CreateDepthStencilView(dsb, &dsvd, &dsv))) return false;
	dc->OMSetRenderTargets(1, &rtv, dsv);


	vp.Width = (float)wndw;
	vp.Height = (float)wndh;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	dc->RSSetViewports(1, &vp);
	

	D3D11_RASTERIZER_DESC rd;
	rd.AntialiasedLineEnable = false;
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.FillMode = D3D11_FILL_SOLID;
//	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.FrontCounterClockwise = false;
	rd.MultisampleEnable = false;
	rd.ScissorEnable = false;
	rd.SlopeScaledDepthBias = 0.0f;
	if (FAILED(d->CreateRasterizerState(&rd, &cull))) return false;
	dc->RSSetState(cull);

	rd.CullMode = D3D11_CULL_NONE;
	if (FAILED(d->CreateRasterizerState(&rd, &nocull))) return false;



	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.MipLODBias = 0.0f;
	sd.MaxAnisotropy = 1;
	sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sd.BorderColor[0] = 0;
	sd.BorderColor[1] = 0;
	sd.BorderColor[2] = 0;
	sd.BorderColor[3] = 0;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	if (FAILED(d->CreateSamplerState(&sd, &wrap))) return false;

	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	if (FAILED(d->CreateSamplerState(&sd, &wrap))) return false;

	sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	if (FAILED(d->CreateSamplerState(&sd, &mirror))) return false;


	D3D11_BLEND_DESC bsd;
	memset(&bsd, 0, sizeof(bsd));
	bsd.RenderTarget[0].BlendEnable = TRUE;
	bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bsd.RenderTarget[0].RenderTargetWriteMask = 0xff;
	if (FAILED(d->CreateBlendState(&bsd, &blend))) return false;

	bsd.RenderTarget[0].BlendEnable = FALSE;
	if (FAILED(d->CreateBlendState(&bsd, &noblend))) return false; 
	
	return true;
}



void DX::begin(const D3DXCOLOR& c) {
	restore_rtv();
	restore_vp();
	dc->ClearRenderTargetView(rtv, c);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void DX::end() {
	sc->Present(0, 0);
}



bool DX::init_wnd() {
	HINSTANCE inst = GetModuleHandle(NULL);

	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = winproc;
	wc.hInstance = inst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = "solarsystemwnd";
	wc.cbSize = sizeof(WNDCLASSEX);
	if (!RegisterClassEx(&wc))
		return false;
	
	wnd = CreateWindowEx(WS_EX_APPWINDOW, "solarsystemwnd", "SOLAR SYSTEM :D", WS_POPUP, 0, 0, wndw, wndh, NULL, NULL, inst, NULL);
	if (!wnd) return false;
	ShowWindow(wnd, SW_SHOW);
	SetForegroundWindow(wnd);
	SetFocus(wnd);
	ShowCursor(false);

	return true;
}