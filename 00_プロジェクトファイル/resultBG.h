#pragma once
//===========================================================================================================================================================
// 
// resultBG.cppのヘッダー [resultBG.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _RESULT_BG_H_
#define _RESULT_BG_H_
#include "main.h"
#include "object2D.h"

class CResultBG :public CObject2D
{
public:
	CResultBG(int nPriority = 4);
	~CResultBG()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CResultBG* Create();   // 生成処理
private:

};


#endif // !TITLE_BG_H_