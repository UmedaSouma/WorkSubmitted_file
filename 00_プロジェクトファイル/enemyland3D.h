//===========================================================================================================================================================
// 
// enemyland3D.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ENEMYLAND3D_H_
#define _ENEMYLAND3D_H_
#include "main.h"
#include "enemy3D.h"

//===========================================================================================================
// 陸エネミークラス
//===========================================================================================================
class CEnemyland3D :public CEnemy3D
{
public:
	CEnemyland3D();
	~CEnemyland3D()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	void Gravity();		// 重力の処理

	static CEnemyland3D* Create(D3DXVECTOR3 pos);	// 生成処理

	void SetSwitchGravity(bool Zero) { m_bZeroGravity = Zero; }
private:
	static inline const float m_fGravity = 0.3f;	// 重力
	float m_fMove;	// 重力を加算するためだけの move 変数
	bool m_bZeroGravity;	// true のとき無重力
};

#endif // !ENEMYLAND3D_H_