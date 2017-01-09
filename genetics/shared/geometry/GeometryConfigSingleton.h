/*
 * GeometrySingleton.h
 *
 *  Created on: Jul 14, 2016
 *      Author: toe
 */

#ifndef GEOMETRYSINGLETON_H_
#define GEOMETRYSINGLETON_H_

#include "GeneticAlgoConfig.h"

namespace PGA {

class GeometryConfigSingleton {
private:
	GeometryConfigSingleton() {};
	static GeometryConfigSingleton* instance;

public:

	static GeometryConfigSingleton* getInstance()
	{
		if (instance == nullptr) {
			instance = new GeometryConfigSingleton();
		}
		return instance;
	}

	void init(ProceduralAlgorithm* algo) {
		conf = algo->get<GeometryConf*>("geometry");
	}

	GeometryConf* conf;

};


}


#endif /* GEOMETRYSINGLETON_H_ */
