#ifndef __MODEL_H__
#define __MODEL_H__


#include "common.h"


class Model {
	struct Vertex {
		D3DXVECTOR3 p;
		D3DXVECTOR2 tc;
		D3DXVECTOR3 n;
	};

	ID3D11Buffer *vb;
	ID3D11Buffer *ib;
	int ic;

public:
	Model();
	~Model();

	bool init_from_file(const char* f);
	void pre_draw();
	void draw();


	static ID3D11Buffer* init_vb(void* d, int s);
	static ID3D11Buffer* init_ib(void* d, int s);
};


#endif