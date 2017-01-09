/*
 * RendererInput_IF.h
 *
 *  Created on: May 19, 2016
 *      Author: toe
 */

#ifndef RENDERERINPUT_IF_H_
#define RENDERERINPUT_IF_H_

class RendererInput_IF {
public:
	virtual void regenerate() = 0;
	virtual void toggleDisplayVoxels() = 0;
	virtual void screenshot() = 0;
};



#endif /* RENDERERINPUT_IF_H_ */
