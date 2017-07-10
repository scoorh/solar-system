#include "camera.h"


Camera * camera = 0;

bool Camera::init() {
	camera = new Camera();
	if (!camera) return false;
	return true;
}


void Camera::destroy() {
	if (camera) delete camera;
}


D3DXMATRIX Camera::view() { return v; }
D3DXMATRIX Camera::proj() { return p; }
D3DXVECTOR3 Camera::position() { return pos; }


Camera::Camera()
: l(D3DXVECTOR3(0, 0, 1)), u(D3DXVECTOR3(0, 1, 0)), r(D3DXVECTOR3(1, 0, 0)), pos(D3DXVECTOR3(0,0,-10))
{}


void Camera::set_lens(const float& f) {
	D3DXMatrixPerspectiveFovLH(&p, f*static_cast<float>(D3DX_PI), static_cast<float>(wndw)/static_cast<float>(wndh), 0.01f, 1000.0f);
}


void Camera::move_forward(const float& d){
	pos += d * l;
}


void Camera::move_side(const float& d) {
	pos += d * r;
}


void Camera::rotate_x(const float& a) {
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &r, a);

	D3DXVec3TransformNormal(&u, &u, &R);
	D3DXVec3TransformNormal(&l, &l, &R);
}


void Camera::rotate_y(const float& a)  {
	D3DXMATRIX R;
	D3DXMatrixRotationY(&R, a);

	D3DXVec3TransformNormal(&r, &r, &R);
	D3DXVec3TransformNormal(&u, &u, &R);
	D3DXVec3TransformNormal(&l, &l, &R);
}


void Camera::rebuild_view() {
	D3DXVec3Normalize(&l, &l);
	D3DXVec3Cross(&u, &l, &r);
	D3DXVec3Normalize(&u, &u);
	D3DXVec3Cross(&r, &u, &l);
	D3DXVec3Normalize(&r, &r);

	float x = -D3DXVec3Dot(&pos, &r);
	float y = -D3DXVec3Dot(&pos, &u);
	float z = -D3DXVec3Dot(&pos, &l);

	v(0, 0) = r.x;
	v(1, 0) = r.y;
	v(2, 0) = r.z;
	v(3, 0) = x;
	v(0, 1) = u.x;
	v(1, 1) = u.y;
	v(2, 1) = u.z;
	v(3, 1) = y;
	v(0, 2) = l.x;
	v(1, 2) = l.y;
	v(2, 2) = l.z;
	v(3, 2) = z;
	v(0, 3) = 0.0f;
	v(1, 3) = 0.0f;
	v(2, 3) = 0.0f;
	v(3, 3) = 1.0f;
}