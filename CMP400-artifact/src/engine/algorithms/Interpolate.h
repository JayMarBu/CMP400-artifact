#pragma once

namespace JEngine
{
	namespace maths
	{
		template<typename T>
		T Lerp(T a, T b, float t);

		template<>
		float Lerp<float>(float a, float b, float t);

		template<>
		double Lerp<double>(double a, double b, float t);

		template<typename T>
		T InvLerp(T a, T b, T v);

		template<>
		float InvLerp<float>(float a, float b, float v);

		template<>
		double InvLerp<double>(double a, double b, double v);
	}
}