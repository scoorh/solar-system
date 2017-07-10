#ifndef __SHADER_H__
#define __SHADER_H__


#include "common.h"


struct MatrixBuffer {
	D3DXMATRIX w;
	D3DXMATRIX v;
	D3DXMATRIX p;
};


class Shader {
protected:
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* l;
	ID3D11Buffer* mb;

	ID3D10Blob* vsb;

	bool init_vs(const char* f);	
	bool init_s(const char* fvs, const char* fps);	

public:
	Shader();
	virtual ~Shader();

	virtual bool init() = 0;
	virtual void use();

	virtual void update_mb(D3DXMATRIX w, D3DXMATRIX v, D3DXMATRIX p);
	virtual void update_vb(D3DXVECTOR3 cp) {}

	static ID3D11PixelShader* init_ps(const char* f);
	static ID3D11Buffer* init_cb(const int& s);
};


extern Shader *ashader;


#endif