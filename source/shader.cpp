#include "shader.h"


Shader *ashader = 0;


Shader::Shader()
: vs(0), ps(0), l(0), vsb(0), mb(0)
{}


Shader::~Shader() {
	if (vsb) vsb->Release();
	if (l) l->Release();
	if (ps) ps->Release();
	if (vs) vs->Release();
	if (mb) mb->Release();
}


void Shader::use() {
	ashader = this;
	devc->IASetInputLayout(l);
	devc->VSSetShader(vs, 0, 0);
	devc->PSSetShader(ps, 0, 0);
}


bool Shader::init_s(const char* fvs, const char* fps) {
	if (!init_vs(fvs)) return false;
	ps = init_ps(fps); if(!ps) return false;
	mb = init_cb(sizeof(MatrixBuffer));
	if (!mb) return false;
	return true;
}


bool Shader::init_vs(const char* f) {
	ID3D10Blob* emsg;

	if (FAILED(D3DX11CompileFromFileA(f, NULL, NULL, "vs", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vsb, &emsg, NULL))) {
		MessageBox(0, (char*)emsg->GetBufferPointer(), "!", 0);
		return false;
	}
	if (FAILED(dev->CreateVertexShader(vsb->GetBufferPointer(), vsb->GetBufferSize(), NULL, &vs))) {
		return false;
	}

	return true;
}


ID3D11PixelShader* Shader::init_ps(const char* f) {
	ID3D11PixelShader* s = 0;
	ID3D10Blob* emsg;
	ID3D10Blob* psb;

	if (FAILED(D3DX11CompileFromFileA(f, NULL, NULL, "ps", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &psb, &emsg, NULL))) {
		MessageBox(0, (char*)emsg->GetBufferPointer(), "!", 0);
		return 0;
	}
	
	dev->CreatePixelShader(psb->GetBufferPointer(), psb->GetBufferSize(), NULL, &s);
	psb->Release();
	return s;
}


ID3D11Buffer* Shader::init_cb(const int& s) {
	ID3D11Buffer* b = 0;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = s;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	dev->CreateBuffer(&bd, NULL, &b);

	return b;
}



void Shader::update_mb(D3DXMATRIX w, D3DXMATRIX v, D3DXMATRIX p) {
	D3DXMatrixTranspose(&w, &w);
	D3DXMatrixTranspose(&v, &v);
	D3DXMatrixTranspose(&p, &p);

	D3D11_MAPPED_SUBRESOURCE mr;
	if (FAILED(devc->Map(mb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr))) return;

	MatrixBuffer* d;
	d = (MatrixBuffer*)mr.pData;
	d->w = w;
	d->v = v;
	d->p = p;

	devc->Unmap(mb, 0);

	devc->VSSetConstantBuffers(0, 1, &mb);
}


