#include "scene.h"


Scene::Scene()
: skydome(0), basicmodelsh(0),
sun(0)
{}


Scene::~Scene(){
	for (std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
		delete (*i);
	}
	objects.clear();

	delete sun;
}


bool Scene::init() {
	init_objects(&objects, "DATA/planets");
	init_objects(&objects, "DATA/rings");

	objects[8]->set_move_around(D3DXVECTOR3(0, 0, 0), objects[5]);
	objects[9]->set_move_around(D3DXVECTOR3(0, 0, 0), objects[6]);

	if (!init_asteroid_pass()) return false;
	if (!init_moons()) return false;	


	skydome = new SkyDome;
	if (!skydome) return false;
	if (!skydome->init()) return false;

	sun = new Sun;
	if (!sun) return false;
	if (!sun->init()) return false;

	basicmodelsh = managers->shaders.find("basic_model");
	
	return true;
}


void Scene::update(float dt) {
	for (std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++){
		(*i)->update(dt);
	}
}


void Scene::pre_draw() {
	sun->begin_scene();
	skydome->draw();
	basicmodelsh->use();
	for (std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
		(*i)->draw();
	}
	sun->end_scene();


	sun->begin_depth();
	for (std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
		(*i)->draw();
	}
	sun->end_depth();

	sun->pre_draw();
}


void Scene::draw() {
	sun->draw();
}


bool Scene::init_moons() {
	int beg = objects.size();
	init_objects(&objects, "DATA/moons");


	objects[beg]->set_move_around(D3DXVECTOR3(5, 2, 5), objects[2]);
	

	return true;
}


bool Scene::init_asteroid_pass() {
	Object* ob;

	for (int i = 0; i < 72; i++) {
		ob = new Object();
		if (!ob) return false;

		objects.push_back(ob);

		ob->set_move_currangle(DEG_TO_RAD(i*5));
		ob->set_move_data(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(130, 0, 130));
		ob->set_move_time(0.01f);
		ob->set_model("DATA/MODEL/asteroidzone_m1");
		ob->set_diffuse_text("DATA/TEXTURE/moon.jpg");

		int rx = rand() % 2;
		float fx = (float)(rand()) / (float)RAND_MAX;
		int ry = rand() % 2;
		float fy = (float)(rand()) / (float)RAND_MAX;
		
		ob->set_rotation((float)rx + fx, (float)ry + fy, 0);
	}

	return true;
}


void Scene::init_objects(std::vector<Object*>* o, char *fn) {
	std::fstream f;
	f.open(fn, std::ios_base::in);
	if (!f.is_open()) return;

	Object* ob;
	int c;
	f >> c;

	D3DXVECTOR3 v;
	D3DXVECTOR3 v2;
	std::string s;
	float fl;
	for (int i = 0; i < c; i++){
		ob = new Object();
		if (!ob) return;

		f >> v.x >> v.y >> v.z;
		ob->set_translation(v.x, v.y, v.z);
		f >> v.x >> v.y >> v.z;
		ob->set_rotation(DEG_TO_RAD(v.x), DEG_TO_RAD(v.y), DEG_TO_RAD(v.z));
		f >> v.x >> v.y >> v.z;
		ob->set_scale(v.x, v.y, v.z);
		f >> v.x >> v.y >> v.z;
		ob->set_spin(v.x, v.y, v.z);
		f >> v.x >> v.y >> v.z;
		f >> v2.x >> v2.y >> v2.z;
		ob->set_move_data(v, v2);
		f >> fl;
		ob->set_move_currangle(DEG_TO_RAD(fl));
		f >> fl;
		ob->set_move_time(fl);

		f >> s;
		ob->set_model(s.c_str());
		f >> s;
		ob->set_diffuse_text(s.c_str());
		f >> s;
		ob->set_second_text(s.c_str());

		o->push_back(ob);
	}
}

