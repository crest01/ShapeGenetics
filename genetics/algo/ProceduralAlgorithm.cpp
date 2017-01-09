/*
 * ProcGenAlgo.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#include "ProceduralAlgorithm.h"
#include "Symbol.h"
#include "GenomeCPU.h"
#include "MutationOperator.h"
#include "CrossoverCPU.h"
#include "SpaceShipGrammar.h"
#include "TargetGrammar.h"
#include "ThreeBranchTreeGrammar_v1.h"
#include "ThreeBranchTreeGrammar_v2.h"
#include "TwoBranchTreeGrammar.h"
#include "CityGrammar.h"
#include "SpaceFillingCity.h"

#ifdef EVAL_GPUVOLUME
	#include "GpuVolumeEvaluator.h"
#endif /* EVAL_GPUVOLUME */

#ifdef EVAL_CPUVOLUME
	#include "GenericVolumeEvaluator.h"
	#include "BoxesVolumeEvaluator.h"
#endif /* EVAL_CPUVOLUME */

#ifdef EVAL_GPUIMAGE
	#include "ImageEvaluator.h"
#endif /* EVAL_GPUIMAGE */

#include "Population.h"
#include "Selection.h"
#include "GeneticAlgorithm.h"
#include "Statistics.h"
#include "GeneticAlgoConfig.h"
#include "Config.h"
#include <sys/stat.h>
#include "GeometryConfigSingleton.h"
#include "MetropolisHastings.h"
#include "MHOperators.h"
#include "MHPopulation.h"
#include "SMC.h"
#include "SMCOperators.h"
#include "SOSMC.h"
#include "SOSMCOperators.h"

namespace PGA {


ProceduralAlgorithm_impl* ProceduralAlgorithm_impl::createFromConfig(const std::string& filename)
{
	struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0) {
		std::stringstream ss;
		ss << "Config file " << filename << " not found!";
		throw std::runtime_error(ss.str());
	}
	std::cout << "using config from file " << filename << std::endl;

	Config config = loadConfig(filename.c_str());
	Config& base = config.loadConfig("Algorithm");
	std::string type = base.loadString("type", "undef");
	ProceduralAlgorithm_impl* impl = nullptr;
	if (type.compare("GeneticAlgorithm") == 0) {
		impl = createGeneticAlgoFromConfig(config);
	}
	else if (type.compare("MetropolisHastings") == 0) {
		impl = createMHAlgoFromConfig(config);
	}
	else if (type.compare("SMC") == 0) {
		impl = createSMCAlgoFromConfig(config);
	}
	else if (type.compare("SOSMC") == 0) {
		impl = createSOSMCAlgoFromConfig(config);
	}
	else {
		std::stringstream ss;
		ss << "Algorithm type " << type << " unknown";
		throw std::invalid_argument(ss.str().c_str());
	}
	impl->_type = type;
	return impl;
}

void ProceduralAlgorithm_impl::init()
{
	if (_type.compare("GeneticAlgorithm") == 0) {
		return initGeneticAlgo();
	}
	else if (_type.compare("MetropolisHastings") == 0) {
		return initMHAlgo();
	}
	else if (_type.compare("SMC") == 0) {
		return initSMCAlgo();
	}
	else if (_type.compare("SOSMC") == 0) {
		return initSOSMCAlgo();
	}
	else {
		std::stringstream ss;
		ss << "Algorithm type " << _type << " unknown";
		throw std::invalid_argument(ss.str().c_str());
	}
}

void ProceduralAlgorithm_impl::initGeneticAlgo()
{

	initCommonAlgo();

	_algo->get<MutationConf*>("mutation")->impl()->init(_algo);
	_algo->get<RecombinationConf*>("recombination")->impl()->init(_algo);
	_algo->get<SelectionConf*>("selection")->first()->init(_algo);
	_algo->get<SelectionConf*>("selection")->second()->init(_algo);
	_algo->get<PopulationConf*>("population")->impl()->init(_algo);
	_algo->get<AlgorithmConf*>("base")->impl()->init(_algo);


}

