#pragma once
#include "MathDefs.h"
#include <vector>

class Circle;
class FluidSim;

class sorter {
public:
	sorter(int ni_, int nj_);
	~sorter();

	void sort(FluidSim* sim);
	void getNeigboringParticles_cell(int i, int j, int wl, int wh, int hl, int hh, std::vector<Circle*>&);

	int	 getNumParticleAt(int i, int j);
	void deleteAllParticles();

	std::vector< std::vector<Circle*> > cells;
	int ni;
	int nj;
};