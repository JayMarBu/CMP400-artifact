#include "pch.h"
#include "engine/algorithms/Interpolate.h"

namespace JEngine
{
	namespace maths
	{
		template<typename T>
		T Lerp(T a, T b, float t) { return {}; }

		template<>
		float Lerp<float>(float a, float b, float t)
		{
			return(1 - t) * a + t * b;
		}
		
		template<>
		double Lerp<double>(double a, double b, float t)
		{
			return(1 - (double)t) * a + t * b;
		}

		template<typename T>
		T InvLerp(T a, T b, T v) { return {}; }

		template<>
		float InvLerp<float>(float a, float b, float v)
		{
			return (v - a) / (b - a);
		}

		template<>
		double InvLerp<double>(double a, double b, double v)
		{
			return (v - a) / (b - a);
		}

	}
}