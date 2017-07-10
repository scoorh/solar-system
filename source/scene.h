#ifndef __SCENE_H__
#define __SCENE_H__


#include "include.h"


class Scene {
	std::vector<Object*> objects;
	SkyDome *skydome;
	Sun *sun;

	Shader* basicmodelsh;
	

	void init_objects(std::vector<Object*>* o, char *fn);

	bool init_asteroid_pass();
	bool init_moons();
	void draw_db();

public:
	Scene();
	~Scene();

	bool init();

	void update(float dt);
	
	void draw();
	void pre_draw();
};




#endif