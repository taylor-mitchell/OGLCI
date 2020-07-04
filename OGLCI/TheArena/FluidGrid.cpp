#include "FluidGrid.h"
#include "MatrixFunctions.h"

FluidGrid::FluidGrid(int width, int height, float cellWidth, float cellHeight)
    :width(width),
    height(height),
    cellWidth(cellWidth),
    cellHeight(cellHeight),
    density(1.0f),
    grid(width, height, cellWidth, cellHeight),
    cells(width, height),
    distances(width, height),
    velocities(width, height),
    pressures(width, height),
    pressureDerivatives(width, height),
	pressureLHS(width * height, width * height)
{
	for (int i = 0; i < width * height; ++i)
	{
		for (int j = 0; j < width * height; ++j)
		{
			if (i == j)
			{
				pressureLHS(i, j) = 1.0f;
			}
			else
			{
				if (i == j - width || i == j + width || i == j - 1 || i == j + 1)
				{
					pressureLHS(i, j) = -1.0f;
				}
			}
		}
	}
}

void FluidGrid::addParticle(Circle* circle)
{
	int gridPosX = circle->getPos().x / cellWidth;
	int gridPosY = circle->getPos().y / cellHeight;
	cells(gridPosX, gridPosY).push_back(circle);
}

void FluidGrid::interpolateParticles()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			velocities(i, j) = glm::vec2(0.0f, 0.0f);
			for (int k = 0; k < cells(i, j).size(); ++k)
			{
				velocities(i, j) += cells(i, j).at(k)->getVelocity();
			}
		}
	}
}

void FluidGrid::extrapolateParticles()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			for (int k = 0; k < cells(i, j).size(); ++k)
			{
				cells(i, j).at(k)->setVelocity(velocities(i, j));
			}
		}
	}
}

void FluidGrid::populateDistances()
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			//just using a static wall on the bottom of the grid for now
			distances(j, i) = i * cellHeight;
		}
	}
}

void FluidGrid::clearParticles()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			cells(i, j).clear();
		}
	}
}

void FluidGrid::calculatePressure()
{
	Array2D<float> pressureResult;
	Array2D<float> permutation;
	Array2D<float> lower;
	Array2D<float> upper;

	Array2D<float> pressureRHS;
	pressureRHS.setSize(1, width * height);

    for (int j = 1; j < height - 1; ++j) {
		for (int i = 1; i < width - 1; ++i) {
			pressureRHS(0, j * width + i) = cells(i, j).size() * -1.0f; //gravity is -1.0f for now. mass is number of particles in the cell      
        }
    }
		
	//I could just decomposeLU once in constructor and reuse the resulting matrices every time
	Matrix::decomposeLU(pressureLHS, lower, upper, permutation);
	Matrix::solveLU(permutation, lower, upper, pressureRHS, pressureResult);

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			pressures(i, j) = glm::vec2(pressureResult(0, j * width + i), pressureResult(0, j * width + i));
		}
	}	
}

void FluidGrid::calculateVelocity()
{
	float viscosity = 1.0f;
	for (int j = 1; j < height - 1; ++j) {
		for (int i = 1; i < width - 1; ++i) {
			glm::vec2 velocityConvection = glm::vec2((velocities(i - 1, j).x + velocities(i + 1, j).x) / 2.0f, (velocities(i, j - 1).y + velocities(i, j + 1).y) / 2.0f);
			glm::vec2 velocityLaplacian = glm::vec2(
				velocities(i - 1, j).x + velocities(i + 1, j).x + velocities(i, j - 1).x + velocities(i, j + 1).x - 4.0f * velocities(i, j).x / (cellWidth * cellWidth),
				velocities(i - 1, j).y + velocities(i + 1, j).y + velocities(i, j - 1).y + velocities(i, j + 1).y - 4.0f * velocities(i, j).y / (cellHeight * cellHeight));
			glm::vec2 pressureGradient = glm::vec2((pressures(i - 1, j).x + pressures(i + 1, j).x) / 2.0f, (pressures(i, j - 1).y + pressures(i, j + 1).y) / 2.0f);
			velocities(i, j) = pressureGradient * -1.0f / density + viscosity * velocityLaplacian - velocityConvection * velocities(i, j);
		}
	}

	for (int i = 0; i < width; ++i)
	{
		velocities(i, 0).y = 0.0f;
	}
}

void FluidGrid::update()
{
	interpolateParticles();
	calculatePressure();
	calculateVelocity();
	extrapolateParticles();
	clearParticles();
}


