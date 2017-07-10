#include "object.h"


#define DEBUG_DENY 0.5f


Object::Object() : m(0), dt(0), st(0) {
	set_translation(0, 0, 0);
	set_rotation(0, 0, 0);
	set_scale(1, 1, 1); 	
	set_spin(0, 0, 0);
}


Object::~Object() {}


void Object::draw() {
	ashader->update_mb(w, camera->view(), camera->proj());
	ashader->update_vb(camera->position());
	if (dt) dt->use(0);
	if (st) st->use(1);
	m->pre_draw();
	m->draw();
}


void Object::update(const float& dt) {
	moveData.angle += moveData.time * DEG_TO_RAD(dt) * DEBUG_DENY;
	if (moveData.angle > 6.28) moveData.angle = 0;

	add_rotation(DEG_TO_RAD(spin.x*dt), DEG_TO_RAD(spin.y*dt), DEG_TO_RAD(spin.z*dt));
	if (r.x > 6.28) r.x = 0;
	if (r.y > 6.28) r.y = 0;
	if (r.z > 6.28) r.z = 0;

	float x = 0, y = 0, z = 0;
	if (moveData.around) {
		x = moveData.around->p.x;
		y = moveData.around->p.y;
		z = moveData.around->p.z;
	}

	x = (x + (moveData.axes.x * cosf(moveData.angle)));
	z = (z + (moveData.axes.z * sinf(moveData.angle)));
	y = (y + (moveData.axes.y * sinf(moveData.angle)));

	set_translation(x, y, z);

	D3DXMatrixMultiply(&w, &sm, &rm);
	D3DXMatrixMultiply(&w, &w, &tm);
}


void Object::set_move_time(const float& f) {
	moveData.time = f;
}

void Object::set_move_data(const D3DXVECTOR3& center, const D3DXVECTOR3& axes) {
	moveData.axes = axes;
	moveData.center = center;
}

void Object::set_move_around(const D3DXVECTOR3& axes, Object* obj) {
	moveData.axes = axes;
	moveData.around = obj;
}

void Object::set_move_currangle(const float& f) {
	moveData.angle = f;
}


void Object::set_model(const char* n) {
	m = managers->models.find_and_init_f(n);
}


void Object::set_diffuse_text(const char* n) {
	dt = managers->textures.find_and_init_f(n);
}


void Object::set_second_text(const char* n) {
	st = managers->textures.find_and_init_f(n);
}



void Object::set_translation(const float& x, const float& y, const float& z) {
	p.x = x; p.y = y; p.z = z;
	D3DXMatrixTranslation(&tm, p.x, p.y, p.z);
}

void Object::set_rotation(const float& x, const float& y, const float& z) {
	r.x = x; r.y = y; r.z = z;
	D3DXMatrixRotationYawPitchRoll(&rm, r.y, r.x, r.z);
}

void Object::set_scale(const float& x, const float& y, const float& z) {
	s.x = x; s.y = y; s.z = z;
	D3DXMatrixScaling(&sm, s.x, s.y, s.z);
}

void Object::set_spin(const float& x, const float& y, const float& z) {
	spin.x = x; spin.y = y; spin.z = z;
}

void Object::add_translation(const float& x, const float& y, const float& z) {
	p.x += x; p.y += y; p.z += z;
	D3DXMatrixTranslation(&tm, p.x, p.y, p.z);
}

void Object::add_rotation(const float& x, const float& y, const float& z) {
	r.x += x; r.y += y; r.z += z;
	D3DXMatrixRotationYawPitchRoll(&rm, r.y, r.x, r.z);
}

void Object::add_scale(const float& x, const float& y, const float& z) {
	s.x += x; s.y += y; s.z += z;
	D3DXMatrixScaling(&sm, s.x, s.y, s.z);
}

void Object::add_spin(const float& x, const float& y, const float& z) {
	spin.x += x; spin.y += y; spin.z += z;
}


