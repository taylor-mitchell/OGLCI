#pragma once
#include "Grid.h"
#include <vector>
#include "Circle.h"
#include "Array2D.h"

class FluidGrid
{
private:
	int width;
	int height;
	float cellWidth;
	float cellHeight;
	float density;
	//float deltaT;
	Grid grid;
	Array2D<std::vector<Circle*>> cells;
	Array2D<float> distances;
	Array2D<glm::vec2> velocities;
	Array2D<glm::vec2> pressures;
	Array2D<glm::vec2> pressureDerivatives;
	Array2D<float> pressureLHS;

public:
	FluidGrid() :width(0), height(0), cellWidth(0.0f), cellHeight(0.0f), density(0.0f), grid(), cells(), distances(), velocities(), pressures(), pressureDerivatives() {};
	FluidGrid(int width, int height, float cellWidth, float cellHeight);

	inline Grid* getGrid() { return &grid; };
	void addParticle(Circle* circle); 
	void interpolateParticles();
	void extrapolateParticles();
	void populateDistances();
	void clearParticles();
	void calculatePressure();
	void calculateVelocity();
	void update();




};