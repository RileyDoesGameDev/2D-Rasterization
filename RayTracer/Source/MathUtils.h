#pragma once
#include <glm/glm.hpp>
template<typename T>
	inline T Lerp(T a, T b, float t)
	{
		return  static_cast<T>(a + ((b - a) * t));
	}

	inline void QuadraticPoint(int x1, int y1, int x2, int y2, int x3, int y3, float t, int& x, int& y)
	{
		float one_minus_t1 = 1 - t;
		float a1 = one_minus_t1 * one_minus_t1;
		float b1 = 2 * one_minus_t1 * t;
		float c1 = t * t;


		x = (int)(a1 * x1 + b1 * x2 + c1 * x3);
		y = (int)(a1 * x1 + b1 * x2 + c1 * x3);
	}

	inline glm::vec3 Cross(const glm::vec3& v1, const glm::vec3& v2) {
		glm::vec3 result;

		result.x = v1.y * v2.z - v2.y * v1.z;
		result.y = v1.z * v2.x - v2.z * v1.x;
		result.z = v1.x * v2.y - v2.x * v1.y;

		return result;
	}


	template<typename T>
	inline T Clamp(const T& value, const T& min, const T& max)
	{
		return (value < min) ? min : (value > max) ? max : value;
	}

	inline bool approximately(float value1, float value2)
	{
		// check if the difference between the values is less than epsilon
		return ((value1 - value2) < FLT_EPSILON);
	}