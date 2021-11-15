#pragma once

namespace JEngine
{
	namespace maths
	{
		std::pair<double, double> BoxMullerPair(double mu, double sigma);

		double BoxMullerMarsagliaPolar(double mean, double stdDev);

	}
}