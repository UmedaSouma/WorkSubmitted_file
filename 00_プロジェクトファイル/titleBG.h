#pragma once
//===========================================================================================================================================================
// 
// titleBG.cppのヘッダー [titleBG.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _TITLE_BG_H_
#define _TITLE_BG_H_
#include "main.h"
#include "object2D.h"

class CTitleBG :public CObject2D
{
public:
	CTitleBG(int nPriority=4);
	~CTitleBG()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CTitleBG* Create();   // 生成処理
private:

};


#endif // !TITLE_BG_H_