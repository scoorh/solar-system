#ifndef __DX_H__
#define __DX_H__



#include "common.h"


LRESULT CALLBACK winproc(HWND w, UINT m, WPARAM wp, LPARAM lp);


class RTV;

class DX {
	ID3D11Device *d;
	ID3D11DeviceContext *dc;
	IDXGISwapChain *sc;
	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* dsb;	
	ID3D11DepthStencilView* dsv;

	ID3D11DepthStencilState* depth;
	ID3D11DepthStencilState* nodepth;

	ID3D11RasterizerState* cull;
	ID3D11RasterizerState* nocull;

	ID3D11SamplerState *wrap;
	ID3D11SamplerState *clamp;
	ID3D11SamplerState *mirror;

	ID3D11BlendState* blend;
	ID3D11BlendState* noblend;

	D3D11_VIEWPORT vp;


	HWND wnd;
	HINSTANCE inst;
	bool init_wnd();


	DX();
	~DX();

	bool _init();

public:
	void begin(const D3DXCOLOR& c = D3DXCOLOR(0,0,0,1));
	void end();

	static bool init();
	static void destroy();

	HWND get_wnd();
	HINSTANCE get_inst();

	void set_culling(const bool& b);
	void set_depth(const bool& b);
	void set_wrap_samlper(const int& s = 0);
	void set_clamp_samlper(const int& s = 0);
	void set_mirror_samlper(const int& s = 0);
	void set_blending(const bool& b);
	void restore_rtv();
	void restore_vp();
};


extern DX* dx;



#endif