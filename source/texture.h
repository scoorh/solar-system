#ifndef __TEXTURE_H__
#define __TEXTURE_H__



#include "common.h"


class Texture {
	ID3D11ShaderResourceView* srv;

public:
	Texture();
	~Texture();

	bool init_from_file(const char* f);

	ID3D11ShaderResourceView *get();
	void use(const int& slot);
};



#endif