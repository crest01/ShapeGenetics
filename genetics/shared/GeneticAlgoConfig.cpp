/*
 * GeneticAlgoConfig.cpp
 *
 *  Created on: Feb 19, 2016
 *      Author: toe
 */

#include "GeneticAlgoConfig.h"
#include <iomanip>
#include "Selection_IF.h"

namespace PGA {


ConfigBase::ConfigBase(const std::string& config_name):
			_name(config_name)
{}
void ConfigBase::setName(const std::string& name) {
	_name = name;
}

std::string& ConfigBase::getName() {
	return _name;
}

ReadWriteLock& ConfigBase::getLock() {
	return _lock;
}


GenomeConf::GenomeConf(const std::string& name) :
				ConfigBase(name),
				_minimal_length(0),
				_optimal_length(0),
				_maximal_length(0),
				_genome_factory(nullptr)
{}

void GenomeConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Genome: " << getName() << std::endl;
	out << std::setw(22) << "minimal length: " <<  _minimal_length << std::endl;
	out << std::setw(22) << "optimal length: " <<  _optimal_length << std::endl;
	out << std::setw(22) << "maximal length: " <<  _maximal_length << std::endl;
}

Genome_Factory_IF* GenomeConf::factory() {
	ReadLocker(this->getLock());
	return _genome_factory;
}

void GenomeConf::setFactory(Genome_Factory_IF* factory) {
	WriteLocker(this->getLock());
	_genome_factory = factory;
}
int GenomeConf::getOptimalLength() {
	ReadLocker(this->getLock());
	return _optimal_length;
}
void GenomeConf::setOptimalLength(int val) {
	WriteLocker(this->getLock());
	_optimal_length = val;
}
int GenomeConf::getMinimalLength() {
	ReadLocker(this->getLock());
	return _minimal_length;
}
void GenomeConf::setMinimalLength(int val) {
	WriteLocker(this->getLock());
	_minimal_length = val;
}
int GenomeConf::getMaximalLength() {
	ReadLocker(this->getLock());
	return _maximal_length;
}
void GenomeConf::setMaximalLength(int val) {
	WriteLocker(this->getLock());
	_maximal_length = val;
}

int GenomeConf::getMaximalDepth() {
	ReadLocker(this->getLock());
	return _maximal_depth;
}
void GenomeConf::setMaximalDepth(int val) {
	WriteLocker(this->getLock());
	_maximal_depth = val;
}



GrammarConf::GrammarConf(const std::string& name) :
				ConfigBase(name),
				_sm(),
				_pt(),
				_system(),
				_num_threads(4),
				_grammar(nullptr)
{}

void GrammarConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Grammar: " << getName() << std::endl;
	out << std::setw(22) << "num_threads: " <<  _num_threads << std::endl;
}

Grammar_IF* GrammarConf::impl() {
	ReadLocker(this->getLock());
	return _grammar;
}
void GrammarConf::setImpl(Grammar_IF* impl) {
	WriteLocker(this->getLock());
	_grammar = impl;
}

int GrammarConf::getNumThreads() {
	ReadLocker(this->getLock());
	return _num_threads;
}
void GrammarConf::setNumThreads(const int numThreads) {
	WriteLocker(this->getLock());
	_num_threads = numThreads;
}

PGG::Parameters::ParameterTable& GrammarConf::getPt() {
	ReadLocker(this->getLock());
	return _pt;
}

SymbolManager& GrammarConf::getSm() {
	ReadLocker(this->getLock());
	return _sm;
}

PGG::CPU::GrammarSystem& GrammarConf::getSystem() {
	ReadLocker(this->getLock());
	return _system;
}


void GrammarConf::addShapeFile(const std::string& filename)
{
	WriteLocker(this->getLock());
	_shape_files.push_back(filename);
}
void GrammarConf::addShapeColor(const math::float4& color)
{
	WriteLocker(this->getLock());
	_shape_colors.push_back(color);
}
int GrammarConf::getNumShapes()
{
	ReadLocker(this->getLock());
	return _shape_files.size();
}
const std::string& GrammarConf::getShapeFile(const int index)
{
	ReadLocker(this->getLock());
	return _shape_files[index];
}
const math::float4& GrammarConf::getShapeColor(const int index)
{
	ReadLocker(this->getLock());
	return _shape_colors[index];
}


