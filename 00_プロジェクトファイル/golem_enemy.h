#pragma once
//===========================================================================================================================================================
// 
// golem_enemy.cppのヘッダー [golem_enemy.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _GOLEM_ENEMY_H_
#define _GOLEM_ENEMY_H_
#include "main.h"
#include "golem.h"

class CGolemEnemy :public CGolem
{
public:
	CGolemEnemy();
	~CGolemEnemy()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CGolemEnemy* Create(D3DXVECTOR3 pos);   // 生成処理
private:

};



#endif // !GOLEM_ENEMY_H_