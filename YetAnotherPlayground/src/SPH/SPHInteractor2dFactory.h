#pragma once
#ifndef SPH_INTERACTOR_2D_FACTORY_H
#define SPH_INTERACTOR_2D_FACTORY_H

#include "SPHInteractor2d.h"
#include "SPHPlaneInteractor2d.h"
#include <string>
#include "MappedData.h"

class SPHInteractor2dFactory
{
public:
	static SPHInteractor2d* getInteractor( std::string name, MappedData* map )
	{
		SPHInteractor2d* out=0;
		std::string type = map->getData( name, "type" ).getStringData();
		if( type.compare( "plane" ) == 0 )
		{
			glm::vec2 start = map->getData( name, "start" ).getVec2();
			glm::vec2 direction = map->getData( name, "direction" ).getVec2();
			glm::vec2 up = map->getData( name, "up" ).getVec2();
			out = new SPHPlaneInteractor2d( start, direction, up );	
		}
		return out;
	}	
};

#endif