void ProceduralAlgorithm_impl::initMHAlgo()
{
	initCommonAlgo();

	_algo->get<MutationConf*>("init_mutation")->impl()->init(_algo);
	_algo->get<MutationConf*>("diffusion")->impl()->init(_algo);
	_algo->get<MutationConf*>("jump")->impl()->init(_algo);
	_algo->get<PopulationConf*>("population")->impl()->init(_algo);
	_algo->get<AlgorithmConf*>("base")->impl()->init(_algo);

}

void ProceduralAlgorithm_impl::initSMCAlgo()
{
	initCommonAlgo();
	_algo->get<MutationConf*>("mutation")->impl()->init(_algo);
//	_algo->get<MutationConf*>("init_mutation")->impl()->init(_algo); this is the same as mutation
	_algo->get<SelectionConf*>("resampling")->first()->init(_algo);
	_algo->get<PopulationConf*>("population")->impl()->init(_algo);
	_algo->get<AlgorithmConf*>("base")->impl()->init(_algo);

}

void ProceduralAlgorithm_impl::initSOSMCAlgo()
{
	initCommonAlgo();

	_algo->get<MutationConf*>("mutation")->impl()->init(_algo);
//	_algo->get<MutationConf*>("init_mutation")->impl()->init(_algo); this is the same as mutation
	_algo->get<SelectionConf*>("resampling")->first()->init(_algo);
	_algo->get<PopulationConf*>("population")->impl()->init(_algo);
	_algo->get<AlgorithmConf*>("base")->impl()->init(_algo);

}

void ProceduralAlgorithm_impl::initCommonAlgo()
{
	GeometryConfigSingleton::getInstance()->init(_algo);

	_algo->get<GenomeConf*>("genome")->factory()->init(_algo);
	_algo->get<GrammarConf*>("grammar")->impl()->init(_algo);
	_algo->get<GrammarConf*>("target")->impl()->init(_algo);
	_algo->get<EvaluationConf*>("evaluation")->impl()->init(_algo);

	AlgorithmConf* base = _algo->get<AlgorithmConf*>("base");
	StatisticsConf* stats = _algo->get<StatisticsConf*>("statistics");

	stats->impl()->init(_algo);

//	std::string filename(base->getPrefix() + stats->getStatisticsFilename() + ".csv");
//	std::cout << "Statistics will be written to " << filename << std::endl;
//	stats_file.open(filename, std::ofstream::out);
//
//	stats->impl()->setOutStream(stats_file);
//	stats->impl()->writeHeader(true, false);

	this->_current_index = 0;
	this->_display_target = false;

}

/*
 * create a basic algo: contains the
 * basic data structure (called genome), the grammar configuration,
 * the Target grammar config (if used), the fitness evaluation config
 * and the geometry config
 */
