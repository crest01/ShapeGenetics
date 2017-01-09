/*
 * GeneticAlgoScene.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#include "GeneticAlgoScene.h"

GeneticAlgoScene::GeneticAlgoScene(const std::string& filename):
	seed(217197128u),
	genetic_algo(PGA::ProceduralAlgorithm_impl::createFromConfig(filename)),
	_current_generation(0)
{
}

double GeneticAlgoScene::recombine(bool restart)
{
	//genetic_algo->reset();
	//genetic_algo->doOneGeneration();
	genetic_algo->run();
	_current_generation ++;
	return 0.0f;
}

double GeneticAlgoScene::generate()
{


	auto t0 = std::chrono::high_resolution_clock::now();

	genetic_algo->generateGeometry();

	auto t1 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);

	return time_span.count();
}

void GeneticAlgoScene::getVoxels(std::vector<math::float4>& centers)
{
	genetic_algo->getVoxels(centers);
}

void GeneticAlgoScene::getGeometry(std::vector<PGA::GeneratedVertex>& geometry, std::vector<unsigned>& indices)
{
}

PGA::GeometryBufferInstanced& GeneticAlgoScene::getGeometryBuffer()
{
	return genetic_algo->getGeometryBufferInstanced();
}


