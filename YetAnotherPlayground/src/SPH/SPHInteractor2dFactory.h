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
			vec2f start = map->getData( name, "start" ).getVec2f();
			vec2f direction = map->getData( name, "direction" ).getVec2f();
			vec2f up = map->getData( name, "up" ).getVec2f();
			out = new SPHPlaneInteractor2d( start, direction, up );	
		}
		return out;
	}	
};

#endif