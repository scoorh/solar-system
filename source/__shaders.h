#ifndef ____SHADERS_H__
#define ____SHADERS_H__



#include "shader.h"
#include "dx.h"


enum ModelType {
	BASIC,
	ALPHA_MAP,
};


class BasicMeshShader : public Shader {
	ID3D11Buffer* vb;

	struct VarBuffer {
		D3DXVECTOR3 cp;
		float paddddddddddddddddd;
	};

public:
	BasicMeshShader();
	~BasicMeshShader();

	bool init();

	void update_vb(D3DXVECTOR3 cp);
};



class SkyDomeShader : public Shader {
public:
	SkyDomeShader() {}
	~SkyDomeShader() {}

	bool init();
};



class DepthShader : public Shader {
public:
	DepthShader() {}
	~DepthShader() {}

	bool init();
};









#endif