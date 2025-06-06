//===========================================================================================================================================================
// 
// golem.cppのヘッダー [golem.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#pragma once
#ifndef _GOLEM_H_
#define _GOLEM_H_
#include "main.h"
#include "enemyland3D.h"

class CGolem:public CEnemyland3D
{
public:
	CGolem();
	~CGolem()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CGolem* Create(D3DXVECTOR3 pos);   // 生成処理
private:

};

#endif // !GOLEM_H_