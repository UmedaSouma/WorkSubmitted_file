#pragma once
//===========================================================================================================================================================
// 
// slime_boss.cppのヘッダー [slime_boss.h]
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
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	void SelectAttackState();	// 攻撃状態の選択
	void UpdateAttack();	// 攻撃更新(攻撃時の動き)
	void Attack_Rush();		// 体当たり攻撃
	void Attack_Jump();		// ジャンプ攻撃
	void Attack_Beam();		// ビーム攻撃

	void SetBossAppear(bool Appear) { m_bIsAppear = Appear; }
	bool GetBossAppear() { return m_bIsAppear; }

	static CSlimeBoss* Create(D3DXVECTOR3 pos);   // 生成処理

private:
	STATE_ATTACK_BOSS_SLIME m_StateAttack;	// 攻撃の状態
	int m_nStateNoneCnt;	// 無の状態をカウント
	int m_nStateAttackCnt;	// 攻撃自体のカウント
	bool m_bIsAppear;	// 出現の有無
	int m_nTurn;		// 攻撃の挙動を決めるターン
	CDangerZone* m_pDangerZone;	// 危険範囲のポインタ
	D3DXVECTOR3 m_OriginPos;	// 原点の
	bool m_FirstApper;
	


};

#endif // !SLIME_BOSS_H_