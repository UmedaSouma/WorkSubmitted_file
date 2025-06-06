#pragma once
//===========================================================================================================================================================
// 
// calculation.cppのヘッダー [calculation.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CALCULATION_H_
#define _CALCULATION_H_
#include "main.h"
#include <random>

class CCalculation
{
public:

	// 回転軸の列挙型
	typedef enum
	{
		ROT_X = 0,
		ROT_Y,
		ROT_Z,
	}ROT_AXIS;

	CCalculation();
	~CCalculation();
	static float FixAngle(float Angle, float TargetAngle);
	static float DemandAngle(D3DXVECTOR3 posA , D3DXVECTOR3 posB);
	static void ResetRot(float& rot);	// 角度をリセット
	static float Ratio(float Max, float Curr);
	static D3DXVECTOR3 ShiftPosition(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 shiftsize,ROT_AXIS axis);
	static float TwoPointDistance(D3DXVECTOR3 posA, D3DXVECTOR3 posB);
	static float TwoPointDistanceHeight(D3DXVECTOR3 posA, D3DXVECTOR3 posB);
	static uint64_t RandomInt(uint64_t min, uint64_t max);
private:
	static std::mt19937_64 mt64;	// 乱数生成器
};

#endif // !CALCULATION_H_