void ProceduralAlgorithm_impl::createBasicAlgoFromConfig(ProceduralAlgorithm* algo, Config& AlgoConfig)
{

	Config& baseConfig = AlgoConfig.loadConfig("BaseConfig");
	std::string baseConfigFile(baseConfig.loadString("file", ""));

	Config* config;
	Config base_config;

	if (!baseConfigFile.empty()) {
		std::cout << "use basic config from file " << baseConfigFile << std::endl;
		base_config = loadConfig(baseConfigFile.c_str());
		config = &base_config;
	}
	else {
		config = &AlgoConfig;
	}

	/***************************************************
	 * Genome Configuration
	 **************************************************/
	GenomeConf *genome_cfg = new GenomeConf;
	algo->add("genome", genome_cfg);

	Config& genome = config->loadConfig("Genome");
	genome_cfg->setName(genome.loadString("name", "undef"));
	genome_cfg->setMinimalLength(genome.loadInt("min_length", 0));
	genome_cfg->setMaximalLength(genome.loadInt("max_length", 0));
	genome_cfg->setOptimalLength(genome.loadInt("optimal_length", 0));
	genome_cfg->setMaximalDepth(genome.loadInt("max_depth", 0));
	genome_cfg->setFactory(new Genome_Factory(algo));

	/***************************************************
	 * Basic Grammar Configuration
	 **************************************************/

	GrammarConf *grammar_cfg = new GrammarConf;
	algo->add("grammar", grammar_cfg);

	Config& Grammar = config->loadConfig("Grammar");
	std::string grammar_name(Grammar.loadString("name", "undef"));
	grammar_cfg->setName(grammar_name);
	grammar_cfg->setNumThreads(Grammar.loadInt("num_threads", 0));

	std::string shape_1_file(Grammar.loadString("shape_1", ""));
	math::float4 c1;
	c1.x = (Grammar.loadFloat("color_1_r", 0.5f));
	c1.y = (Grammar.loadFloat("color_1_g", 0.5f));
	c1.z = (Grammar.loadFloat("color_1_b", 0.5f));
	c1.w = 1.0f;
	if (!shape_1_file.empty()) {
		grammar_cfg->addShapeFile(shape_1_file);
		grammar_cfg->addShapeColor(c1);
	}

	std::string shape_2_file(Grammar.loadString("shape_2", ""));
	math::float4 c2;
	c2.x = (Grammar.loadFloat("color_2_r", 0.5f));
	c2.y = (Grammar.loadFloat("color_2_g", 0.5f));
	c2.z = (Grammar.loadFloat("color_2_b", 0.5f));
	c2.w = 1.0f;
	if (!shape_2_file.empty()) {
		grammar_cfg->addShapeFile(shape_2_file);
		grammar_cfg->addShapeColor(c2);
	}

	std::string shape_3_file(Grammar.loadString("shape_3", ""));
	math::float4 c3;
	c3.x = (Grammar.loadFloat("color_3_r", 0.5f));
	c3.y = (Grammar.loadFloat("color_3_g", 0.5f));
	c3.z = (Grammar.loadFloat("color_3_b", 0.5f));
	c3.w = 1.0f;
	if (!shape_3_file.empty()) {
		grammar_cfg->addShapeFile(shape_3_file);
		grammar_cfg->addShapeColor(c3);
	}

	if (grammar_name.compare("spaceship") == 0) {
		grammar_cfg->setImpl(new SpaceShip::SpaceShipGrammar(algo, grammar_cfg));
		std::cout << "using Spaceship grammar" << std::endl;
	}
	else if (grammar_name.compare("tree_3_branch_v1") == 0){
		grammar_cfg->setImpl(new Tree::ThreeBranchTreeGrammar_v1(algo, grammar_cfg));
		std::cout << "using 3 branch Tree grammar v1" << std::endl;
	}
	else if (grammar_name.compare("tree_3_branch_v2") == 0){
		grammar_cfg->setImpl(new Tree::ThreeBranchTreeGrammar_v2(algo, grammar_cfg));
		std::cout << "using 3 branch Tree grammar v2" << std::endl;
	}
	else if (grammar_name.compare("tree_2_branch") == 0){
		grammar_cfg->setImpl(new Tree::TwoBranchTreeGrammar(algo, grammar_cfg));
		std::cout << "using 2 branch Tree grammar" << std::endl;
	}
	else if (grammar_name.compare("city") == 0)
	{
		grammar_cfg->setImpl(new City::CityGrammar(algo, grammar_cfg, false));
		std::cout << "using City grammar" << std::endl;
	}
	else if (grammar_name.compare("city_sep") == 0)
	{
		grammar_cfg->setImpl(new City::CityGrammar(algo, grammar_cfg, true));
		std::cout << "using City grammar" << std::endl;
	}
	else if (grammar_name.compare("city_filling") == 0)
	{
		grammar_cfg->setImpl(new City::SpaceFillingCityGrammar(algo, grammar_cfg));
		std::cout << "using  SpaceFillingCity grammar" << std::endl;
	}

	else {
		throw std::invalid_argument("Invalid Grammar");
	}

	/***************************************************
	 * Grammar configuration for the Target object
	 **************************************************/

	GrammarConf *target_cfg = new GrammarConf;
	algo->add("target", target_cfg);

	Config& targetGrammar = config->loadConfig("TargetGrammar");
	std::string target_grammar_name(targetGrammar.loadString("name", ""));
	if (!target_grammar_name.empty()) {
		target_cfg->setName(targetGrammar.loadString("name", "undef"));
		target_cfg->setNumThreads(targetGrammar.loadInt("num_threads", 0));
		target_cfg->addShapeFile(targetGrammar.loadString("shape_1", ""));
		math::float4 t_c1;
		t_c1.x = (Grammar.loadFloat("color_1_r", 0.5f));
		t_c1.y = (Grammar.loadFloat("color_1_g", 0.5f));
		t_c1.z = (Grammar.loadFloat("color_1_b", 0.5f));
		t_c1.w = 1.0f;
		target_cfg->addShapeColor(t_c1);
	}
	target_cfg->setImpl(new SpaceShip::TargetGrammar(algo, target_cfg));
	/***************************************************
	 * Fitness evaluation configuration
	 **************************************************/

	Config& fitness = config->loadConfig("Fitness");
	std::string eval_config_name(fitness.loadString("name", "undef"));

	Config& eval_config = fitness.loadConfig(eval_config_name.c_str());

	std::string eval_method = eval_config.loadString("method", "undef");

	// voxel evaluation
	if (eval_method.compare("GPU_voxel_evaluation") == 0 ||
			eval_method.compare("CPU_boxes_evaluation") == 0 ||
			eval_method.compare("CPU_voxel_evaluation") == 0) {
		VolumeEvaluationConf* eval = new VolumeEvaluationConf(eval_method);
		algo->add("evaluation", eval);

		float3 bb_min;
		bb_min.x = eval_config.loadFloat("bb_min_x", 0.0f);
		bb_min.y = eval_config.loadFloat("bb_min_y", 0.0f);
		bb_min.z = eval_config.loadFloat("bb_min_z", 0.0f);
		eval->setBBmin(bb_min);

		float3 bb_max;
		bb_max.x = eval_config.loadFloat("bb_max_x", 0.0f);
		bb_max.y = eval_config.loadFloat("bb_max_y", 0.0f);
		bb_max.z = eval_config.loadFloat("bb_max_z", 0.0f);
		eval->setBBMax(bb_max);

		math::int3 voxels;
		voxels.x = eval_config.loadInt("voxels_x", 0);
		voxels.y = eval_config.loadInt("voxels_y", 0);
		voxels.z = eval_config.loadInt("voxels_z", 0);
		eval->setVoxels(voxels);

		eval->setTargetString(eval_config.loadString("target", ""));

		eval->setGoodWeight(eval_config.loadFloat("good_weight", 0.0f));
		eval->setBadWeight(eval_config.loadFloat("bad_weight", 0.0f));
		eval->setLengthWeight(eval_config.loadFloat("length_weight", 0.0f));
		eval->setSeparateOverlap(eval_config.loadInt("separate_overlap", 0));
		eval->setOverlapWeight(eval_config.loadFloat("overlap_weight", 0.0f));

		if (eval_method.compare("GPU_voxel_evaluation") == 0) {
#ifdef EVAL_GPUVOLUME
			eval->setImpl(new GPU::GpuVolumeEvaluator(algo));
#else
			throw std::invalid_argument("GPU volume evaluator not installed");
#endif /* EVAL_GPUVOLUME */
		}
		else if (eval_method.compare("CPU_boxes_evaluation") == 0) {
#ifdef EVAL_CPUVOLUME
			eval->setImpl(new CPU::BoxesVolumeEvaluator(algo));
#else
			throw std::invalid_argument("CPU boxes volume evaluator not installed");
#endif /* EVAL_CPUVOLUME */
		}
		else if (eval_method.compare("CPU_voxel_evaluation") == 0) {
#ifdef EVAL_CPUVOLUME
			eval->setImpl(new CPU::GenericVolumeEvaluator(algo));
#else
			throw std::invalid_argument("CPU volume evaluator not installed");
#endif /* EVAL_CPUVOLUME */
		}
		else {
			throw std::invalid_argument("Invalid volumetric evaluation method");
		}
	}
	// image evaluation
	else if (eval_method.compare("image_evaluation") == 0) {
		ImageEvaluationConf* eval = new ImageEvaluationConf(eval_method);
		algo->add("evaluation", eval);

		float3 bb_min;
		bb_min.x = eval_config.loadFloat("bb_min_x", 0.0f);
		bb_min.y = eval_config.loadFloat("bb_min_y", 0.0f);
		bb_min.z = eval_config.loadFloat("bb_min_z", 0.0f);
		eval->setBBmin(bb_min);

		float3 bb_max;
		bb_max.x = eval_config.loadFloat("bb_max_x", 0.0f);
		bb_max.y = eval_config.loadFloat("bb_max_y", 0.0f);
		bb_max.z = eval_config.loadFloat("bb_max_z", 0.0f);
		eval->setBBMax(bb_max);

		math::int2 pixel;
		pixel.x = eval_config.loadInt("pixels_x", 0);
		pixel.y = eval_config.loadInt("pixels_y", 0);
		eval->setPixels(pixel);

		eval->setTargetFilename(eval_config.loadString("target_filename", ""));

		eval->setGoodWeight(eval_config.loadFloat("good_weight", 0.0f));
		eval->setBadWeight(eval_config.loadFloat("bad_weight", 0.0f));
		eval->setLengthWeight(eval_config.loadFloat("length_weight", 0.0f));

		Config& camera = eval_config.loadConfig("Camera");
		float3 lookat;
		float fov = camera.loadFloat("fov", 0.0f) * math::constants<float>::pi() / 180.0f;
		float znear = camera.loadFloat("znear", 0.0f);
		float zfar = camera.loadFloat("zfar", 0.0f);
		float phi = camera.loadFloat("phi", 0.0f);
		float theta = camera.loadFloat("theta", 0.0f);
		float radius = camera.loadFloat("radius", 0.0f);
		lookat.x = camera.loadFloat("lookat_x", 0.0f);
		lookat.y = camera.loadFloat("lookat_y", 0.0f);
		lookat.z = camera.loadFloat("lookat_z", 0.0f);

		eval->setCameraParameters(fov, znear, zfar, phi, theta, radius, lookat);

#ifdef EVAL_GPUIMAGE
		eval->setImpl(new GPU::ImageEvaluator(algo));
#else
		throw std::invalid_argument("Image evaluation method not installed");
#endif /* EVAL_GPUIMAGE */

	}
	else {
		throw std::invalid_argument("Invalid evaluation method");
	}

	/***************************************************
	 * Geometry configuration
	 **************************************************/
	GeometryConf *geometry_cfg = new GeometryConf;
	algo->add("geometry", geometry_cfg);

	Config& geometry = config->loadConfig("Geometry");
	geometry_cfg->setName(geometry.loadString("name", "undef"));
	geometry_cfg->setInstancedObjectMeshBuffer(new GeometryBufferInstanced);
	geometry_cfg->setInstancedTargetMeshBuffer(new GeometryBufferInstanced);

	/***************************************************
	 * Statistics Configuration
	 **************************************************/
	StatisticsConf *statistics_cfg = new StatisticsConf;
	algo->add("statistics", statistics_cfg);

	Config& statistics = config->loadConfig("Statistics");
	statistics_cfg->setReruns(statistics.loadInt("num_runs", 0));
	statistics_cfg->setPrintStats((statistics.loadInt("print_stats", 0) != 0));
	statistics_cfg->setStatisticsFilename(statistics.loadString("statistics_out", "stats.csv"));
	statistics_cfg->setPerformanceStatisticsFilename(statistics.loadString("performance_statistics_out", "performance_stats.csv"));
	statistics_cfg->setSaveFinalObj(statistics.loadInt("obj_save_final", 1));
	statistics_cfg->setSaveFinalObj_detailed(statistics.loadInt("obj_save_final_detailed", 1));
	statistics_cfg->setSaveEveryObj(statistics.loadInt("obj_save_all", 0));
	statistics_cfg->setSaveEveryObj_detailed(statistics.loadInt("obj_save_all_detailed", 0));
	statistics_cfg->setObjPrefix(statistics.loadString("prefix", "result_"));
	statistics_cfg->setImpl(new Statistics(algo));


}


