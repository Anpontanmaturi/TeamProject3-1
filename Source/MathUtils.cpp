#include <stdlib.h>
#include "MathUtils.h"

//指定範囲のランダム値を計算する
float MathUtils::RandomRange(float min, float max)
{
	//0.0～1.0の間までのランダム値
	float value = static_cast<float>(rand()) / RAND_MAX;

	//min～maxまでのランダム値に変換
	return min + (max - min) * value;
}
