#ifndef __SKY_DOME_H__
#define __SKY_DOME_H__


#include "common.h"
#include "__shaders.h"
#include "resource_manager.h"
#include "camera.h"


class SkyDome {
	struct V {
		D3DXVECTOR3 p;
		D3DXVECTOR2 tc;
		float t;
	};

	Shader *sh;

	Texture *front;
	Texture *back;
	Texture *top;
	Texture* bottom;
	Texture* left;
	Texture* right;

	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
	int ic;

public:
	SkyDome();
	~SkyDome();

	bool init();
	void draw();
};



#endif