ProceduralAlgorithm_impl* ProceduralAlgorithm_impl::createGeneticAlgoFromConfig(Config& config)
{


	ProceduralAlgorithm_impl* impl = new ProceduralAlgorithm_impl;

	impl->_algo = new ProceduralAlgorithm;
	ProceduralAlgorithm* algo = impl->_algo;

	createBasicAlgoFromConfig(algo, config);

	/***************************************************
	 * Mutation config
	 **************************************************/
	MutationConf *mutation_cfg = new MutationConf;
	algo->add("mutation", mutation_cfg);
	algo->add("init_mutation", mutation_cfg);

	Config& mutation = config.loadConfig("Mutation");
	mutation_cfg->setName(mutation.loadString("name", "undef"));
	mutation_cfg->setMutationProb(mutation.loadFloat("basic_prob", 0.0f));
	mutation_cfg->setSymbolGrowProb(mutation.loadFloat("grow_prob", 0.0f));
	mutation_cfg->setSymbolCutProb(mutation.loadFloat("cut_prob", 0.0f));
	mutation_cfg->setParameterMutationProb(mutation.loadFloat("permutate_prob", 0.0f));
	mutation_cfg->setImpl(new MutationOperator(algo));

	/***************************************************
	 * Crossover (recombination) config
	 **************************************************/
	RecombinationConf *recomb_cfg = new RecombinationConf;
	algo->add("recombination", recomb_cfg);

	Config& recomb = config.loadConfig("Recombination");
	recomb_cfg->setName(recomb.loadString("name", "undef"));
	recomb_cfg->setReplaceProb(recomb.loadFloat("replace_prob", 0.0f));
	recomb_cfg->setImpl(new Crossover_CPU(algo));

	/***************************************************
	 * Selection config
	 **************************************************/
	SelectionConf *selection_cfg = new SelectionConf;
	algo->add("selection", selection_cfg);

	Config& selection = config.loadConfig("Selection");
	std::string first(selection.loadString("first", "Random"));
	if (first.compare("Roulette") == 0) {
		selection_cfg->setFirst(new RouletteWheelSelector(algo));
	}
	else if (first.compare("Tournament") == 0) {
		selection_cfg->setFirst(new TournamentSelector(algo));
	}
	else if (first.compare("Random") == 0) {
		selection_cfg->setFirst(new RandomSelector(algo));
	}
	else {
		throw std::invalid_argument("Invalid Selection method");
	}

	std::string second(selection.loadString("second", "Random"));

	if (second.compare("Roulette") == 0) {
		selection_cfg->setSecond(new RouletteWheelSelector(algo));
	}
	else if (second.compare("Tournament") == 0) {
		selection_cfg->setSecond(new TournamentSelector(algo));
	}
	else if (second.compare("Random") == 0) {
		selection_cfg->setSecond(new RandomSelector(algo));
	}
	else {
		throw std::invalid_argument("Invalid Selection method");
	}

	selection_cfg->setTournamentSize(selection.loadInt("tournament_size", 0));



	/***************************************************
	 * Population config
	 **************************************************/

	PopulationConf *population_cfg = new PopulationConf;
	algo->add("population", population_cfg);

	Config& population = config.loadConfig("Population");
	population_cfg->setName(population.loadString("name", "undef"));
	population_cfg->setImpl(new Population(algo));

	/***************************************************
	 * General algorithm config
	 **************************************************/

	AlgorithmConf *base_cfg = new AlgorithmConf;
	algo->add("base", base_cfg);
	Config& base = config.loadConfig("Algorithm");
	base_cfg->setName(base.loadString("name", "undef"));
	base_cfg->setName(base.loadString("name", "undef"));
	base_cfg->setName(base.loadString("name", "undef"));
	base_cfg->setPopulationSize(base.loadInt("population_size", 0));
	base_cfg->setInitGenomeLength(base.loadInt("initial_genome_length", 0));
	base_cfg->setMaxGenerations(base.loadInt("max_generations", 0));
	base_cfg->setElitism(base.loadInt("elitism", 0));
	base_cfg->setImpl(new GeneticAlgorithm(algo));
	base_cfg->setPrefix(base.loadString("prefix", "ga_"));
	base_cfg->setUseMaxRuntime(base.loadInt("use_max_runtime", 0));
	base_cfg->setMaxRuntime(base.loadFloat("max_runtime", 0.0f));

	return impl;

}

