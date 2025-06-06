#pragma once
//===========================================================================================================================================================
// 
// UI_key.cppのヘッダー [UI_key.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _UI_KEY_H_
#define _UI_KEY_H_
#include "main.h"
#include "UI.h"
#include "UI_skill.h"

class CUIKey:public CUI
{
public:
	CUIKey();
	~CUIKey()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	void UpdateUIType();
	void SetKey();
	void SetJoyKey();

	void KeySetTex(CUISkill::UI_SKILL skilltype);

	static CUIKey* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CUISkill* uiskill,CUISkill::UI_SKILL skilltype);   // 生成処理
private:
	CUISkill* m_pUISkill;	// UIスキルのポインタ
	CUISkill::UI_SKILL m_skilltype;	// UIの種類
	bool m_UIJoy;	// 操作されているのがジョイパッドの時 true
};



#endif // !UI_KEY_H_