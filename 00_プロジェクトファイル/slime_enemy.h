//===========================================================================================================================================================
// 
// slime_enemy.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SLIME_ENEMY_H_
#define _SLIME_ENEMY_H_
#include "main.h"
#include "slime.h"

class CSlimeEnemy :public CSlime
{
public:
	CSlimeEnemy();
	~CSlimeEnemy()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CSlimeEnemy* Create(D3DXVECTOR3 pos);   // 生成処理
private:

};

#endif // !SLIME_ENEMY_H_