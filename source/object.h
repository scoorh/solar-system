#ifndef __OBJECT_H__
#define __OBJECT_H__


#include "common.h"
#include "camera.h"
#include "__shaders.h"
#include "resource_manager.h"


class Object;
struct MovementData {
	D3DXVECTOR3 center;
	D3DXVECTOR3 axes;
	Object* around;
	float angle;
	float time;

	MovementData() : around(0) {}
};


class Object {
	friend struct MovementData;

	D3DXVECTOR3 p;
	D3DXVECTOR3 r;
	D3DXVECTOR3 s;

	D3DXMATRIX tm;
	D3DXMATRIX rm;
	D3DXMATRIX sm;
	D3DXMATRIX w;

	D3DXVECTOR3 spin;
	MovementData moveData;

	Model *m;
	Texture *dt;
	Texture *st;

public:
	Object();
	~Object();

	void draw();
	void update(const float& dt);

	void set_model(const char* n);
	void set_diffuse_text(const char* n);
	void set_second_text(const char* n);

	void set_move_data(const D3DXVECTOR3& center, const D3DXVECTOR3& axes);
	void set_move_around(const D3DXVECTOR3& axes, Object* obj);
	void set_move_currangle(const float& f);
	void set_move_time(const float& f);

	void set_translation(const float& x, const float& y, const float& z);
	void set_rotation(const float& x, const float& y, const float& z);
	void set_scale(const float& x, const float& y, const float& z);
	void set_spin(const float& x, const float& y, const float& z);

	void add_translation(const float& x, const float& y, const float& z);
	void add_rotation(const float& x, const float& y, const float& z);
	void add_scale(const float& x, const float& y, const float& z);
	void add_spin(const float& x, const float& y, const float& z);
};



#endif