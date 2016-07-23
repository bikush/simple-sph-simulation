#pragma once
#ifndef SPH_INTERACTOR_3D_FACTORY_H
#define SPH_INTERACTOR_3D_FACTORY_H

#include "SPHInteractor3d.h"
#include "SPHPlaneInteractor3d.h"
#include "SPHAABBInteractor3d.h"
#include <string>
#include <memory>
#include "MappedData.h"

class SPHInteractor3dFactory
{
public:
	static std::unique_ptr< SPHInteractor3d> getInteractor(std::string name, MappedData* map )
	{
		std::string type = map->getData( name, "type" ).getStringData();
		if( type.compare( "plane" ) == 0 )
		{
			glm::vec3 start = map->getData( name, "start" ).getVec3();
			glm::vec3 up = map->getData( name, "up" ).getVec3();
			return std::unique_ptr<SPHInteractor3d>(new SPHPlaneInteractor3d( start, up ));
		}else
		if( type.compare( "AABB" ) == 0 )
		{
			glm::vec3 min = map->getData( name, "mim" ).getVec3();
			glm::vec3 max = map->getData( name, "max" ).getVec3();
			float dampen = map->getData( name, "dampen" ).get<float>();
			return std::unique_ptr<SPHInteractor3d>(new SPHAABBInteractor3d( min, max, dampen ));
		}
		return std::unique_ptr<SPHInteractor3d>(nullptr);
	}	
};

#endif