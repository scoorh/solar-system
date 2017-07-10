#ifndef __RTV_H__
#define __RTV_H__


#include "common.h"


class RTV {
	ID3D11ShaderResourceView* cSRV;
	ID3D11RenderTargetView* cRTV;
	ID3D11ShaderResourceView* dSRV;
	ID3D11DepthStencilView *dDSV;

	bool init_c();
	bool init_d();

	int width;
	int height;

public:
	RTV();
	~RTV();

	bool init(const int& w = 512, const int& h = 512);

	ID3D11ShaderResourceView* color() { return cSRV; }
	ID3D11ShaderResourceView* depth() { return dSRV; }
	
	void begin(const D3DXCOLOR& c = D3DXCOLOR(0, 0, 0, 1));
	void end();
};



#endif