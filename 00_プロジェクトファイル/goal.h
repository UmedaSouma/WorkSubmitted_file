//===========================================================================================================================================================
// 
// goal.cppのヘッダー [goal.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _GOAL_H_
#define _GOAL_H_
#include "main.h"
#include "model.h"

class CGoal :CModel
{
public:
	CGoal();
	~CGoal()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CGoal* Create(D3DXVECTOR3 pos);   // 生成処理
private:

};
#endif // !GOAL_H_