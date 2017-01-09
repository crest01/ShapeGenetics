/*
 * Smoothstep.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: toe
 */

#include "Smoothstep.h"
#include <algorithm>

namespace PGA {

float clamp(float val, float min, float max)
{
	return (std::max(min, std::min(val, max)));
}

float smoothstep(float min, float max, float x)
{
    // Scale, and clamp x to 0..1 range
    x = clamp((x - min)/(max - min), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*x*(x*(x*6 - 15) + 10);
}

} // namespace PGA