ProceduralAlgorithm_impl* ProceduralAlgorithm_impl::createMHAlgoFromConfig(Config& config)
{

	ProceduralAlgorithm_impl* impl = new ProceduralAlgorithm_impl;
	impl->_algo = new ProceduralAlgorithm;
	ProceduralAlgorithm* algo = impl->_algo;

	createBasicAlgoFromConfig(algo, config);

	/***************************************************
	 * Diffusion move config
	 **************************************************/

	MutationConf *diffusion_cfg = new MutationConf;
	algo->add("diffusion", diffusion_cfg);

	Config& mutation = config.loadConfig("Mutation");
	diffusion_cfg->setName(mutation.loadString("name", "undef"));
	diffusion_cfg->setMutationProb(mutation.loadFloat("basic_prob", 0.0f));
	diffusion_cfg->setImpl(new DiffusionMove(algo));

	/***************************************************
	 * Jump move config
	 **************************************************/

	MutationConf *jump_cfg = new MutationConf;
	algo->add("jump", jump_cfg);

	jump_cfg->setName(mutation.loadString("name", "undef"));
	jump_cfg->setMutationProb(mutation.loadFloat("basic_prob", 0.0f));
	jump_cfg->setSymbolGrowProb(mutation.loadFloat("grow_prob", 0.0f));
	jump_cfg->setSymbolCutProb(mutation.loadFloat("cut_prob", 0.0f));
	jump_cfg->setImpl(new JumpMove(algo));

	/***************************************************
	 * Config to create initial population
	 **************************************************/

	MutationConf *grow_cfg = new MutationConf;
	algo->add("init_mutation", grow_cfg);
	grow_cfg->setImpl(new GrowMutation(algo));


	/***************************************************
	 * Population config
	 **************************************************/
	PopulationConf *population_cfg = new PopulationConf;
	algo->add("population", population_cfg);

	Config& population = config.loadConfig("Population");
	population_cfg->setName(population.loadString("name", "undef"));
	population_cfg->setImpl(new MHPopulation(algo));

	/***************************************************
	 * Basic algorithm config
	 **************************************************/

	AlgorithmConf *base_cfg = new AlgorithmConf;
	algo->add("base", base_cfg);
	Config& base = config.loadConfig("Algorithm");
	base_cfg->setName(base.loadString("name", "undef"));
	base_cfg->setPopulationSize(base.loadInt("population_size", 0));
	base_cfg->setInitGenomeLength(base.loadInt("initial_genome_length", 0));
	base_cfg->setMaxGenerations(base.loadInt("max_generations", 0));
	base_cfg->setElitism(base.loadInt("elitism", 0));
	base_cfg->setPrefix(base.loadString("prefix", "mh_"));
	base_cfg->setImpl(new MHAlgorithm(algo));
	base_cfg->setUseMaxRuntime(base.loadInt("use_max_runtime", 0));
	base_cfg->setMaxRuntime(base.loadFloat("max_runtime", 0.0f));


	return impl;

}

