#pragma once
#ifndef SPH_INTERACTOR_3D_FACTORY_H
#define SPH_INTERACTOR_3D_FACTORY_H

#include "SPHInteractor3d.h"
#include "SPHPlaneInteractor3d.h"
#include "SPHAABBInteractor3d.h"
#include <string>
#include "MappedData.h"

class SPHInteractor3dFactory
{
public:
	// TODO: this is very wrong
	static SPHInteractor3d* getInteractor(std::string name, MappedData* map )
	{
		SPHInteractor3d* out=0;
		std::string type = map->getData( name, "type" ).getStringData();
		if( type.compare( "plane" ) == 0 )
		{
			vec3f start = map->getData( name, "start" ).getVec3f();
			vec3f up = map->getData( name, "up" ).getVec3f();
			out = new SPHPlaneInteractor3d( start, up );	
		}else
		if( type.compare( "AABB" ) == 0 )
		{
			vec3f min = map->getData( name, "mim" ).getVec3f();
			vec3f max = map->getData( name, "max" ).getVec3f();
			float dampen = map->getData( name, "dampen" ).getFloat();
			out = new SPHAABBInteractor3d( min, max, dampen );	
		}
		return out;
	}	
};

#endif