MutationConf::MutationConf(const std::string& name) :
				ConfigBase(name),
				_mutator(nullptr),
				_mutation_prob(),
				_parameter_mutation_prob(),
				_symbol_cut_prob(),
				_symbol_grow_prob()
{}

void MutationConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Mutation: " << getName() << std::endl;
	out << std::setw(22) << "base chance: " << _mutation_prob << "%" << std::endl;
	out << std::setw(22) << "cut chance: " <<  _symbol_cut_prob << "%" << std::endl;
	out << std::setw(22) << "grow chance: "  << _symbol_grow_prob << "%" << std::endl;
	out << std::setw(22) << "perm. chance: "  << _parameter_mutation_prob << "%" << std::endl;
}


Mutation_IF* MutationConf::impl()
{
	ReadLocker(this->getLock());
	return _mutator;
}

void MutationConf::setImpl(Mutation_IF* impl) {
	WriteLocker(this->getLock());
	_mutator = impl;
}

float MutationConf::getMutationProb() {
	ReadLocker(this->getLock());
	return _mutation_prob;
}

void MutationConf::setMutationProb(float mutationProb) {
	WriteLocker(this->getLock());
	_mutation_prob = mutationProb;
}

float MutationConf::getParameterMutationProb() {
	ReadLocker(this->getLock());
	return _parameter_mutation_prob;
}

void MutationConf::setParameterMutationProb(float parameterMutationProb) {
	WriteLocker(this->getLock());
	_parameter_mutation_prob = parameterMutationProb;
}

float MutationConf::getSymbolCutProb() {
	ReadLocker(this->getLock());
	return _symbol_cut_prob;
}

void MutationConf::setSymbolCutProb(float symbolCutProb) {
	WriteLocker(this->getLock());
	_symbol_cut_prob = symbolCutProb;
}

float MutationConf::getSymbolGrowProb() {
	ReadLocker(this->getLock());
	return _symbol_grow_prob;
}

void MutationConf::setSymbolGrowProb(float symbolGrowProb) {
	WriteLocker(this->getLock());
	_symbol_grow_prob = symbolGrowProb;
}




RecombinationConf::RecombinationConf(const std::string& name) :
				ConfigBase(name),
				_recombinator(nullptr),
				_replace_prob()
{}

void RecombinationConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Recombination: " << getName() << std::endl;
	out << std::setw(22) << "replace prob: " << _replace_prob << "%" << std::endl;
}


float RecombinationConf::getReplaceProb() {
	ReadLocker(this->getLock());
	return _replace_prob;
}

void RecombinationConf::setReplaceProb(float replace_probability) {
	WriteLocker(this->getLock());
	_replace_prob = replace_probability;
}

Recombination_IF* RecombinationConf::impl() {
	ReadLocker(this->getLock());
	return _recombinator;
}

void RecombinationConf::setImpl(Recombination_IF* impl) {
	WriteLocker(this->getLock());
	_recombinator = impl;
}





SelectionConf::SelectionConf(const std::string& name) :
				ConfigBase(name),
				_tournament_size(),
				_first(nullptr),
				_second(nullptr)
{}

void SelectionConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Selection: " << _first->getName() << "/" <<  _second->getName() << std::endl;
	out << std::setw(22) << "Tournament size: " << _tournament_size << std::endl;
}

Selection_IF* SelectionConf::first()  {
	ReadLocker(this->getLock());
	return _first;
}

void SelectionConf::setFirst( Selection_IF* first) {
	WriteLocker(this->getLock());
	_first = first;
}

Selection_IF* SelectionConf::second()  {
	ReadLocker(this->getLock());
	return _second;
}

void SelectionConf::setSecond( Selection_IF* second) {
	WriteLocker(this->getLock());
	_second = second;
}

int SelectionConf::getTournamentSize()  {
	ReadLocker(this->getLock());
	return _tournament_size;
}

void SelectionConf::setTournamentSize(int tournamentSize) {
	WriteLocker(this->getLock());
	_tournament_size = tournamentSize;
}


EvaluationConf::EvaluationConf(const std::string& name) :
				ConfigBase(name),
				_evaluator(nullptr)
{}

void EvaluationConf::setImpl(Evaluation_IF* impl) {
	WriteLocker(this->getLock());
	_evaluator = impl;
}

