#pragma once
//===========================================================================================================================================================
// 
// UI.cppのヘッダー [UI.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _UI_H_
#define _UI_H_
#include "main.h"
#include "object2D.h"

class CUI :public CObject2D
{
public:
	CUI(int nPriority = 4);
	~CUI()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画
	void DeleteUI() { m_nLife = 0; }

	static CUI* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int life, const char* filepass);   // 生成処理

	// 寿命の設定
	void SetLife(int life) { m_nLife = life; }
	int GetLife() { return m_nLife; }

	// 表示の設定
	void SetUse(bool use) { m_bUse = use; }
	bool GetUse() { return m_bUse; }
private:
	int m_nLife;	// 寿命
	bool m_Isblink;	// 点滅するか否か
	bool m_bUse;	// 表示しているかどうか 
};

#endif // !UI_H_