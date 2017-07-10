#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__


#include "common.h"
#include "model.h"
#include "texture.h"
#include "shader.h"


template<typename T>
class ResourceManager {
	struct Node {
		std::string name;
		T* data;
	};

	std::list<Node> r;

public:
	ResourceManager() {}

	~ResourceManager() {
		for (std::list<Node>::iterator i = r.begin(); i != r.end(); i++)
			delete (*i).data;
		r.clear();
	}

	T* find_and_init_f(std::string n) {
		for (std::list<Node>::iterator i = r.begin(); i != r.end(); i++)
		if (n == (*i).name)
			return (*i).data;


		T* e = new T();
		if (!e) return 0;
		Node nn;
		nn.name = n;
		nn.data = e;
		if (!e->init_from_file(n.c_str())) { delete e; return false; }
		r.push_back(nn);
		return e;
	}


	T* find_and_init(std::string n)  {
		for (std::list<Node>::iterator i = r.begin(); i != r.end(); i++)
		if (n == (*i).name)
			return (*i).data;

		T* e = new T();
		if (!e) return 0;
		Node nn;
		nn.name = n;
		nn.data = e;
		if (!e->init()) { delete e; return false; }
		r.push_back(nn);
		return e;
	}

	T* find(std::string n) {
		for (std::list<Node>::iterator i = r.begin(); i != r.end(); i++)
		if (n == (*i).name)
			return (*i).data;

		return 0;
	}


	T* add(T* e, std::string n) {
		Node nn;
		nn.name = n;
		nn.data = e;
		r.push_back(nn);
		return e;
	}

	T* add_and_init(T* e, std::string n) {
		Node nn;
		nn.name = n;
		nn.data = e;
		if (!e->init()) { delete e; return false; }
		r.push_back(nn);
		return e;
	}

	template<typename U>
	T* add_and_init(U* e, std::string n) {
		Node nn;
		nn.name = n;
		nn.data = e;
		if (!e->init()) { delete e; return false; }
		r.push_back(nn);
		return e;
	}

};



class ResourceManagers {
public:
	ResourceManager<Model> models;
	ResourceManager<Texture> textures;
	ResourceManager<Shader> shaders;

	static bool init();
	static void destroy();
};


extern ResourceManagers *managers;



#endif