#include "resource_manager.h"


ResourceManagers *managers = 0;


bool ResourceManagers::init() {
	managers = new ResourceManagers();
	if (!managers)
		return false;
	return true;
}

void ResourceManagers::destroy() {
	if (managers) delete managers;
}