Evaluation_IF* EvaluationConf::impl() {
	ReadLocker(this->getLock());
	return _evaluator;
}


VolumeEvaluationConf::VolumeEvaluationConf(const std::string& name):
				EvaluationConf(name),
				_target_string(),
				_good_weight(0),
				_bad_weight(0),
				_length_weight(0)
{ }

void VolumeEvaluationConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Fitness calculation: " << getName() << std::endl;
	out << std::setw(22) <<"Bounding Box min: " <<  "("<< _bb_min.x << ", " << _bb_min.y << ", " << _bb_min.z << ")" << std::endl;
	out << std::setw(22) <<"Bounding Box max: " <<  "("<< _bb_max.x << ", " << _bb_max.y << ", " << _bb_max.z << ")" << std::endl;
	out << std::setw(22) <<"Voxels: " <<  "("<< _voxels.x << ", " << _voxels.y << ", " << _voxels.z << ")" << std::endl;
	out << std::setw(22) <<"Good weight: " <<  _good_weight << std::endl;
	out << std::setw(22) <<"Bad weight: " <<  _bad_weight << std::endl;
	out << std::setw(22) <<"Length weight: " << _length_weight << std::endl;
	out << std::setw(22) <<"Target: " << _target_string << std::endl;
}

std::string& VolumeEvaluationConf::getTargetString()  {
	ReadLocker(this->getLock());
	return _target_string;
}

void VolumeEvaluationConf::setTargetString( const std::string& targetString) {
	WriteLocker(this->getLock());
	_target_string = targetString;
}

math::float3& VolumeEvaluationConf::getBBmax()  {
	ReadLocker(this->getLock());
	return _bb_max;
}

void VolumeEvaluationConf::setBBMax(const math::float3& bbMax) {
	WriteLocker(this->getLock());
	_bb_max = bbMax;
}

math::float3& VolumeEvaluationConf::getBBmin()  {
	ReadLocker(this->getLock());
	return _bb_min;
}

void VolumeEvaluationConf::setBBmin(const math::float3& bbMin) {
	WriteLocker(this->getLock());
	_bb_min = bbMin;
}


math::int3& VolumeEvaluationConf::getVoxels()  {
	ReadLocker(this->getLock());
	return _voxels;
}

void VolumeEvaluationConf::setVoxels(const math::int3& voxels) {
	WriteLocker(this->getLock());
	_voxels = voxels;
}

float VolumeEvaluationConf::getBadWeight()  {
	ReadLocker(this->getLock());
	return _bad_weight;
}

void VolumeEvaluationConf::setBadWeight(float badWeight) {
	WriteLocker(this->getLock());
	_bad_weight = badWeight;
}

float VolumeEvaluationConf::getGoodWeight()  {
	ReadLocker(this->getLock());
	return _good_weight;
}

void VolumeEvaluationConf::setGoodWeight(float goodWeight) {
	WriteLocker(this->getLock());
	_good_weight = goodWeight;
}

float VolumeEvaluationConf::getLengthWeight()  {
	ReadLocker(this->getLock());
	return _length_weight;
}

void VolumeEvaluationConf::setLengthWeight(float lengthWeight) {
	WriteLocker(this->getLock());
	_length_weight = lengthWeight;
}

float VolumeEvaluationConf::getOverlapWeight()  {
	ReadLocker(this->getLock());
	return _overlap_weight;
}

void VolumeEvaluationConf::setOverlapWeight(float val) {
	WriteLocker(this->getLock());
	_overlap_weight = val;
}

bool VolumeEvaluationConf::getSeparateOverlap()  {
	ReadLocker(this->getLock());
	return _separate_overlap;
}

void VolumeEvaluationConf::setSeparateOverlap(bool val) {
	WriteLocker(this->getLock());
	_separate_overlap = val;
}


ImageEvaluationConf::ImageEvaluationConf(const std::string& name) :
				EvaluationConf(name),
				_bb_min(),
				_bb_max(),
				_pixels(),
				_good_weight(),
				_bad_weight(),
				_length_weight()
{}

void ImageEvaluationConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Fitness calculation: " << getName() << std::endl;
	out << std::setw(22) << "Pixels: " << "("<< _pixels.x << ", " << _pixels.y << ")" << std::endl;
	out << std::setw(22) << "Camera: " << std::endl;
	out << std::setw(25) << "fov: " << _fov << std::endl;
	out << std::setw(25) << "znear: " << _znear << std::endl;
	out << std::setw(25) << "zfar: " << _zfar << std::endl;
	out << std::setw(22) << "Camera Pos: " << std::endl;
	out << std::setw(25) << "phi: " << _phi << std::endl;
	out << std::setw(25) << "theta: " << _theta << std::endl;
	out << std::setw(25) << "radius: " << _radius << std::endl;
	out << std::setw(25) << "lookat: " << "(" << _lookat.x << ", " << _lookat.y << ", " << _lookat.z << ")" << std::endl;
	out << std::setw(22) << "Good weight: " <<  _good_weight << std::endl;
	out << std::setw(22) << "Bad weight: " <<  _bad_weight << std::endl;
	out << std::setw(22) << "Length weight: " <<  _length_weight << std::endl;
	out << std::setw(22) << "Target file: " <<  _target_filename << std::endl;
}


math::float3& ImageEvaluationConf::getBBmax()  {
	ReadLocker(this->getLock());
	return _bb_max;
}

void ImageEvaluationConf::setBBMax(const math::float3& bbMax) {
	WriteLocker(this->getLock());
	_bb_max = bbMax;
}

math::float3& ImageEvaluationConf::getBBmin()  {
	ReadLocker(this->getLock());
	return _bb_min;
}

void ImageEvaluationConf::setBBmin(const math::float3& bbMin) {
	WriteLocker(this->getLock());
	_bb_min = bbMin;
}

std::string& ImageEvaluationConf::getTargetFilename()  {
	ReadLocker(this->getLock());
	return _target_filename;
}

void ImageEvaluationConf::setTargetFilename( const std::string& targetFilename) {
	WriteLocker(this->getLock());
	_target_filename = targetFilename;
}

math::int2& ImageEvaluationConf::getPixels()  {
	ReadLocker(this->getLock());
	return _pixels;
}

void ImageEvaluationConf::setPixels(const math::int2& pixels) {
	WriteLocker(this->getLock());
	_pixels = pixels;
}

float ImageEvaluationConf::getBadWeight()  {
	ReadLocker(this->getLock());
	return _bad_weight;
}

void ImageEvaluationConf::setBadWeight(float badWeight) {
	WriteLocker(this->getLock());
	_bad_weight = badWeight;
}

float ImageEvaluationConf::getGoodWeight()  {
	ReadLocker(this->getLock());
	return _good_weight;
}

void ImageEvaluationConf::setGoodWeight(float goodWeight) {
	WriteLocker(this->getLock());
	_good_weight = goodWeight;
}

float ImageEvaluationConf::getLengthWeight()  {
	ReadLocker(this->getLock());
	return _length_weight;
}

void ImageEvaluationConf::setLengthWeight(float lengthWeight) {
	WriteLocker(this->getLock());
	_length_weight = lengthWeight;
}

void ImageEvaluationConf::setCameraParameters(float fov, float znear, float zfar, float phi, float theta, float radius, math::float3 lookat)
{
	WriteLocker(this->getLock());
	_fov = fov;
	_znear = znear;
	_zfar = zfar;
	_phi = phi;
	_theta = theta;
	_radius = radius;
	_lookat = lookat;
}

void ImageEvaluationConf::getCameraParameters(float& fov, float& znear, float& zfar, float& phi, float& theta, float& radius, math::float3& lookat)
{
	ReadLocker(this->getLock());
	fov = _fov;
	znear = _znear;
	zfar = _zfar;
	phi = _phi;
	theta = _theta;
	radius = _radius;
	lookat = _lookat;
}



PopulationConf::PopulationConf(const std::string& name) :
				ConfigBase(name),
				_population(nullptr)
{}

void PopulationConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Population: " << getName() << std::endl;
}


Population_IF* PopulationConf::impl() {
	ReadLocker(this->getLock());
	return _population;
}
void PopulationConf::setImpl(Population_IF* impl) {
	WriteLocker(this->getLock());
	_population = impl;
}




GeometryConf::GeometryConf(const std::string& name) :
				ConfigBase(name),
				_generate(false),
				_instanced_mesh_buffer_object(nullptr),
				_instanced_mesh_buffer_target(nullptr),
				_evaluator(nullptr)
{}

void GeometryConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Geometry Buffer: " << getName() << std::endl;
}


Evaluation_IF* GeometryConf::getEvaluator() {
	ReadLocker(this->getLock());
	return _evaluator;
}
void GeometryConf::setEvaluator(Evaluation_IF* impl) {
	WriteLocker(this->getLock());
	_evaluator = impl;
}

void GeometryConf::setUsedMeshBuffer(MeshBufferTarget target) {
	WriteLocker(this->getLock());
	_target = target;
}

GeometryBufferInstanced* GeometryConf::getInstancedMeshBuffer() {
	ReadLocker(this->getLock());
	if (_target == MeshBufferTarget::TARGET) {
		return _instanced_mesh_buffer_target;
	}
	else if (_target == MeshBufferTarget::OBJECT) {
		return _instanced_mesh_buffer_object;
	}
	return nullptr;
}

GeometryBufferInstanced* GeometryConf::getInstancedObjectMeshBuffer() {
	ReadLocker(this->getLock());
	return _instanced_mesh_buffer_object;
}

void GeometryConf::setInstancedObjectMeshBuffer(GeometryBufferInstanced* meshBuffer) {
	WriteLocker(this->getLock());
	_instanced_mesh_buffer_object = meshBuffer;
}

GeometryBufferInstanced* GeometryConf::getInstancedTargetMeshBuffer() {
	ReadLocker(this->getLock());
	return _instanced_mesh_buffer_target;
}

void GeometryConf::setInstancedTargetMeshBuffer(GeometryBufferInstanced* meshBuffer) {
	WriteLocker(this->getLock());
	_instanced_mesh_buffer_target = meshBuffer;
}

bool GeometryConf::generate() {
	ReadLocker(this->getLock());
	return _generate;
}

void GeometryConf::setGenerate(bool value) {
	WriteLocker(this->getLock());
	_generate = value;
}

StatisticsConf::StatisticsConf(const std::string& name) :
				ConfigBase(name),
				_print_stats(false),
				_save_every_obj(false),
				_save_every_obj_detailed(false),
				_save_final_obj(false),
				_save_final_obj_detailed(false),
				_obj_prefix(),
				_stats_filename(),
				_perfstats_filename(""),
				_statistics(nullptr),
				_reruns(1)
{}

int StatisticsConf::getReruns()  {
	ReadLocker(this->getLock());
	return _reruns;
}

void StatisticsConf::setReruns(int val) {
	WriteLocker(this->getLock());
	_reruns = val;
}

Statistics_IF* StatisticsConf::impl() {
	ReadLocker(this->getLock());
	return _statistics;
}

void StatisticsConf::setImpl(Statistics_IF* statistics) {
	WriteLocker(this->getLock());
	_statistics = statistics;
}

bool StatisticsConf::recordStats() {
	ReadLocker(this->getLock());
	if (_statistics != nullptr)
		return true;

	return false;
}

void StatisticsConf::setPrintStats(bool value) {
	WriteLocker(this->getLock());
	_print_stats = value;
}

bool StatisticsConf::printStats() {
	ReadLocker(this->getLock());
	if (_statistics == nullptr)
		return false;

	return _print_stats;
}

void StatisticsConf::setStatisticsFilename(const std::string& value) {
	WriteLocker(this->getLock());
	_stats_filename = value;
}

const std::string& StatisticsConf::getStatisticsFilename() {
	ReadLocker(this->getLock());
	return _stats_filename;
}

void StatisticsConf::setPerformanceStatisticsFilename(const std::string& value) {
	WriteLocker(this->getLock());
	_perfstats_filename = value;
}

const std::string& StatisticsConf::getPerformanceStatisticsFilename() {
	ReadLocker(this->getLock());
	return _perfstats_filename;
}


