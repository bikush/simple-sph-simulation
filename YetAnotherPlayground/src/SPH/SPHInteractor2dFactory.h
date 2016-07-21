#pragma once
#ifndef SPH_INTERACTOR_2D_FACTORY_H
#define SPH_INTERACTOR_2D_FACTORY_H

#include "SPHInteractor2d.h"
#include "SPHPlaneInteractor2d.h"
#include <string>
#include <memory>
#include "MappedData.h"

class SPHInteractor2dFactory
{
public:
	static std::unique_ptr<SPHInteractor2d> getInteractor( std::string name, MappedData* map )
	{		
		std::string type = map->getData( name, "type" ).getStringData();
		if( type.compare( "plane" ) == 0 )
		{
			glm::vec2 start = map->getData( name, "start" ).getVec2();
			glm::vec2 direction = map->getData( name, "direction" ).getVec2();
			glm::vec2 up = map->getData( name, "up" ).getVec2();
			return std::unique_ptr<SPHInteractor2d> (new SPHPlaneInteractor2d( start, direction, up ));
		}
		return std::unique_ptr<SPHInteractor2d>(nullptr);
	}	
};

#endif