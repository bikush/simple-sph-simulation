#include "Stage.h"



Stage::Stage()
{
}


Stage::~Stage()
{
}

void Stage::addCamera(const Camera& camera) {
	cameras.push_back(camera);
}

void Stage::addScene(const Scene& scene) {

}