
#include "Sorter.h"
#include "FluidSim.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

sorter::sorter(int ni_, int nj_)
	: ni(ni_), nj(nj_)
{
	cells.resize(ni_ * nj_);
}

sorter::~sorter() {
}

void sorter::sort(FluidSim* sim) {
	// Clear All Cells
	for (int j = 0; j < nj; ++j) for (int i = 0; i < ni; ++i) {
		cells[j * ni + i].clear();
	}

	// Store Into The Cells
	const int np = (int)sim->particles.size();
	for (int n = 0; n < np; n++) {
		Circle* p = &sim->particles[n];

		int pi = (int)((p->getPos().x - sim->origin.x) / sim->dx);
		int pj = (int)((p->getPos().y - sim->origin.y) / sim->dx);
		int i = max(0, min(ni - 1, pi));
		int j = max(0, min(nj - 1, pj));
		cells[j * ni + i].push_back(p);
	}
}

void sorter::getNeigboringParticles_cell(int i, int j, int wl, int wh, int hl, int hh, std::vector<Particle*>& res) {

	for (int si = i + wl; si <= i + wh; si++) for (int sj = j + hl; sj <= j + hh; sj++) {
		if (si < 0 || si > ni - 1 || sj < 0 || sj > nj - 1) continue;
		res.insert(res.end(), cells[sj * ni + si].begin(), cells[sj * ni + si].end());
	}
}

int sorter::getNumParticleAt(int i, int j) {
	return (int)cells[j * ni + i].size();
}

void sorter::deleteAllParticles() {
	for (int j = 0; j < nj; ++j) for (int i = 0; i < ni; ++i) {
		cells[j * ni + i].clear();
	}
}