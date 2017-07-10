#include "input.h"

Input *input = 0;


bool Input::init() {
	input = new Input();
	if (!input) return false;
	if (!input->_init()) return false;
	return true;
}


void Input::destroy() {
	if (input) delete input;
}



Input::Input() : di(0), m(0), k(0)
{}


Input::~Input() {
	if (m) {
		m->Unacquire();
		m->Release();
	}

	if (k) {
		k->Unacquire();
		k->Release();
	}

	if (di) di->Release();

}


void Input::update() {	
	HRESULT r = k->GetDeviceState(sizeof(ks), (LPVOID)&ks);
	if (FAILED(r))
		if ((r == DIERR_INPUTLOST) || (r == DIERR_NOTACQUIRED))	
			k->Acquire();
	
	
	r = m->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&ms);
	if (FAILED(r))
		if ((r == DIERR_INPUTLOST) || (r == DIERR_NOTACQUIRED))
			m->Acquire();
}


bool Input::_init() {	
	if (FAILED(DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&di, NULL))) return false;

	if (FAILED(di->CreateDevice(GUID_SysKeyboard, &k, NULL))) return false;
	if (FAILED(k->SetDataFormat(&c_dfDIKeyboard))) return false;	
	if (FAILED(k->SetCooperativeLevel(dx->get_wnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE))) return false;
	if (FAILED(k->Acquire())) return false;
	
	if (FAILED(di->CreateDevice(GUID_SysMouse, &m, NULL))) return false;
	if (FAILED(m->SetDataFormat(&c_dfDIMouse))) return false;
	if (FAILED(m->SetCooperativeLevel(dx->get_wnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return false;
	if (FAILED(m->Acquire())) return false;

	return true;
}



bool Input::is_mouse_pressed(const int& _k){
	if (ms.rgbButtons[_k] & 0x80) return true;
	return false;
}


bool Input::is_key_pressed(const int& _k) {
	if (ks[_k] & 0x80) return true;
	return false;
}


float Input::get_mouse_delta_x() {
	return (float)ms.lX;
}

float Input::get_mouse_delta_y() {
	return (float)ms.lY;
}