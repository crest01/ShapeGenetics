/*
 * Config.h
 *
 *  Created on: Dec 8, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICALGOCONFIG_H_
#define GPUPROCGENETICS_SRC_GENETICALGOCONFIG_H_

#include "SymbolManager.h"
#include "ParameterTable.h"
#include "GrammarSystem.h"
#include "ReadWriteLock.h"
#include <string>
#include <sstream>
#include <ostream>
#include <map>

namespace PGA {

class ProceduralAlgorithm;

class ConfigBase {
private:

	ReadWriteLock _lock;
	std::string	  _name;

public:
	ConfigBase() = default;

	ConfigBase(const std::string& config_name);
	virtual ~ConfigBase() {}

	void setName(const std::string& name);
	std::string& getName();
	virtual void toString(std::ostream& out) = 0;
	ReadWriteLock& getLock();
};

class Genome_IF;
class Genome_Factory_IF;
class Grammar_IF;
class Mutation_IF;
class Recombination_IF;
class Evaluation_IF;
class Population_IF;
class GeneticAlgorithm_IF;
class Statistics_IF;
class Selection_IF;
class GeometryBufferInstanced;

class GenomeConf : public ConfigBase {
private:
	int 	_minimal_length;
	int		_optimal_length;
	int 	_maximal_length;
	int		_maximal_depth;
	Genome_Factory_IF* _genome_factory;
public:

	GenomeConf() = default;

	GenomeConf(const std::string& name);

	void toString(std::ostream& out);
	Genome_Factory_IF* factory();
	void setFactory(Genome_Factory_IF* factory);
	int getOptimalLength();
	void setOptimalLength(int val);
	int getMinimalLength();
	void setMinimalLength(int val);
	int getMaximalLength();
	void setMaximalLength(int val);
	int getMaximalDepth();
	void setMaximalDepth(int val);
};

class GrammarConf : public ConfigBase {
private:
	SymbolManager					_sm;
	PGG::Parameters::ParameterTable	_pt;
	PGG::CPU::GrammarSystem			_system;
	int								_num_threads;
	Grammar_IF* 					_grammar;
	std::vector<std::string>		_shape_files;
	std::vector<math::float4>		_shape_colors;
public:

	GrammarConf() = default;

	GrammarConf(const std::string& name);

	void toString(std::ostream& out);
	Grammar_IF* impl();
	void setImpl(Grammar_IF* impl);
	int getNumThreads();
	void setNumThreads(const int numThreads);
	PGG::Parameters::ParameterTable& getPt();
	SymbolManager& getSm();
	PGG::CPU::GrammarSystem& getSystem();
	void addShapeFile(const std::string& filename);
	void addShapeColor(const math::float4& color);
	int getNumShapes();
	const std::string& getShapeFile(const int index);
	const math::float4& getShapeColor(const int index);
};


class MutationConf : public ConfigBase {
private:
	Mutation_IF* _mutator;
	float		_mutation_prob;  			// general mutation probability
	float		_parameter_mutation_prob; 	// probability for parameter mutation
	float		_symbol_cut_prob; 			// probability for symbol mutation
	float		_symbol_grow_prob; 			// probability for symbol mutation
public:

	MutationConf() = default;

	MutationConf(const std::string& name);

	void toString(std::ostream& out);
	Mutation_IF* impl();
	void setImpl(Mutation_IF* impl);
	float getMutationProb();
	void setMutationProb(float mutationProb);
	float getParameterMutationProb();
	void setParameterMutationProb(float parameterMutationProb);
	float getSymbolCutProb();
	void setSymbolCutProb(float symbolCutProb);
	float getSymbolGrowProb();
	void setSymbolGrowProb(float symbolGrowProb);
};

class RecombinationConf : public ConfigBase {
private:
	Recombination_IF* _recombinator;
	float 			  _replace_prob; // probability a gene is replaced if possible
public:

	RecombinationConf() = default;

	RecombinationConf(const std::string& name);

	void toString(std::ostream& out);
	float getReplaceProb();
	void setReplaceProb(float replace_probability);
	Recombination_IF* impl();
	void setImpl(Recombination_IF* impl);
};


class SelectionConf : public ConfigBase {
private:
	int	_tournament_size;
	Selection_IF* _first;
	Selection_IF* _second;
public:

	SelectionConf() = default;

	SelectionConf(const std::string& name);
	void toString(std::ostream& out);
	Selection_IF* first();
	void setFirst( Selection_IF* first);
	Selection_IF* second();
	void setSecond( Selection_IF* second);
	int getTournamentSize();
	void setTournamentSize(int tournamentSize);
};

class EvaluationConf : public ConfigBase {
private:
	Evaluation_IF*	_evaluator;
public:
	EvaluationConf(const std::string& name);
	virtual ~EvaluationConf() {}

	virtual void toString(std::ostream& out) = 0;

	void setImpl(Evaluation_IF* impl);
	Evaluation_IF* impl();
};

class VolumeEvaluationConf : public EvaluationConf {
private:
	math::float3	_bb_min;
	math::float3	_bb_max;
	math::int3		_voxels;
	std::string		_target_string;
	float			_good_weight;
	float			_bad_weight;
	float			_length_weight;
	bool			_separate_overlap;
	float			_overlap_weight;
public:

	VolumeEvaluationConf(const std::string& name);
	virtual ~VolumeEvaluationConf() {}
	void toString(std::ostream& out);
	std::string& getTargetString();
	void setTargetString( const std::string& targetString);
	math::float3& getBBmax();
	void setBBMax(const math::float3& bbMax);
	math::float3& getBBmin();
	void setBBmin(const math::float3& bbMin);
	math::int3& getVoxels();
	void setVoxels(const math::int3& voxels);
	float getBadWeight();
	void setBadWeight(float badWeight);
	float getGoodWeight();
	void setGoodWeight(float goodWeight);
	float getLengthWeight();
	void setLengthWeight(float lengthWeight);
	float getOverlapWeight();
	void setOverlapWeight(float overlapWeight);
	bool getSeparateOverlap();
	void setSeparateOverlap(bool separate);
};

class ImageEvaluationConf : public EvaluationConf {
private:
	math::float3	_bb_min;
	math::float3	_bb_max;
	std::string		_target_filename;
	float			_good_weight;
	float			_bad_weight;
	float			_length_weight;

	math::int2		_pixels;
	float			_fov;
	float 			_znear;
	float			_zfar;
	float 			_phi;
	float			_theta;
	float 			_radius;
	math::float3	_lookat;

public:

	ImageEvaluationConf(const std::string& name);
	void toString(std::ostream& out);
	math::float3& getBBmax();
	void setBBMax(const math::float3& bbMax);
	math::float3& getBBmin();
	void setBBmin(const math::float3& bbMin);
	std::string& getTargetFilename();
	void setTargetFilename( const std::string& targetFilename);
	math::int2& getPixels();
	void setPixels(const math::int2& pixels);
	float getBadWeight();
	void setBadWeight(float badWeight);
	float getGoodWeight();
	void setGoodWeight(float goodWeight);
	float getLengthWeight();
	void setLengthWeight(float lengthWeight);
	void setCameraParameters(float fov, float znear, float zfar, float phi, float theta, float radius, math::float3 lookat);
	void getCameraParameters(float& fov, float& znear, float& zfar, float& phi, float& theta, float& radius, math::float3& lookat);

};

class PopulationConf : public ConfigBase {
private:
	Population_IF* 	_population;
public:

	PopulationConf() = default;

	PopulationConf(const std::string& name);
	void toString(std::ostream& out);
	Population_IF* impl();
	void setImpl(Population_IF* impl);
};

class GeometryConf : public ConfigBase {
public:
	typedef enum {
		OBJECT,
		TARGET
	} MeshBufferTarget;
private:
	bool 					_generate;
	MeshBufferTarget		_target;
	GeometryBufferInstanced* _instanced_mesh_buffer_object;
	GeometryBufferInstanced* _instanced_mesh_buffer_target;
	Evaluation_IF*	_evaluator;

public:

	GeometryConf() = default;

	GeometryConf(const std::string& name);
	void toString(std::ostream& out);
	Evaluation_IF* getEvaluator();
	void setEvaluator(Evaluation_IF* impl);

	void setUsedMeshBuffer(MeshBufferTarget target);

	GeometryBufferInstanced* getInstancedMeshBuffer();

	GeometryBufferInstanced* getInstancedObjectMeshBuffer();
	void setInstancedObjectMeshBuffer(GeometryBufferInstanced* meshBuffer);

	GeometryBufferInstanced* getInstancedTargetMeshBuffer();
	void setInstancedTargetMeshBuffer(GeometryBufferInstanced* meshBuffer);

	bool generate();
	void setGenerate(bool value);
};

class StatisticsConf : public ConfigBase {
private:
	int						_reruns;
	Statistics_IF*			_statistics;
	bool					_print_stats;
	bool					_save_every_obj;
	bool					_save_every_obj_detailed;
	bool					_save_final_obj;
	bool					_save_final_obj_detailed;
	std::string				_obj_prefix;
	std::string				_stats_filename;
	std::string				_perfstats_filename;
public:
	StatisticsConf() = default;
	StatisticsConf(const std::string& name);
	void toString(std::ostream& out);
	Statistics_IF* impl();
	void setImpl(Statistics_IF* impl);
	bool recordStats();
	void setPrintStats(bool value);
	bool printStats();
	void setStatisticsFilename(const std::string& value);
	const std::string& getStatisticsFilename();
	void setPerformanceStatisticsFilename(const std::string& value);
	const std::string& getPerformanceStatisticsFilename();
	void setSaveEveryObj(bool value);
	bool saveEveryObj();
	void setSaveEveryObj_detailed(bool value);
	bool saveEveryObj_detailed();
	void setSaveFinalObj(bool value);
	bool saveFinalObj();
	void setSaveFinalObj_detailed(bool value);
	bool saveFinalObj_detailed();
	void setObjPrefix(const std::string& value);
	const std::string& getobjPrefix();
	int getReruns();
	void setReruns(int val);
};

class AlgorithmConf : public ConfigBase {
private:
	int 					_population_size;
	int						_init_genome_length;
	int						_max_generations;
	int						_elitism;
	int						_current_generation;
	GeneticAlgorithm_IF*	_algorithm;
	std::string				_prefix;
	bool					_use_max_runtime;
	float					_max_runtime;
public:

	AlgorithmConf() = default;

	AlgorithmConf(const std::string& name);
	void toString(std::ostream& out);
	GeneticAlgorithm_IF* impl();
	void setImpl(GeneticAlgorithm_IF* impl);
	int getElitism();
	void setElitism(int elitism);

	int getCurrentGeneration();
	void setCurrentGeneration(int generation);

	int getInitGenomeLength();
	void setInitGenomeLength(int initGenomeLength);
	int getMaxGenerations();
	void setMaxGenerations(int maxgeneration);
	int getPopulationSize();
	void setPopulationSize(int populationSize);

	void setPrefix(const std::string& value);
	const std::string& getPrefix();
	void setUseMaxRuntime(bool value);
	bool getUseMaxRuntime();
	float getMaxRuntime();
	void setMaxRuntime(float value);


};


class ProceduralAlgorithm {
private:
	static ProceduralAlgorithm* s_instance;


	std::map<std::string, ConfigBase*> _cfg;

public:
	ProceduralAlgorithm()
	{};


	std::map<std::string, ConfigBase*>& getAllConfigs() {
		return _cfg;
	}

	void add(const std::string& id, ConfigBase* config)
	{
		if (_cfg.find(id) != _cfg.end()) {
			std::stringstream ss;
			ss << "attempting to overwrite configuration " << id;
			throw std::invalid_argument(ss.str().c_str());
		}
		_cfg[id] = config;
	}

	template<typename T>
	T get(const std::string& id) {
		if (_cfg.find(id) == _cfg.end()) {
			std::stringstream ss;
			ss << "configuration " << id << " unknown";
			throw std::invalid_argument(ss.str().c_str());
		}
		ConfigBase* ret = _cfg[id];
		return dynamic_cast<T>(ret);
	}
};

}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_CONFIG_H_ */
