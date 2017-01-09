/*
 * RandomParameter.h
 *
 *  Created on: Nov 6, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_PARAMETER_RANDOMPARAMETER_H_
#define GPUPROCGENETICS_SRC_PARAMETER_RANDOMPARAMETER_H_

#include "math/vector.h"

namespace PGA {

using ::math::float3;

float3 	getRandomValue(const float3& min, const float3& max);
math::float2 getRandomValue(const math::float2& min, const math::float2& max);
float 	getRandomValue(const float& min, const float& max);
float 	getRandomValue01();
int		getRandomValue(const int& min, const int& max);

void reinitMtGenerator();

}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_PARAMETER_RANDOMPARAMETER_H_ */
