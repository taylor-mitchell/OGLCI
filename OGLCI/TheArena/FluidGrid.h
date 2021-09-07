#pragma once
#include "Grid.h"
#include <vector>
#include "Circle.h"
#include "Array2D.h"
#include "Renderer.h"
#include "Line.h"
#include "MatrixFunctions.h"

class FluidGrid
{
private:
	int width;
	int height;
	float cellWidth;
	float cellHeight;
	float viscosity;
	float deltaT;
	Grid grid;
	Array2D<std::vector<Circle*>> cells;
	Array2D<Line> lines;
	
	Array2D<float> energies;
	Array2D<Matrix::OneValueTwoDimDerivativeResult> energySpatialDerivative;
	
	Array2D<glm::vec2> velocities;
	Array2D<glm::vec2> velocityTimeDerivative;
	Array2D<Matrix::TwoValueTwoDimDerivativeResult> velocitySpatialDerivative;
	Array2D<Matrix::TwoValueTwoDimDerivativeResult> velocitySecondSpatialDerivative;
	Array2D<Matrix::TwoValueTwoDimMixedDerivativeResult> velocityMixedDerivative;

	Array2D<float> densities;
	Array2D<float> densityTimeDerivative;
	Array2D<float> densitySecondTimeDerivative;
	Array2D<Matrix::OneValueTwoDimDerivativeResult> densitySpatialDerivative;
	Array2D<Matrix::OneValueTwoDimDerivativeResult> densitySecondSpatialDerivative;
	Array2D<Matrix::OneValueTwoDimMixedDerivativeResult> densityMixedDerivative;

	Array2D<float> pressures;
	Array2D<Matrix::OneValueTwoDimDerivativeResult> pressureSpatialDerivative;
	Array2D<Matrix::OneValueTwoDimDerivativeResult> pressureSecondSpatialDerivative;

	

public:
	FluidGrid() :width(0), height(0), cellWidth(0.0f), cellHeight(0.0f), viscosity(1.0f), deltaT(1.0f), grid(), cells(), lines(), densities(), energies(), velocities(), pressures(){};
	FluidGrid(int width, int height, float cellWidth, float cellHeight);

	inline Grid* getGrid() { return &grid; };
	void addParticle(Circle* circle); 
	void interpolateParticles();
	void extrapolateParticles();
	void clearParticles();
	void calculateSpatialDerivatives();
	void calculateFirstDerivatives();
	void calculateSecondDerivatives();
	void calculateDensity();	
	void calculatePressure();
	void calculateVelocity();
	void calculateEnergy();
	void update();

	void draw(Renderer& renderer);




};