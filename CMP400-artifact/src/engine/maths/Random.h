#pragma once

namespace JEngine
{
	namespace maths
	{
		std::pair<double, double> BoxMuller2D(double mu, double sigma);
		double BoxMuller(double mu, double sigma);

		template <typename T>
		T BoxMuller(T mu, T sigma)
		{
			constexpr T epsilon = std::numeric_limits<T>::epsilon();
			constexpr T two_pi = glm::two_pi<T>();

			//initialize the random uniform number generator (runif) in a range 0 to 1
			static std::mt19937 rng(std::random_device{}()); // Standard mersenne_twister_engine seeded with rd()
			static std::uniform_real_distribution<> runif(0.0, 1.0);

			//create two random numbers, make sure u1 is greater than epsilon
			T u1, u2;
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

		double BoxMullerMarsagliaPolar(double mean, double stdDev);

	}
}