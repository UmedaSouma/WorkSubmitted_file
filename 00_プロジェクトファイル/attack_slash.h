#pragma once
//===========================================================================================================================================================
// 
// attack_slash.cppのヘッダー [attack_slash.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ATTACK_SLASH_H_
#define _ATTACK_SLASH_H_
#include "main.h"
#include "object3D.h"

class CAttackSlash :public CObject3D
{
public:
	static const unsigned int m_SlashDamage = 25;

	CAttackSlash();
	~CAttackSlash()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CAttackSlash* Create(D3DXVECTOR3 pos, int life, bool breturn);   // 生成処理

	void Collision();
	void Knockback();
private:
	int m_nLife;	// 寿命
	D3DXVECTOR3 m_Move;	// 動く速さ
	bool m_bReturn;	// 回転方向
};

#endif // !ATTACK_SLASH_H_
