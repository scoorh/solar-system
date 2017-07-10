#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "common.h"


class Camera {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 r;
	D3DXVECTOR3 l;
	D3DXVECTOR3 u;

	D3DXMATRIX v;
	D3DXMATRIX p;

public:
	Camera();

	void rebuild_view();
	void set_lens(const float& f);
	void move_forward(const float& d);
	void move_side(const float& d);
	void rotate_x(const float& a);
	void rotate_y(const float& a);


	D3DXMATRIX view();
	D3DXMATRIX proj();
	D3DXVECTOR3 position();
	D3DXVECTOR3& rpos() { return pos; }

	static bool init();
	static void destroy();
};


extern Camera*  camera;


#endif