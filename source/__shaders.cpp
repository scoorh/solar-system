#include "__shaders.h"


BasicMeshShader::BasicMeshShader() : vb(0)
{}


BasicMeshShader::~BasicMeshShader() {
	if (vb) vb->Release();
}


bool BasicMeshShader::init() {
	if (!init_s("FX/basic_model.vs", "FX/basic_model.ps")) { vsb->Release(); vsb = 0; return false; }

	vb = init_cb(sizeof(VarBuffer));
	if (!vb) return false;

	D3D11_INPUT_ELEMENT_DESC pl[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(dev->CreateInputLayout(pl, 3, vsb->GetBufferPointer(), vsb->GetBufferSize(), &l))) {
		vsb->Release();
		vsb = 0;
		return false;
	}

	vsb->Release();
	vsb = 0;

	return true;
}


void BasicMeshShader::update_vb(D3DXVECTOR3 cp) {
	D3D11_MAPPED_SUBRESOURCE mr;
	if (FAILED(devc->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr))) return;

	VarBuffer* d;
	d = (VarBuffer*)mr.pData;
	d->cp = cp;
	devc->Unmap(vb, 0);

	devc->PSSetConstantBuffers(0, 1, &vb);
}







bool SkyDomeShader::init() {
	if (!init_s("FX/sky_dome.vs", "FX/sky_dome.ps")) { vsb->Release(); vsb = 0; return false; }

	
	D3D11_INPUT_ELEMENT_DESC pl[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PSIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (FAILED(dev->CreateInputLayout(pl, 3, vsb->GetBufferPointer(), vsb->GetBufferSize(), &l))) {
		vsb->Release();
		vsb = 0;
		return false;
	}

	vsb->Release();
	vsb = 0;

	return true;
}





bool DepthShader::init() {
	if (!init_s("FX/depth.vs", "FX/depth.ps")) { vsb->Release(); vsb = 0; return false; }

	D3D11_INPUT_ELEMENT_DESC pl[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (FAILED(dev->CreateInputLayout(pl, 1, vsb->GetBufferPointer(), vsb->GetBufferSize(), &l))) {
		vsb->Release();
		vsb = 0;
		return false;
	}

	vsb->Release();
	vsb = 0;

	return true;
}

