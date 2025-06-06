//===========================================================================================================================================================
// 
// ŒvŽZ [calculation.cpp]
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
// Šp“x•â³ŒvŽZ
//===========================================================================================================
float CCalculation::FixAngle(float Angle, float TargetAngle)
{
	// –Ú“I‚ÌŠp“x‚ÆŒ»Ý‚ÌŠp“x‚Ì·•ª
	float Diff = TargetAngle - Angle;

	// ·‚ª }ƒÎ ‚¾‚Á‚½‚ç•â³‚ð‚·‚é
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
	// 2“_‚ÌÀ•W‚ÅŠp“x‚ðŽZo‚·‚é
	float ResultRot = 0.0f;		// ŒvŽZŒ‹‰Ê‚ð“ü‚ê‚é•Ï”
	float Z = posA.z - posB.z;	// Z‚Ì‘å‚«‚³‚ð‘ª‚é
	float X = posA.x - posB.x;	// X‚Ì‘å‚«‚³‚ð‘ª‚é
	ResultRot = atan2f(X, Z);	// ŒvŽZ

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
// Š„‡ŒvŽZ
//===========================================================================================================
float CCalculation::Ratio(float Max, float Curr)
{
	float Ratio = Curr / Max;

	return Ratio;
}


//===========================================================================================================
// Š„‡ŒvŽZ
//===========================================================================================================
D3DXVECTOR3 CCalculation::ShiftPosition(D3DXVECTOR3 pos,D3DXVECTOR3 rot, D3DXVECTOR2 shiftsize, ROT_AXIS axis)
{
	D3DXVECTOR3 resultpos = pos;
	float fRot = 0.0f;
	float fPos_1 = 0.0f;
	float fPos_2 = 0.0f;

	// ‘ÎŠpü
	float fLength = sqrtf(shiftsize.x * shiftsize.x + shiftsize.y * shiftsize.y);
	// Šp“x
	float fAngle = atan2f(shiftsize.x, shiftsize.y);

	switch (axis)
	{
	case ROT_X:

		break;

	case ROT_Y:
		// ˆÊ’u‚ðÝ’è
		resultpos.x += sinf(rot.y - fAngle) * fLength;
		resultpos.z += cosf(rot.y - fAngle) * fLength;

		break;

	case ROT_Z:

		break;
	}

	//// ˆÊ’u‚ðÝ’è
	//resultpos.x += sinf(rot.y - fAngle) * fLength;
	//resultpos.z += cosf(rot.y - fAngle) * fLength;

	return resultpos;
}

//===========================================================================================================
// 2“_ŠÔ‚Ì‹——£‚ðŒvŽZ‚·‚é
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
// ƒ‰ƒ“ƒ_ƒ€‚È®”‚ð¶¬
//===========================================================================================================
uint64_t CCalculation::RandomInt(uint64_t min, uint64_t max)
{
	static std::mt19937_64 mt64(0);
	std::uniform_int_distribution<uint64_t>randint(min, max);

	return randint(mt64);
}
