#include "sky_dome.h"



SkyDome::SkyDome() : sh(0), front(0), back(0), top(0), bottom(0), left(0), right(0) {}

SkyDome::~SkyDome()
{}


bool SkyDome::init() {	
	sh = managers->shaders.find("sky_dome");
	if (!sh) return false;
	
	front = managers->textures.find_and_init_f("DATA/TEXTURE/skybox_front5.png");
	if (!front) return false;
	back = managers->textures.find_and_init_f("DATA/TEXTURE/skybox_back6.png");
	if (!back) return false;
	top = managers->textures.find_and_init_f("DATA/TEXTURE/skybox_top3.png");
	if (!top) return false;
	bottom = managers->textures.find_and_init_f("DATA/TEXTURE/skybox_bottom4.png");
	if (!bottom) return false;
	left = managers->textures.find_and_init_f("DATA/TEXTURE/skybox_left2.png");
	if (!left) return false;
	right = managers->textures.find_and_init_f("DATA/TEXTURE/skybox_right1.png");
	if (!right) return false;


	ic = 36;
	float s = 10.0f;
	V *v = new V[ic];
	//front
	v[0].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f); v[0].tc = D3DXVECTOR2(0, 1); v[0].t = 0;
	v[1].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f); v[1].tc = D3DXVECTOR2(0, 0); v[1].t = 0;
	v[2].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f); v[2].tc = D3DXVECTOR2(1, 0); v[2].t = 0;
	v[3].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f); v[3].tc = D3DXVECTOR2(0, 1); v[3].t = 0;
	v[4].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f); v[4].tc = D3DXVECTOR2(1, 0); v[4].t = 0;
	v[5].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f); v[5].tc = D3DXVECTOR2(1, 1); v[5].t = 0;

	//back
	v[6].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f); v[6].tc = D3DXVECTOR2(0, 1); v[6].t = 1;
	v[7].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f); v[7].tc = D3DXVECTOR2(0, 0); v[7].t = 1;
	v[8].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f); v[8].tc = D3DXVECTOR2(1, 0); v[8].t = 1;
	v[9].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f); v[9].tc = D3DXVECTOR2(0, 1); v[9].t = 1;
	v[10].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f); v[10].tc = D3DXVECTOR2(1, 0); v[10].t = 1;
	v[11].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f); v[11].tc = D3DXVECTOR2(1, 1); v[11].t = 1;

	//top
	v[12].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f); v[12].tc = D3DXVECTOR2(0, 1); v[12].t = 2;
	v[13].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f); v[13].tc = D3DXVECTOR2(0, 0); v[13].t = 2;
	v[14].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f); v[14].tc = D3DXVECTOR2(1, 0); v[14].t = 2;
	v[15].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f); v[15].tc = D3DXVECTOR2(0, 1); v[15].t = 2;
	v[16].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f); v[16].tc = D3DXVECTOR2(1, 0); v[16].t = 2;
	v[17].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f); v[17].tc = D3DXVECTOR2(1, 1); v[17].t = 2;

	//bottom
	v[18].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f); v[18].tc = D3DXVECTOR2(0, 1); v[18].t = 3;
	v[19].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f); v[19].tc = D3DXVECTOR2(0, 0); v[19].t = 3;
	v[20].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f); v[20].tc = D3DXVECTOR2(1, 0); v[20].t = 3;
	v[21].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f); v[21].tc = D3DXVECTOR2(0, 1); v[21].t = 3;
	v[22].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f); v[22].tc = D3DXVECTOR2(1, 0); v[22].t = 3;
	v[23].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f); v[23].tc = D3DXVECTOR2(1, 1); v[23].t = 3;

	//left
	v[24].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f); v[24].tc = D3DXVECTOR2(0, 1); v[24].t = 4;
	v[25].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f); v[25].tc = D3DXVECTOR2(0, 0); v[25].t = 4;
	v[26].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f); v[26].tc = D3DXVECTOR2(1, 0); v[26].t = 4;
	v[27].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f); v[27].tc = D3DXVECTOR2(0, 1); v[27].t = 4;
	v[28].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f); v[28].tc = D3DXVECTOR2(1, 0); v[28].t = 4;
	v[29].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f); v[29].tc = D3DXVECTOR2(1, 1); v[29].t = 4;

	//right
	v[30].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f); v[30].tc = D3DXVECTOR2(0, 1); v[30].t = 5;
	v[31].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f); v[31].tc = D3DXVECTOR2(0, 0); v[31].t = 5;
	v[32].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f); v[32].tc = D3DXVECTOR2(1, 0); v[32].t = 5;
	v[33].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f); v[33].tc = D3DXVECTOR2(0, 1); v[33].t = 5;
	v[34].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f); v[34].tc = D3DXVECTOR2(1, 0); v[34].t = 5;
	v[35].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f); v[35].tc = D3DXVECTOR2(1, 1); v[35].t = 5;

	for (int k = 0; k < 36; k++)
	{
		v[k].p.x *= s;
		v[k].p.y *= s;
		v[k].p.z *= s;
	}


	unsigned long *i = new unsigned long[ic];
	for (int k = 0; k < ic; k++)
		i[k] = k;

	vb = Model::init_vb(&v[0], sizeof(V)* ic);
	ib = Model::init_ib(&i[0], sizeof(unsigned long)* ic);
	if (!vb) return false;
	if (!ib) return false;
	

	delete v;
	delete i;

	return true;
}


void SkyDome::draw() {
	dx->set_culling(false);
	dx->set_depth(false);

	sh->use();

	ID3D11ShaderResourceView *srv;
	srv = front->get();
	devc->PSSetShaderResources(0, 1, &srv);
	srv = back->get();
	srv = top->get();
	srv = bottom->get();
	srv = left->get();
	srv = right->get();
	

	D3DXMATRIX w, t;
	D3DXMatrixIdentity(&w);
	D3DXMatrixTranslation(&t, camera->position().x, camera->position().y, camera->position().z);
	D3DXMatrixMultiply(&w, &w, &t);

	sh->update_mb(w, camera->view(), camera->proj());
	unsigned int s = sizeof(V);
	unsigned int o = 0;
	devc->IASetVertexBuffers(0, 1, &vb, &s, &o);
	devc->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	devc->DrawIndexed(ic,0,0);

	dx->set_culling(true);
	dx->set_depth(true);
}