ProceduralAlgorithm_impl* ProceduralAlgorithm_impl::createSMCAlgoFromConfig(Config& config)
{

	ProceduralAlgorithm_impl* impl = new ProceduralAlgorithm_impl;
	impl->_algo = new ProceduralAlgorithm;
	ProceduralAlgorithm* algo = impl->_algo;

	createBasicAlgoFromConfig(algo, config);

	/***************************************************
	 * Mutation: add a single element to any valid element
	 **************************************************/
	MutationConf *mutation_cfg = new MutationConf;
	algo->add("mutation", mutation_cfg);

	Config& mutation = config.loadConfig("Mutation");
	mutation_cfg->setName(mutation.loadString("name", "undef"));
	mutation_cfg->setMutationProb(mutation.loadFloat("basic_prob", 0.0f));
	mutation_cfg->setImpl(new SMCGrowMutation(algo));

	// initial mutation: also add a single element -> re-use the mutation config
	algo->add("init_mutation", mutation_cfg);

	/***************************************************
	 * Population Config
	 **************************************************/
	// population: usually one or two sorted vectors
	PopulationConf *population_cfg = new PopulationConf;
	algo->add("population", population_cfg);

	Config& population = config.loadConfig("Population");
	population_cfg->setName(population.loadString("name", "undef"));
	population_cfg->setImpl(new Population(algo));

	/***************************************************
	 * Resampling Config
	 **************************************************/
	SelectionConf* resampling = new SelectionConf;
	resampling->setName("Roulette Wheel resampling");
	resampling->setFirst(new SMCSelection(algo));
	algo->add("resampling", resampling);

	/***************************************************
	 * basic Algo config
	 **************************************************/
	AlgorithmConf *base_cfg = new AlgorithmConf;
	algo->add("base", base_cfg);
	Config& base = config.loadConfig("Algorithm");
	base_cfg->setName(base.loadString("name", "undef"));
	base_cfg->setPopulationSize(base.loadInt("population_size", 0));
	// start with a single element
	base_cfg->setInitGenomeLength(1);
	// no elitism
	base_cfg->setElitism(0);
	base_cfg->setMaxGenerations(base.loadInt("max_generations", 0));

	base_cfg->setPrefix(base.loadString("prefix", "smc_"));

	// use the SMC - algorithm implementation
	base_cfg->setImpl(new SMCAlgorithm(algo));
	base_cfg->setUseMaxRuntime(base.loadInt("use_max_runtime", 0));
	base_cfg->setMaxRuntime(base.loadFloat("max_runtime", 0.0f));


	return impl;

}

