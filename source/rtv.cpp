#include "rtv.h"
#include "dx.h"


RTV::RTV()
: cSRV(0), cRTV(0), dSRV(0), dDSV(0) {}


RTV::~RTV() {
	if (cSRV) cSRV->Release();
	if (cRTV) cRTV->Release();
	if (dSRV) dSRV->Release();
	if (dDSV) dDSV->Release();
}


void RTV::begin(const D3DXCOLOR& c) {
	devc->OMSetRenderTargets(1, &cRTV, dDSV);
	devc->ClearRenderTargetView(cRTV, c);
	devc->ClearDepthStencilView(dDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void RTV::end() {
	dx->restore_rtv();
	dx->restore_vp();
}


bool RTV::init(const int& w, const int& h) {
	width = w;
	height = h;
	if (!init_c()) return false;
	if (!init_d()) return false;
	return true;
}


bool RTV::init_c() {
	ID3D11Texture2D* t;

	D3D11_TEXTURE2D_DESC td;
	memset(&td, 0, sizeof(td));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	if (FAILED(dev->CreateTexture2D(&td, 0, &t))) return false;

	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	memset(&rtvd, 0, sizeof(rtvd));
	rtvd.Format = td.Format;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;
	if (FAILED(dev->CreateRenderTargetView(t, &rtvd, &cRTV))) { t->Release(); return false; }

	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	memset(&srvd, 0, sizeof(srvd));
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;
	if (FAILED(dev->CreateShaderResourceView(t, &srvd, &cSRV))) { t->Release(); return false; }
	
	t->Release();

	return true;
}

bool RTV::init_d() {

	ID3D11Texture2D* t;

	D3D11_TEXTURE2D_DESC td;
	memset(&td, 0, sizeof(td));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R32_TYPELESS;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	if (FAILED(dev->CreateTexture2D(&td, 0, &t))) return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC  rtvd;
	memset(&rtvd, 0, sizeof(rtvd));
	rtvd.Format = DXGI_FORMAT_D32_FLOAT;
	rtvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;;
	rtvd.Texture2D.MipSlice = 0;
	if (FAILED(dev->CreateDepthStencilView(t, &rtvd, &dDSV))) { t->Release(); return false; }
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	memset(&srvd, 0, sizeof(srvd));
	srvd.Format = DXGI_FORMAT_R32_FLOAT;
	srvd.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	if (FAILED(dev->CreateShaderResourceView(t, &srvd, &dSRV))) { t->Release(); return false; }
	
	t->Release();

	return true;
}