void StatisticsConf::setSaveEveryObj(bool value)
{
	WriteLocker(this->getLock());
	this->_save_every_obj = value;
}
bool StatisticsConf::saveEveryObj()
{
	ReadLocker(this->getLock());
	return _save_every_obj;
}
void StatisticsConf::setSaveEveryObj_detailed(bool value)
{
	WriteLocker(this->getLock());
	this->_save_every_obj_detailed = value;
}
bool StatisticsConf::saveEveryObj_detailed()
{
	ReadLocker(this->getLock());
	return _save_every_obj_detailed;
}
void StatisticsConf::setSaveFinalObj(bool value)
{
	WriteLocker(this->getLock());
	this->_save_final_obj = value;
}
bool StatisticsConf::saveFinalObj()
{
	ReadLocker(this->getLock());
	return _save_final_obj;
}
void StatisticsConf::setSaveFinalObj_detailed(bool value)
{
	WriteLocker(this->getLock());
	this->_save_final_obj_detailed = value;
}
bool StatisticsConf::saveFinalObj_detailed()
{
	ReadLocker(this->getLock());
	return _save_final_obj_detailed;
}


void StatisticsConf::setObjPrefix(const std::string& value)
{
	WriteLocker(this->getLock());
	this->_obj_prefix = value;
}
const std::string& StatisticsConf::getobjPrefix()
{
	ReadLocker(this->getLock());
	return this->_obj_prefix;
}
void StatisticsConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Statistics: " << getName() << std::endl;
	out << std::setw(22) << "Reruns: " << _reruns << std::endl;
	out << std::setw(22) << "Save Final Object: " <<  _save_final_obj << std::endl;
	out << std::setw(22) << "Save Every Object: " <<  _save_every_obj << std::endl;
}



AlgorithmConf::AlgorithmConf(const std::string& name) :
				ConfigBase(name),
				_population_size(),
				_init_genome_length(),
				_max_generations(),
				_elitism(),
				_algorithm(nullptr)
{}

void AlgorithmConf::toString(std::ostream& out) {
	ReadLocker(this->getLock());
	out << "Basic Conf: " << getName() << std::endl;
	out << std::setw(22) << "Population size: " << _population_size << std::endl;
	out << std::setw(22) << "Initial length: " <<  _init_genome_length << std::endl;
	out << std::setw(22) << "Generations: " <<  _max_generations << std::endl;
	out << std::setw(22) << "Elitism: " <<  _elitism << std::endl;
}


GeneticAlgorithm_IF* AlgorithmConf::impl() {
	ReadLocker(this->getLock());
	return _algorithm;
}

void AlgorithmConf::setImpl(GeneticAlgorithm_IF* impl) {
	WriteLocker(this->getLock());
	_algorithm = impl;
}

int AlgorithmConf::getElitism() {
	ReadLocker(this->getLock());
	return _elitism;
}

void AlgorithmConf::setElitism(int elitism) {
	WriteLocker(this->getLock());
	_elitism = elitism;
}
int AlgorithmConf::getCurrentGeneration() {
	ReadLocker(this->getLock());
	return _current_generation;
}

void AlgorithmConf::setCurrentGeneration(int generation) {
	WriteLocker(this->getLock());
	_current_generation = generation;
}

int AlgorithmConf::getInitGenomeLength()  {
	ReadLocker(this->getLock());
	return _init_genome_length;
}

void AlgorithmConf::setInitGenomeLength(int initGenomeLength) {
	WriteLocker(this->getLock());
	_init_genome_length = initGenomeLength;
}


int AlgorithmConf::getMaxGenerations() {
	ReadLocker(this->getLock());
	return _max_generations;
}

void AlgorithmConf::setMaxGenerations(int maxgeneration) {
	WriteLocker(this->getLock());
	_max_generations = maxgeneration;
}

bool AlgorithmConf::getUseMaxRuntime() {
	ReadLocker(this->getLock());
	return _use_max_runtime;
}

void AlgorithmConf::setUseMaxRuntime(bool value) {
	WriteLocker(this->getLock());
	_use_max_runtime = value;
}

float AlgorithmConf::getMaxRuntime() {
	ReadLocker(this->getLock());
	return _max_runtime;
}

void AlgorithmConf::setMaxRuntime(float value) {
	WriteLocker(this->getLock());
	_max_runtime = value;
}

int AlgorithmConf::getPopulationSize() {
	ReadLocker(this->getLock());
	return _population_size;
}

void AlgorithmConf::setPopulationSize(int populationSize) {
	WriteLocker(this->getLock());
	_population_size = populationSize;
}


void AlgorithmConf::setPrefix(const std::string& value)
{
	WriteLocker(this->getLock());
	this->_prefix = value;
}
const std::string& AlgorithmConf::getPrefix()
{
	ReadLocker(this->getLock());
	return this->_prefix;
}




}  // namespace PGA




