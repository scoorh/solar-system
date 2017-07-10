#include "include.h"
#include "scene.h"


#include <ctime>


int WINAPI WinMain(HINSTANCE hi, HINSTANCE hpi, PSTR cl, int cs) {
//int main() {
	srand((UINT)time(0));

	bool done = false;

	Timer t;

	DX::init();
	Input::init();
	ResourceManagers::init();
	Camera::init();
	camera->rpos().z = -100.0f;
	camera->set_lens(0.25f);


	managers->shaders.add_and_init(new BasicMeshShader(), "basic_model");
	managers->shaders.add_and_init(new SkyDomeShader(), "sky_dome");
	managers->shaders.add_and_init(new DepthShader(), "depth");



	Scene *scene = new Scene();
	if (!scene) done = true;
	if (!scene->init()) done = true;


	bool tmultpi = false;
	float tmultp = 1.0f;
	
	t.reset();
	MSG msg = { 0 };
	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		input->update();
		t.tick();

	
		if (input->is_key_pressed(DIK_NUMPADPLUS)) {
			if (!tmultpi) {
				tmultpi = true;
				tmultp *= 2.0f;
			}
		}
		else if (input->is_key_pressed(DIK_NUMPADMINUS)) {
			if (!tmultpi) {
				tmultpi = true;
				tmultp /= 2.0f;
			}
		}
		else tmultpi = false;


		float dt = 38 * t.delta_time() * tmultp;
		float camdt = 30 * t.delta_time();

		
		if (msg.message == WM_QUIT)	done = true;
		else if (input->is_key_pressed(DIK_ESCAPE)) done = true;
		else {
			if (input->is_key_pressed(DIK_A)) camera->move_side(-camdt);
			if (input->is_key_pressed(DIK_D)) camera->move_side(camdt);
			if (input->is_key_pressed(DIK_S)) camera->move_forward(-camdt);
			if (input->is_key_pressed(DIK_W)) camera->move_forward(camdt);

			if (input->is_mouse_pressed(0)) {
				camera->rotate_x(0.004f*input->get_mouse_delta_y());
				camera->rotate_y(0.004f*input->get_mouse_delta_x());
			}
				
			camera->rebuild_view();

			scene->update(dt);

			scene->pre_draw();
			dx->begin();					
			scene->draw();				
			dx->end();
		}
	}


	delete scene;

	Camera::destroy();
	ResourceManagers::destroy();
	Input::destroy();
	DX::destroy();

	return 0;
}