ProceduralAlgorithm_impl* ProceduralAlgorithm_impl::createSOSMCAlgoFromConfig(Config& config)
{

	ProceduralAlgorithm_impl* impl = new ProceduralAlgorithm_impl;
	impl->_algo = new ProceduralAlgorithm;
	ProceduralAlgorithm* algo = impl->_algo;

	createBasicAlgoFromConfig(algo, config);

	/***************************************************
	 * Mutation: add a single element to the last changed element
	 **************************************************/
	MutationConf *mutation_cfg = new MutationConf;
	algo->add("mutation", mutation_cfg);

	Config& mutation = config.loadConfig("Mutation");
	mutation_cfg->setName(mutation.loadString("name", "undef"));
	mutation_cfg->setMutationProb(mutation.loadFloat("basic_prob", 0.0f));
	mutation_cfg->setImpl(new SoSmcGrowMutation(algo));

	// initial mutation: also add a single element -> re-use the mutation config
	algo->add("init_mutation", mutation_cfg);

	/***************************************************
	 * Population conf
	 **************************************************/
	PopulationConf *population_cfg = new PopulationConf;
	algo->add("population", population_cfg);

	Config& population = config.loadConfig("Population");
	population_cfg->setName(population.loadString("name", "undef"));
	population_cfg->setImpl(new Population(algo));

	/***************************************************
	 * Resampling config
	 **************************************************/
	SelectionConf* resampling = new SelectionConf;
	resampling->setName("Roulette Wheel resampling");
	resampling->setFirst(new SoSmcSelection(algo));
	algo->add("resampling", resampling);

	/***************************************************
	 * basic algo config
	 **************************************************/
	AlgorithmConf *base_cfg = new AlgorithmConf;
	algo->add("base", base_cfg);
	Config& base = config.loadConfig("Algorithm");
	base_cfg->setName(base.loadString("name", "undef"));
	base_cfg->setPopulationSize(base.loadInt("population_size", 0));
	// start with a single element
	base_cfg->setInitGenomeLength(1);
	// no elitism
	base_cfg->setElitism(0);
	base_cfg->setMaxGenerations(base.loadInt("max_generations", 0));

	base_cfg->setPrefix(base.loadString("prefix", "sosmc_"));

	// use the SMC - algorithm implementation
	base_cfg->setImpl(new SOSMCAlgorithm(algo));
	base_cfg->setUseMaxRuntime(base.loadInt("use_max_runtime", 0));
	base_cfg->setMaxRuntime(base.loadFloat("max_runtime", 0.0f));


	return impl;

}

}; // namespace PGA
