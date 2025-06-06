#pragma once
//===========================================================================================================================================================
// 
// UI_skill.cppのヘッダー [UI_skill.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _UI_SKILLH_
#define _UI_SKILL_H_
#include "main.h"
#include "UI.h"

class CUISkill :public CUI
{
public:
	static unsigned const int MAX_SIZE = 130;

	typedef enum
	{
		UI_SKILL_NORMAL=0,
		//UI_SKILL_SWORD,
		//UI_SKILL_SWORD_2,
		UI_SKILL_CANSELL,
		UI_SKILL_ATTACK,
		UI_SKILL_PREV,
		UI_SKILL_NEXT,
		UI_SKILL_MAX
	}UI_SKILL;

	CUISkill();
	~CUISkill()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	void TypeInit();	// 種類ごとの初期設定
	void FirstSkillSet();	// スキルの UI の表示
	void UpdateDisplay();	// 表示の更新
	void SetUICancel();		// キャンセルのみ表示の設定
	void SetUISkill();		// SKILL 使用時の表示

	static CUISkill* Create(UI_SKILL type);   // 生成処理
private:
	UI_SKILL m_type;	// UIの種類
	bool m_bFirstSkill;	// 初回のスキル使用
};



#endif // !UI_SKILL_H_