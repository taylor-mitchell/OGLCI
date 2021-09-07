#include "FluidGrid.h"


FluidGrid::FluidGrid(int width, int height, float cellWidth, float cellHeight)
	:width(width),
	height(height),
	cellWidth(cellWidth),
	cellHeight(cellHeight),
	viscosity(1.0f),
	deltaT(0.001f),
	grid(width, height, cellWidth, cellHeight),
	cells(width, height),
	lines(width, height),
	densities(width, height),
	energies(width, height),
	velocities(width, height),
	pressures(width, height),
	velocityTimeDerivative(width, height),
	velocitySpatialDerivative(width, height),
	velocitySecondSpatialDerivative(width, height),
	velocityMixedDerivative(width, height),
	densityTimeDerivative(width, height),
	densitySecondTimeDerivative(width, height),
	densitySpatialDerivative(width, height),
	densitySecondSpatialDerivative(width, height),
	densityMixedDerivative(width, height),
	pressureSpatialDerivative(width, height),
	pressureSecondSpatialDerivative(width, height)	
{
}

void FluidGrid::addParticle(Circle* circle)
{
	int gridPosX = static_cast<int>(circle->getPos().x / cellWidth);
	int gridPosY = static_cast<int>(circle->getPos().y / cellHeight);
	if (gridPosX > 0 && gridPosX < width && gridPosY > 0 && gridPosY < height)
	{
		cells(gridPosX, gridPosY).push_back(circle);
	}
}

void FluidGrid::interpolateParticles()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			velocities(i, j) = glm::vec2(0.0f, 0.0f);
			//weights(i, j) = 0.0f;
			for (int k = 0; k < cells(i, j).size(); ++k)
			{
				//velocities(i, j) += cells(i, j).at(k)->getVelocity();
				//weights(i, j) += cells(i, j).at(k)->getRadius();
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
				//cells(i, j).at(k)->setVelocity(velocities(i, j));
			}
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

void FluidGrid::calculateSpatialDerivatives()
{
	velocitySpatialDerivative = Matrix::derivative(velocities, cellWidth, cellHeight);
	velocitySecondSpatialDerivative = Matrix::secondDerivative(velocities, cellWidth, cellHeight);
	velocityMixedDerivative = Matrix::mixedDerivative(velocities, cellWidth, cellHeight);

	densitySpatialDerivative = Matrix::derivative(densities, cellWidth, cellHeight);
	densitySecondSpatialDerivative = Matrix::secondDerivative(densities, cellWidth, cellHeight);
	densityMixedDerivative = Matrix::mixedDerivative(densities, cellWidth, cellHeight);

	pressureSpatialDerivative = Matrix::derivative(pressures, cellWidth, cellHeight);
	pressureSecondSpatialDerivative = Matrix::secondDerivative(pressures, cellWidth, cellHeight);
}

void FluidGrid::calculateFirstDerivatives()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			//some of these might be wrong because of negatives.  if it doesnt work, i will come back and rederive everything :(
			densityTimeDerivative(i, j) = -1.0f * (densities(i, j) * velocitySpatialDerivative(i, j).du_dx + velocities(i, j).x * densitySpatialDerivative(i, j).du_dx + densities(i, j) * velocitySpatialDerivative(i, j).dv_dy + velocities(i, j).y * densitySpatialDerivative(i, j).du_dy);
			velocityTimeDerivative(i, j) = -1.0f * glm::vec2(	velocities(i, j).x * velocitySpatialDerivative(i, j).du_dx + velocities(i, j).y * velocitySpatialDerivative(i, j).du_dy + pressureSpatialDerivative(i, j).du_dx / densities(i, j), 
																velocities(i, j).x * velocitySpatialDerivative(i, j).dv_dx + velocities(i, j).y * velocitySpatialDerivative(i, j).dv_dy + pressureSpatialDerivative(i, j).du_dy / densities(i, j));
		}
	}
}

void FluidGrid::calculateSecondDerivatives()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			//some of these might be wrong because of negatives.  if it doesnt work, i will come back and rederive everything :(
			densitySecondTimeDerivative(i, j) =
				densities(i, j) * (velocities(i, j).x * (2.0f * velocitySecondSpatialDerivative(i, j).du_dx + velocityMixedDerivative(i, j).dv_dydx + velocityMixedDerivative(i, j).dv_dxdy) +
					velocities(i, j).y * (2.0f * velocitySecondSpatialDerivative(i, j).dv_dy + velocityMixedDerivative(i, j).du_dydx + velocityMixedDerivative(i, j).du_dxdy) +
					std::pow(velocitySpatialDerivative(i, j).du_dx, 2) + std::pow(velocitySpatialDerivative(i, j).dv_dy, 2) + 2.0f * velocitySpatialDerivative(i, j).dv_dx * velocitySpatialDerivative(i, j).du_dy) +
				velocities(i, j).x * (velocities(i, j).x * densitySecondSpatialDerivative(i, j).du_dx + 2.0f * densitySpatialDerivative(i, j).du_dx * velocitySpatialDerivative(i, j).du_dx + densitySpatialDerivative(i, j).du_dx * velocitySpatialDerivative(i, j).dv_dy + densitySpatialDerivative(i, j).du_dy * velocitySpatialDerivative(i, j).dv_dx) +
				velocities(i, j).y * (velocities(i, j).y * densitySecondSpatialDerivative(i, j).du_dy + 2.0f * densitySpatialDerivative(i, j).du_dy * velocitySpatialDerivative(i, j).dv_dy + densitySpatialDerivative(i, j).du_dx * velocitySpatialDerivative(i, j).du_dy + densitySpatialDerivative(i, j).du_dy * velocitySpatialDerivative(i, j).du_dx) +
				velocities(i, j).x * velocities(i, j).y * (densityMixedDerivative(i, j).du_dxdy + densityMixedDerivative(i, j).du_dydx) +
				-1.0f / densities(i, j) * (pressureSpatialDerivative(i, j).du_dx * densitySpatialDerivative(i, j).du_dx + pressureSpatialDerivative(i, j).du_dy * densitySpatialDerivative(i, j).du_dy) +
				densitySecondSpatialDerivative(i, j).du_dx + densitySecondSpatialDerivative(i, j).du_dy -
				densityTimeDerivative(i, j) * velocitySpatialDerivative(i, j).du_dx -
				velocityTimeDerivative(i, j).x * densitySpatialDerivative(i, j).du_dx -
				densityTimeDerivative(i, j) * velocitySpatialDerivative(i, j).du_dy -
				velocityTimeDerivative(i, j).y * densitySpatialDerivative(i, j).du_dy;
		}
	}
}

void FluidGrid::calculatePressure()
{
	
}

void FluidGrid::calculateVelocity()
{

}

void FluidGrid::calculateEnergy()
{
}

void FluidGrid::update()
{
	//interpolateParticles();
	calculateSpatialDerivatives();
	//calculateDensity();
	calculatePressure();
	calculateVelocity();
	//extrapolateParticles();
	clearParticles();
}

void FluidGrid::draw(Renderer& renderer)
{

}


