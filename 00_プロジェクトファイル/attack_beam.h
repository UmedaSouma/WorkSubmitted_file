#pragma once
//===========================================================================================================================================================
// 
// attack_beam.cppのヘッダー [attack_beam.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ATTACK_BEAM_H_
#define _ATTACK_BEAM_H_
#include "main.h"
#include "model.h"

class CAttackBeam :public CModel
{
public:
	static const unsigned int m_BeamDamage = 60;

	CAttackBeam();
	~CAttackBeam()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CAttackBeam* Create(D3DXVECTOR3 pos,int life,bool breturn);   // 生成処理
	
	void Collision();
private:
	int m_nLife;	// 寿命
	D3DXVECTOR3 m_Move;	// 動く速さ
	bool m_bReturn;	// 回転方向
};


#endif // !ATTACK_BEAM_H_