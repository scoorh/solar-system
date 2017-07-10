#include "sun.h"


Sun::Sun() : quadvb(0), quadib(0), depth(0), final(0),
flare(0),
depthsh(0), scene(0), help(0),
vs(0), l(0), matrixB(0), lightB(0),
flareSh(0), raysSh(0), blendSh(0), pslightB(0)
{}


Sun::~Sun() {
	if (quadvb) quadvb->Release();
	if (quadib) quadib->Release();
	if (depth) delete depth;
	if (scene) delete scene;
	if (help) delete help;
	if (final) delete final;
	if (matrixB) matrixB->Release();
	if (lightB) lightB->Release();
	if (pslightB) pslightB->Release();
	if (vs) vs->Release();
	if (l) l->Release();

	if (flareSh) flareSh->Release();
	if (raysSh) raysSh->Release();
	if (blendSh) blendSh->Release();
}


RTV *Sun::get_rtv() {
	return final;
}


void Sun::begin_depth() {
	depth->begin();
	depthsh->use();
}


void Sun::end_depth() {
	depth->end();
}


void Sun::begin_scene() {
	scene->begin();
}


void Sun::end_scene() {
	scene->end();
}


void Sun::pre_draw() {
	ID3D11ShaderResourceView *c, *d;

	devc->IASetInputLayout(l);
	devc->VSSetShader(vs, 0, 0);
	update_b();

	help->begin();
	devc->PSSetShader(flareSh, 0, 0);
	dx->set_clamp_samlper();
	d = depth->color();
	c = flare->get();
	devc->PSSetShaderResources(0, 1, &d);
	devc->PSSetShaderResources(1, 1, &c);
	draw_b();
	help->end();

	final->begin();
	devc->PSSetShader(raysSh, 0, 0);
	dx->set_clamp_samlper();
	c = help->color();
	devc->PSSetShaderResources(0, 1, &c);
	draw_b();
	final->end();
}


void Sun::draw() {
	dx->set_depth(false);
	ID3D11ShaderResourceView *i, *s;
//	i = help->color();
	i = final->color();
	s = scene->color();
	
	devc->PSSetShader(blendSh, 0, 0);
	devc->VSSetShader(vs, 0, 0);
	devc->IASetInputLayout(l);
	dx->set_clamp_samlper();	
	devc->PSSetShaderResources(0, 1, &i);
	devc->PSSetShaderResources(1, 1, &s);	
	update_b();
	draw_b();

	dx->set_depth(true);
}


bool Sun::init() {
	width = wndw;
	height = wndh;

	if (!init_b()) return false;
	
	depth = new RTV;
	if (!depth) return false;
	if (!depth->init(width, height)) return false;

	final = new RTV;
	if (!final) return false;
	if (!final->init(width, height)) return false;

	help = new RTV;
	if (!help) return false;
	if (!help->init(width, height)) return false;

	scene = new RTV;
	if (!scene) return false;
	if (!scene->init(width, height)) return false;


	depthsh = managers->shaders.find("depth");
	

	if (!depthsh) return false;

	
	flare = managers->textures.find_and_init_f("DATA/TEXTURE/flare.png");
	if (!flare) return false;
	

	if (!init_s()) return false;

	return true;
}


void Sun::draw_b() {
	unsigned int s = sizeof(V);
	unsigned int o = 0;
	devc->IASetVertexBuffers(0, 1, &quadvb, &s, &o);
	devc->IASetIndexBuffer(quadib, DXGI_FORMAT_R32_UINT, 0);
	devc->DrawIndexed(6, 0, 0);
}


