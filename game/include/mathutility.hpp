#ifndef MATHUTILITY_HPP
#define MATHUTILITY_HPP

constexpr float  PI_F = 3.14159265358979f;
constexpr float PI_F_180 = 180.0f / PI_F;

inline float MapRange(float rangeMin, float rangeMax, float current, float endRangeMin, float endRangeMax)
{
	float testValue = rangeMax - rangeMin;
	float firstValue = current / testValue;
	float result = endRangeMin + (endRangeMax - endRangeMin) * firstValue;
	return result;
}

#endif // !MATHUTILITY_HPP

