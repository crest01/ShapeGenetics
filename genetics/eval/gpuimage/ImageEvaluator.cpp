/*
 * VolumeEvaluator.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: Karl Haubenwallner
 */

#include "ImageEvaluator.h"
#include "ToTextureRendererInstanced.h"
#include "Smoothstep.h"
#include "Grammar_IF.h"
#include "GeometryBufferInstanced.h"

namespace PGA {
namespace GPU {


ImageEvaluator::ImageEvaluator(ProceduralAlgorithm* base) :
	_good_pixels(0),
	_bad_pixels(0),
	_algo(base),
	_renderer(nullptr),
	_target_initialized(false),
	_calc_time(0),
	_prod_time(0)
{ }


ImageEvaluator::~ImageEvaluator()
{
	if (_renderer)
		delete _renderer;
}


void ImageEvaluator::init(ProceduralAlgorithm* base)
{
	_conf = _algo->get<ImageEvaluationConf*>("evaluation");
	_base = _algo->get<AlgorithmConf*>("base");
	_grammar = _algo->get<GrammarConf*>("grammar");
	_genome = _algo->get<GenomeConf*>("genome");
	_geometry = _algo->get<GeometryConf*>("geometry");
	_population = _algo->get<PopulationConf*>("population");


	_renderer = new ToTextureRendererInstanced(_conf->getTargetFilename(), _conf->getPixels().x, _conf->getPixels().y);

	float fov, znear, zfar, phi, theta, radius;
	math::float3 lookat;

	_conf->getCameraParameters(fov, znear, zfar, phi, theta, radius, lookat);

	_renderer->setupCamera(fov, znear, zfar, phi, theta, radius, lookat);

	std::cout << "Image based Evaluation: " << std::endl;
	std::cout << "\tgood weight: " << _conf->getGoodWeight() << std::endl;
	std::cout << "\tbad weight: " << _conf->getBadWeight() << std::endl;
	std::cout << "\tlength weight: " << _conf->getLengthWeight() << std::endl;
	std::cout << "\ttarget pixels: " << _renderer->getTargetPixels() << std::endl;

}


float ImageEvaluator::calcRating(Genome_IF* genome)
{

	//std::cout << "calc rating, current " << genome->getEvalPoints();

	auto t0 = std::chrono::high_resolution_clock::now();

//	if (_renderer) {
//		delete _renderer;
//	}
//
//	_renderer = new ToTextureRenderer(_conf->getTargetFilename(), _conf->getPixels().x, _conf->getPixels().y);

	float fov, znear, zfar, phi, theta, radius;
	math::float3 lookat;

	_conf->getCameraParameters(fov, znear, zfar, phi, theta, radius, lookat);

	_renderer->setupCamera(fov, znear, zfar, phi, theta, radius, lookat);

	_grammar->impl()->createGeometry(genome);

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
	_prod_time = time_span.count();

	int good_pixels = 0;
	int bad_pixels = 0;
	int target_pixels = _renderer->getTargetPixels();

	_renderer->compare(_geometry->getInstancedObjectMeshBuffer(), good_pixels, bad_pixels, _base->getCurrentGeneration());


	float good_rating = _conf->getGoodWeight() * smoothstep(0.0f, target_pixels, good_pixels);
	float bad_rating = _conf->getBadWeight() * smoothstep(0.0f, target_pixels, bad_pixels);
	float length_rating = _conf->getLengthWeight() * smoothstep(_genome->getMinimalLength(), _genome->getMaximalLength(), genome->length());

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span_2 = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	_calc_time = time_span_2.count();

	float rating = good_rating - bad_rating - length_rating;

//	std::cout << "rating: " << rating << " (good: " << good_pixels << ", bad: " << bad_pixels << ", length: "<< genome->length() <<", target pixels: " << target_pixels << ")" << std::endl;

	return rating;
}


}  // namespace CPU

}  // namespace PGA


