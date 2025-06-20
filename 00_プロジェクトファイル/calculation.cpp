//===========================================================================================================================================================
// 
// 計算 [calculation.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "calculation.h"

CCalculation::CCalculation()
{
}

CCalculation::~CCalculation()
{
}

//===========================================================================================================
// 角度補正計算
//===========================================================================================================
float CCalculation::FixAngle(float Angle, float TargetAngle)
{
	// 目的の角度と現在の角度の差分
	float Diff = TargetAngle - Angle;

	// 差が ±π だったら補正をする
	if (Diff > D3DX_PI)
	{
		TargetAngle -= D3DX_PI * 2;
	}
	if (Diff < -D3DX_PI)
	{
		TargetAngle += D3DX_PI * 2;
	}

	return TargetAngle;
}

float CCalculation::DemandAngle(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	// 2点の座標で角度を算出する
	float ResultRot = 0.0f;		// 計算結果を入れる変数
	float Z = posA.z - posB.z;	// Zの大きさを測る
	float X = posA.x - posB.x;	// Xの大きさを測る
	ResultRot = atan2f(X, Z);	// 計算

	return ResultRot;
}

void CCalculation::ResetRot(float& rot)
{
	if (rot > D3DX_PI /*|| rot.y <= -D3DX_PI*/)
	{
		rot = -D3DX_PI;
	}
	if (rot < -D3DX_PI /*|| rot.y <= -D3DX_PI*/)
	{
		rot = D3DX_PI;
	}
}

//===========================================================================================================
// 割合計算
//===========================================================================================================
float CCalculation::Ratio(float Max, float Curr)
{
	float Ratio = Curr / Max;

	return Ratio;
}


//===========================================================================================================
// 割合計算
//===========================================================================================================
D3DXVECTOR3 CCalculation::ShiftPosition(D3DXVECTOR3 pos,D3DXVECTOR3 rot, D3DXVECTOR2 shiftsize, ROT_AXIS axis)
{
	D3DXVECTOR3 resultpos = pos;
	float fRot = 0.0f;
	float fPos_1 = 0.0f;
	float fPos_2 = 0.0f;

	// 対角線
	float fLength = sqrtf(shiftsize.x * shiftsize.x + shiftsize.y * shiftsize.y);
	// 角度
	float fAngle = atan2f(shiftsize.x, shiftsize.y);

	switch (axis)
	{
	case ROT_X:

		break;

	case ROT_Y:
		// 位置を設定
		resultpos.x += sinf(rot.y - fAngle) * fLength;
		resultpos.z += cosf(rot.y - fAngle) * fLength;

		break;

	case ROT_Z:

		break;
	}

	//// 位置を設定
	//resultpos.x += sinf(rot.y - fAngle) * fLength;
	//resultpos.z += cosf(rot.y - fAngle) * fLength;

	return resultpos;
}

//===========================================================================================================
// 2点間の距離を計算する
//===========================================================================================================
float CCalculation::TwoPointDistance(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	float LengthX = posA.x - posB.x;
	float LengthY = posA.z - posB.z;

	float ResultDis = sqrtf(LengthX * LengthX + LengthY * LengthY);

	return ResultDis;
}

float CCalculation::TwoPointDistanceHeight(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	float LengthX = posA.x - posB.x;
	float LengthY = posA.y - posB.y;

	float ResultDis = sqrtf(LengthX * LengthX + LengthY * LengthY);

	return ResultDis;
}

//===========================================================================================================
// ランダムな整数を生成
//===========================================================================================================
uint64_t CCalculation::RandomInt(uint64_t min, uint64_t max)
{
	static std::mt19937_64 mt64(0);
	std::uniform_int_distribution<uint64_t>randint(min, max);

	return randint(mt64);
}
