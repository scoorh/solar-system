#ifndef __SUN_H__
#define __SUN_H__



#include "common.h"
#include "__shaders.h"
#include "resource_manager.h"
#include "camera.h"
#include "object.h"
#include "rtv.h"



class Sun {
	struct V {
		D3DXVECTOR3 p;
		D3DXVECTOR2 tc;
	};
	struct LB {
		D3DXMATRIX vp;
	};

	ID3D11Buffer* quadvb;
	ID3D11Buffer* quadib;

	Shader* depthsh;
	Texture *flare;


	ID3D11Buffer* matrixB;
	ID3D11Buffer* lightB;
	ID3D11Buffer* pslightB;
	ID3D11VertexShader* vs;
	ID3D11InputLayout* l;

	ID3D11PixelShader* flareSh;
	ID3D11PixelShader* raysSh;
	ID3D11PixelShader* blendSh;


	int width;
	int height;

	RTV *depth;
	RTV *scene;
	RTV *final;
	RTV *help;

	void update_b();	

	bool init_s();
	bool init_b();
	void draw_b();
	
public:
	Sun();
	~Sun();

	bool init();
	void begin_depth();
	void end_depth();
	void begin_scene();
	void end_scene();

	void pre_draw();
	void draw();


	RTV *get_rtv();
};



#endif