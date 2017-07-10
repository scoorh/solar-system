#ifndef __INPUT_H__
#define __INPUT_H__


#include "common.h"
#include "dx.h"


class Input {
	IDirectInput8* di;
	IDirectInputDevice8* k;
	IDirectInputDevice8* m;

	unsigned char ks[256];
	DIMOUSESTATE ms;		

	Input();
	~Input();

	bool _init();

public:
	void update();

	bool is_key_pressed(const int& _k);
	bool is_mouse_pressed(const int& _k);
	float get_mouse_delta_x();
	float get_mouse_delta_y();


	static bool init();
	static void destroy();
};


extern Input *input;


#endif