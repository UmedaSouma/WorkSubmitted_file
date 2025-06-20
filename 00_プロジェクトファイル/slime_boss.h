#pragma once
//===========================================================================================================================================================
// 
// slime_boss.cppÌwb_[ [slime_boss.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SLIME_BOSS_H_
#define _SLIME_BOSS_H_
#include "main.h"
#include "enemy3D.h"
#include "enemyland3D.h"
#include "dangerzone.h"


class CSlimeBoss :public CEnemyland3D
{
public:
	typedef enum
	{
		ATTACK_NONE=0,
		ATTACK_RUSH,
		ATTACK_JUMP,
		ATTACK_BEAM,
		ATTACK_MAX
	}STATE_ATTACK_BOSS_SLIME;

	CSlimeBoss();
	~CSlimeBoss()override;
	HRESULT Init()override;	// úÝè
	void Uninit()override;	// I¹
	void Update()override;	// XV
	void Draw()override;	// `æ

	void SelectAttackState();	// UóÔÌIð
	void UpdateAttack();	// UXV(UÌ®«)
	void Attack_Rush();		// Ì½èU
	void Attack_Jump();		// WvU
	void Attack_Beam();		// r[U

	void SetBossAppear(bool Appear) { m_bIsAppear = Appear; }
	bool GetBossAppear() { return m_bIsAppear; }

	static CSlimeBoss* Create(D3DXVECTOR3 pos);   // ¶¬

private:
	STATE_ATTACK_BOSS_SLIME m_StateAttack;	// UÌóÔ
	int m_nStateNoneCnt;	// ³ÌóÔðJEg
	int m_nStateAttackCnt;	// U©ÌÌJEg
	bool m_bIsAppear;	// o»ÌL³
	int m_nTurn;		// UÌ®ðßé^[
	CDangerZone* m_pDangerZone;	// ë¯ÍÍÌ|C^
	D3DXVECTOR3 m_OriginPos;	// ´_Ì
	bool m_FirstApper;
	


};

#endif // !SLIME_BOSS_H_