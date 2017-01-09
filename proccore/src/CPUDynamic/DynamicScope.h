#ifndef CPUDYNAMIC_DYNAMICSCOPE
#define CPUDYNAMIC_DYNAMICSCOPE

#pragma once

#include "scope/Scope.h"
namespace PGG
{
	static const int TableEntryScopeOffset = 0;
	typedef Scope::CoordinateframeScope<int> DynamicScope;
}
#endif