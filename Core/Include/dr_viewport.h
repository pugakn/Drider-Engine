#pragma once
#include "dr_core_prerequisites.h"

namespace driderSDK {

struct DR_CORE_EXPORT Viewport
{
	float topLeftX;
	float topLeftY;
	UInt32 width;
	UInt32 height;
	float minDepth;
	float maxDepth;
};

}
