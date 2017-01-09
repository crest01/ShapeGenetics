/*
 * Parameter.h
 *
 *  Created on: Nov 6, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_PARAMETER_PARAMETER_H_
#define GPUPROCGENETICS_SRC_PARAMETER_PARAMETER_H_

#include <iostream>

namespace PGA {

struct Parameter {
	unsigned int 	size;
	unsigned char* 	data;
	template< typename T>
	T getValue() {
		if(size != sizeof(T)) {
			std::cout << "Parameter Warning: Parameter size mismatch!" << std::endl;
			throw(std::runtime_error("Parameter size mismatch"));

		}
		return *(reinterpret_cast<T*>(data));
	}
};

}  // namespace PGA


#endif /* GPUPROCGENETICS_SRC_PARAMETER_PARAMETER_H_ */
