/*
 * GenomeGPU.h
 *
 *  Created on: Nov 12, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_GenomeGPU_H_
#define GPUPROCGENETICS_SRC_GENETICS_GenomeGPU_H_

#ifdef 0
namespace PGA {

class SymbolManager;
class ParseTree;

struct Gene_GPU: public Gene_IF {
private:
	Gene_IF*		_parent;
	int 			_id;
	unsigned int	_num_params;
	unsigned char*	_params;

	Parameter*	getParameter(const int index)
	{
		if (num_params <= index)
			return nullptr;

		unsigned char* ptr = params;
		for (int i = 0; i < index; ++i) {
			Parameter *p = reinterpret_cast<Parameter*>(ptr);
			ptr += p->size;
		}
		return reinterpret_cast<Parameter*>(ptr);
	}

public:
	int	id() { return _id; }
	unsigned int numParameter() { return _num_params; };
	Parameter* getParameter(const unsigned int index);

};

class Genome_GPU : public Genome_IF {
private:
	std::vector<unsigned char>		_data;
	unsigned char*					_end;
	unsigned int					_current_bytes;
	unsigned int					_num_genes;

	void clearGenome();

	// creates a new gene-struct at the end of the array, and
	// updates the prev/next-pointers
	Gene* createGene();

	// creates a empty parameter behind the current last gene
	Parameter* createParameter(const unsigned int size);

	// creates the gene and it's parameters inside the array
	Gene* createAndAddGene(void *parent, SymbolDescriptor* sd);

	void assureSize(const unsigned int bytes)
	{
		_data.reserve(bytes);
	}

	// remove a single gene from the array, and update the links
	void removeGene(Gene* gene);


public:

	virtual void createFromString(const SymbolManager& sm, const std::string& string) = 0;

	// remove a gene and all it's children from the array
	virtual void deleteGene(const unsigned int index) = 0;

	virtual Gene_IF* operator[](const unsigned int index) = 0;

	virtual Gene_IF* getGene(const unsigned int index) = 0;

	virtual int length() const = 0;

	Gene_IF* createAndAddGene(Gene_IF *parent, SymbolDescriptor* sd) = 0;
};



} // namespace PGA

#endif


#endif /* GPUPROCGENETICS_SRC_GENETICS_GenomeGPU_H_ */
