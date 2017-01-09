/*
 * ParamterDescriptor.h
 *
 *  Created on: Nov 6, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_PARAMETER_PARAMETERDESCRIPTOR_H_
#define GPUPROCGENETICS_SRC_PARAMETER_PARAMETERDESCRIPTOR_H_

#include <vector>
#include <cstring>
#include "Parameter.h"
#include "RandomValues.h"

namespace PGA {


class ParameterDescriptor_IF {
private:
	virtual void generateRandomValue_impl(Parameter* ) = 0;
	std::string _name;

public:
    ParameterDescriptor_IF(const std::string& name):
    	_name(name) {};

    virtual ~ParameterDescriptor_IF() {}
    void generateRandomValue(Parameter* param)
    {
    	generateRandomValue_impl(param);
    }

   virtual unsigned int typeSize() = 0;
   virtual float probability() = 0;

};

template <typename T>
class ParameterDescriptor : public ParameterDescriptor_IF {
	private:
		T _min;
		T _max;

		virtual void generateRandomValue_impl(Parameter* param)
	    {
			if (param->size == sizeof(T)) {
				T val = getRandomValue(_min, _max);
				void *pv = static_cast<void* >(&val);
				memcpy(param->data, pv, sizeof(T));
			} else {
				throw std::invalid_argument("incorrect type for getRandomVal");
			}
	    }

	public:
	    ParameterDescriptor(const std::string& name, const T& min, const T& max):
	    	ParameterDescriptor_IF(name), _min(min), _max(max)
		{
		}
	    unsigned int typeSize() { return sizeof(T); }
	    // returns the likelihood of the last selected parameter. For U[0, 1] = 1
	    float probability() { return 1; }
};

}  // namespace PGA


#endif /* GPUPROCGENETICS_SRC_PARAMETER_PARAMETERDESCRIPTOR_H_ */
