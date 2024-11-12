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
		return (std::fabs(value1 - value2) < FLT_EPSILON);
	}

	inline float Dot(const glm::vec3& v1, const glm::vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	}


	inline glm::vec3 Reflect(const glm::vec3& i, const glm::vec3& n)
	{
		
		return i - ((glm::dot(i, n) * n) * 2.0f);
		//return (i - 2.0f) * dot(i, n) * glm::normalize(n);
	}

	inline float Angle(const glm::vec3& v1, const glm::vec3& v2)
	{
		return glm::acos(glm::dot(glm::normalize(v1), glm::normalize(v2)));
		
	}


	inline bool Refract(const glm::vec3& i, const glm::vec3& n, float ri, glm::vec3& refracted)
	{
		glm::vec3 ni = glm::normalize(i);
		float cosine = glm::dot(ni, n);
		float discriminant = 1 - (ri * ri) * (1 - cosine * cosine);
		if (discriminant > 0) 
		{
			refracted = ri * (ni - (n * cosine)) - (n * glm::sqrt(discriminant));
			return true;
		}
		return false;
	}

	inline float Schlick(float cosine, float ri)
	{
		// Step 1: Calculate the base reflectance at zero incidence (angle = 0)
		// This is the reflection coefficient when the light hits the surface straight on
		float r0 = (1.0f - ri) / (1.0f + ri);
		r0 = r0 * r0;
	
		// Step 2: Use Schlick's approximation to adjust reflectance based on angle
		// Schlick’s approximation gives the probability of reflection at an angle `cosine`
		// It interpolates between `r0` and 1, with stronger reflection at glancing angles
		return r0 + (1.0f - r0) * (float)std::pow((1.0f - cosine), 5);
	}