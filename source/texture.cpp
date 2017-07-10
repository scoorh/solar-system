#include "texture.h"


Texture::Texture() : srv(0) {}

Texture::~Texture() {
	if (srv) srv->Release();
}

bool Texture::init_from_file(const char* f) {	
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(dev, f, NULL, NULL, &srv, NULL))) return false;
	return true;
}

ID3D11ShaderResourceView *Texture::get() {
	return srv;
}

void Texture::use(const int& slot) {
	devc->PSSetShaderResources(slot, 1, &srv);
}