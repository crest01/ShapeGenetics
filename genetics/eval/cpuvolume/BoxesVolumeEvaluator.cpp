/*
 * VolumeEvaluator.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: Karl Haubenwallner
 */

#include "BoxesVolumeEvaluator.h"
#include "Population_IF.h"
#include "Smoothstep.h"

namespace PGA {
namespace CPU {

float BoxesVolumeEvaluator::calcRating(Genome_IF* genome)
{

	_good_voxels = 0;
	_bad_voxels = 0;
	_overlap_voxels = 0;

	_object_space.reset();

	// this will call the compareShape-Method for each generated box, which will set _good_voxels and _bad_voxels, if _lazy_compare is set
	_grammar->impl()->createGeometry(genome);

	float good_rating = _conf->getGoodWeight() * smoothstep(0.0f, _target_space.getNumSetVoxels(), _good_voxels);
	float bad_rating = _conf->getBadWeight() * smoothstep(0.0f, _target_space.getNumSetVoxels() * 2, _bad_voxels + _overlap_voxels);
	float length_rating = _conf->getLengthWeight() * smoothstep(_genome->getMinimalLength(), _genome->getMaximalLength(), genome->length());

	return good_rating - bad_rating - length_rating;
}

void BoxesVolumeEvaluator::generateVoxels(const int index, std::vector<math::float4>& center)
{
	center.clear();
	_object_space.reset();

	_grammar->impl()->createGeometry(_population->impl()->getGenome(index));

	_object_space.exportVoxels(center);

}



}  // namespace CPU

}  // namespace PGA


