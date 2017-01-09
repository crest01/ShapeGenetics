/*
 * Evaluation.h
 *
 *  Created on: Dec 1, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_EVALUATION_IF_H_
#define GPUPROCGENETICS_SRC_GENETICS_EVALUATION_IF_H_

#include "Genome_IF.h"
#include "SupportedShapes.h"

namespace PGA {

class ProceduralAlgorithm;

class Evaluation_IF {
public:
	virtual void init(ProceduralAlgorithm* base) = 0;

	// Volume-evaluation debug stuff:
	// export the center positions for the calculated voxels of the target model
	virtual void generateTargetVoxels(std::vector<math::float4>& voxels) = 0;

	// generate and export the center positions for the calculated voxels of the individual with the given index in the
	// current active population
	virtual void generateVoxels(const int index, std::vector<math::float4>& voxels) = 0;

	// calculate the rating for the given individual
	virtual float calcRating(Genome_IF* genome) = 0;

	virtual float getLastCalculationTime() = 0;
	virtual float getLastProductionTime() = 0;


};

}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GENETICS_EVALUATION_H_ */