bool Sun::init_s() {
	ID3D10Blob* emsg;
	ID3D10Blob* vsb;

	if (FAILED(D3DX11CompileFromFileA("FX/sun.vs", NULL, NULL, "vs", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vsb, &emsg, NULL))) {
		MessageBox(0, (char*)emsg->GetBufferPointer(), "!", 0);
		return false;
	}
	if (FAILED(dev->CreateVertexShader(vsb->GetBufferPointer(), vsb->GetBufferSize(), NULL, &vs))) {
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC pl[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (FAILED(dev->CreateInputLayout(pl, 2, vsb->GetBufferPointer(), vsb->GetBufferSize(), &l))) {
		vsb->Release();
		vsb = 0;
		return false;
	}

	vsb->Release();
	vsb = 0;


	matrixB = Shader::init_cb(sizeof(MatrixBuffer));
	lightB = Shader::init_cb(sizeof(LB));
	pslightB = Shader::init_cb(sizeof(LB));

	if (!matrixB) return false;
	if (!lightB) return false;
	if (!pslightB) return false;


	flareSh = Shader::init_ps("FX/sun_flare.ps");
	if (!flareSh) return false;

	raysSh = Shader::init_ps("FX/sun_rays.ps");
	if (!raysSh) return false;

	blendSh = Shader::init_ps("FX/sun_blend.ps");
	if (!blendSh) return false;

	return true;
}



void Sun::update_b() {
	D3DXMATRIX w, v, p;
	D3DXMatrixLookAtLH(&v, &D3DXVECTOR3(0, 0, -1), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&p, (float)wndw, (float)wndh, 0.01f, 1000.0f);
	D3DXMatrixIdentity(&w);

	D3DXMATRIX vp;
	D3DXMatrixMultiply(&vp, &camera->view(), &camera->proj());

	D3DXMatrixTranspose(&w, &w);
	D3DXMatrixTranspose(&v, &v);
	D3DXMatrixTranspose(&p, &p);
	D3DXMatrixTranspose(&vp, &vp);


	D3D11_MAPPED_SUBRESOURCE mr;
	if (FAILED(devc->Map(matrixB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr))) return;
	MatrixBuffer* d;
	d = (MatrixBuffer*)mr.pData;
	d->w = w;
	d->v = v;
	d->p = p;
	devc->Unmap(matrixB, 0);

	devc->VSSetConstantBuffers(0, 1, &matrixB);

	if (FAILED(devc->Map(lightB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr))) return;
	LB* ld;
	ld = (LB*)mr.pData;
	ld->vp = vp;
	devc->Unmap(lightB, 0);

	devc->VSSetConstantBuffers(1, 1, &lightB);


	D3DXMATRIX ivp;
	D3DXMatrixInverse(&ivp, 0, &vp);
	if (FAILED(devc->Map(pslightB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr))) return;
	ld = (LB*)mr.pData;
	ld->vp = ivp;
	devc->Unmap(lightB, 0);

	devc->PSSetConstantBuffers(0, 1, &pslightB);

}



bool Sun::init_b() {
	V *v = new V[6];
	float left = -((float)width / 2);
	float right = (float)width / 2;
	float top = (float)height / 2;
	float bottom = -((float)height / 2);

	v[0].p = D3DXVECTOR3(left, top, 0.0f);  
	v[0].tc = D3DXVECTOR2(0.0f, 0.0f);
	v[1].p = D3DXVECTOR3(right, bottom, 0.0f);  
	v[1].tc = D3DXVECTOR2(1.0f, 1.0f);
	v[2].p = D3DXVECTOR3(left, bottom, 0.0f); 
	v[2].tc = D3DXVECTOR2(0.0f, 1.0f);
	v[3].p = D3DXVECTOR3(left, top, 0.0f); 
	v[3].tc = D3DXVECTOR2(0.0f, 0.0f);
	v[4].p = D3DXVECTOR3(right, top, 0.0f);  
	v[4].tc = D3DXVECTOR2(1.0f, 0.0f);
	v[5].p = D3DXVECTOR3(right, bottom, 0.0f);
	v[5].tc = D3DXVECTOR2(1.0f, 1.0f);

	unsigned long *i = new unsigned long[6];
	i[0] = 0; i[1] = 1; i[2] = 2; 
	i[3] = 3; i[4] = 4; i[5] = 5;

	quadvb = Model::init_vb(&v[0], sizeof(V)* 6);
	quadib = Model::init_ib(&i[0], sizeof(unsigned long)* 6);

	delete v;
	delete i;

	if (!quadvb) return false;
	if (!quadib) return false;

	return true;
}