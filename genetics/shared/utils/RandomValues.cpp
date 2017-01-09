/*
 * RandomParameter.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Karl Haubenwallner
 */


#include "RandomValues.h"
#include <random>
#include "parameters/Random.h"

namespace PGA {

class RndGen {
private:
	RndGen() :
	_gen(nullptr)
	{
		initGenerator();
	}
	static RndGen* 		_instance;
	std::mt19937* 	_gen;
	std::random_device rd;

public:

	static RndGen* instance() {
		if (_instance == nullptr) {
			_instance = new RndGen();
		}
		return _instance;
	}

	void initGenerator()
	{
		if (_gen != nullptr)
			delete _gen;

		_gen = new std::mt19937(rd());
	}

	float3 getRandomValue(const float3& min, const float3& max)
	{
		return float3( getRandomValue(min.x, max.x),
					   getRandomValue(min.y, max.y),
					   getRandomValue(min.z, max.z));
	}

	math::float2 getRandomValue(const math::float2& min, const math::float2& max)
	{
		return math::float2(getRandomValue(min.x, max.x),
			getRandomValue(min.y, max.y));
	}

	float getRandomValue01()
	{
	//	return *_gen.generateFloat01();
		return std::generate_canonical<float, sizeof(float)*8>(*_gen);
	}

	float getRandomValue(const float& min, const float& max)
	{
		if (max == min) {
			return min;
		}
		float diff = max - min;
	//	return min + diff * *_gen.generateFloat();
		return min + diff * std::generate_canonical<float, sizeof(float)*8>(*_gen);

	}

	int getRandomValue(const int& min, const int& max)
	{
		if (max == min) {
			return min;
		}
		return static_cast<int>(getRandomValue(static_cast<float>(min), static_cast<float>(max)));

	//	int diff = max - min;
	//	return (*_gen.generateUint() % diff + min);
	}


};

RndGen* RndGen::_instance = nullptr;

float3 getRandomValue(const float3& min, const float3& max)
{
	return RndGen::instance()->getRandomValue(min, max);
}

math::float2 getRandomValue(const math::float2& min, const math::float2& max)
{
	return RndGen::instance()->getRandomValue(min, max);
}

float getRandomValue01()
{
	return RndGen::instance()->getRandomValue01();
}

float getRandomValue(const float& min, const float& max)
{
	return RndGen::instance()->getRandomValue(min, max);
}

int getRandomValue(const int& min, const int& max)
{
	return RndGen::instance()->getRandomValue(min, max);
}

void reinitMtGenerator()
{
	RndGen::instance()->initGenerator();
}



}  // namespace PGA


