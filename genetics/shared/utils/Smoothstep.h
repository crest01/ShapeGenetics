/*
 * Smoothstep.h
 *
 *  Created on: Dec 16, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_UTILS_SMOOTHSTEP_H_
#define GPUPROCGENETICS_SRC_UTILS_SMOOTHSTEP_H_

namespace PGA {

float clamp(float val, float min, float max);
float smoothstep(float min, float max, float x);

}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_UTILS_SMOOTHSTEP_H_ */
