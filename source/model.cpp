#include "model.h"


Model::Model() :
vb(0), ib(0), ic(0)
{}


Model::~Model() {
	if (vb) vb->Release();
	if (ib) ib->Release();
}


bool Model::init_from_file(const char* f) {
	Vertex *v;
	unsigned long *i;

	std::fstream file;
	file.open(f, std::ios_base::in);
	if (!file.is_open()) return false;

	file >> ic;
	v = new Vertex[ic];
	if (!v) {
		file.close(); return false;
	}

	i = new unsigned long[ic];
	if (!i) {
		delete[] v; file.close(); return false;
	}	

	for (int x = 0; x < ic; x++) {
		file >> v[x].p.x >> v[x].p.y >> v[x].p.z;
		file >> v[x].tc.x >> v[x].tc.y;
		file >> v[x].n.x >> v[x].n.y >> v[x].n.z;
		i[x] = x;
	}


	vb = init_vb(&v[0], ic);
	ib = init_ib(&i[0], ic); 

	delete v;
	delete i;

	if (!vb) return false;
	if (!ib) return false;

	return true;
}


void Model::pre_draw() {
	unsigned int s = sizeof(Vertex);
	unsigned int o = 0;
	devc->IASetVertexBuffers(0, 1, &vb, &s, &o);
	devc->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
}


void Model::draw() {
	devc->DrawIndexed(ic, 0, 0);
}


ID3D11Buffer* Model::init_vb(void* d, int s) {
	ID3D11Buffer* b = 0;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex)* s;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA bid;
	bid.pSysMem = d;
	dev->CreateBuffer(&bd, &bid, &b);

	return b;
}


ID3D11Buffer* Model::init_ib(void* d, int s) {
	ID3D11Buffer* b = 0;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned long)* s;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA bid;
	bid.pSysMem = d;
	dev->CreateBuffer(&bd, &bid, &b);

	return b;
}