#pragma once

template<typename T>
	inline T Lerp(T a, T b, float t)
	{
		return  static_cast<T>(a + ((b - a) * t));
	}

	void QuadraticPoint(int x1, int y1, int x2, int y2, int x3, int y3, float t, int& x, int& y)
	{
		float one_minus_t1 = 1 - t;
		float a1 = one_minus_t1 * one_minus_t1;
		float b1 = 2 * one_minus_t1 * t;
		float c1 = t * t;


		x = (int)(a1 * x1 + b1 * x2 + c1 * x3);
		y = (int)(a1 * x1 + b1 * x2 + c1 * x3);
	}