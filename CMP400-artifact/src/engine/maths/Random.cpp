#include "pch.h"
#include "engine/maths/Random.h"

namespace JEngine
{
	namespace maths
	{

		std::pair<double, double> BoxMuller2D(double mu, double sigma)
		{
			constexpr double epsilon = std::numeric_limits<double>::epsilon();
			constexpr double two_pi = glm::two_pi<double>();

			//initialize the random uniform number generator (runif) in a range 0 to 1
			static std::mt19937 rng(std::random_device{}()); // Standard mersenne_twister_engine seeded with rd()
			static std::uniform_real_distribution<> runif(0.0, 1.0);

			//create two random numbers, make sure u1 is greater than epsilon
			double u1, u2;
			do
			{
				u1 = runif(rng);
				u2 = runif(rng);
			} while (u1 <= epsilon);

			//compute z0 and z1
			auto mag = sigma * sqrt(-2.0 * log(u1));
			auto z0 = mag * cos(two_pi * u2) + mu;
			auto z1 = mag * sin(two_pi * u2) + mu;

			return std::make_pair(z0, z1);
		}

		double BoxMuller(double mu, double sigma)
		{
			constexpr double epsilon = std::numeric_limits<double>::epsilon();
			constexpr double two_pi = glm::two_pi<double>();

			//initialize the random uniform number generator (runif) in a range 0 to 1
			static std::mt19937 rng(std::random_device{}()); // Standard mersenne_twister_engine seeded with rd()
			static std::uniform_real_distribution<> runif(0.0, 1.0);

			//create two random numbers, make sure u1 is greater than epsilon
			double u1, u2;
			do
			{
				u1 = runif(rng);
				u2 = runif(rng);
			} while (u1 <= epsilon);

			//compute z0 and z1
			auto mag = sigma * sqrt(-2.0 * log(u1));
			auto z0 = mag * cos(two_pi * u2) + mu;

			return z0;
		}

		double BoxMullerMarsagliaPolar(double mean, double stdDev)
		{
			static double spare;
			static bool hasSpare = false;

			if (hasSpare) {
				hasSpare = false;
				return spare * stdDev + mean;
			}

			double u, v, s;
			do {
				u = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
				v = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
				s = u * u + v * v;
			} while (s >= 1.0 || s == 0.0);
			s = sqrt(-2.0 * log(s) / s);
			spare = v * s;
			hasSpare = true;
			return mean + stdDev * u * s;
		}

	}
}