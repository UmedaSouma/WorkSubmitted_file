//===========================================================================================================================================================
// 
// slime.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SLAIME_H_
#define _SLAIME_H_
#include "main.h"
#include "enemyland3D.h"

class CSlime :public CEnemyland3D
{
public:
	CSlime();
	~CSlime()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CSlime* Create(D3DXVECTOR3 pos);   // 生成処理

	void Attack();	// 攻撃の処理
private:
	int m_nCntJumptime;	// ジャンプのクールタイム

};



#endif // !SLAIME_H_