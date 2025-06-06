//===========================================================================================================================================================
// 
// skill.cppのヘッダー [skill.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SKILL_H_
#define _SKILL_H_
#include "main.h"

class CSkill
{
public:
	typedef enum
	{
		SKILL_NONE = 0,
		SKILL_TRIGGER,		// スキルボタンが押された瞬間
		SKILL_PRESS,		// 押されている間
		SKILL_RELEASE,		// 離されたとき
		SKILL_CANCELL,		// スキル解除
		SKILL_SWORD_BEFORE, // 剣スキル発動前
		SKILL_SWORD,		// 剣スキル発動
		SKILL_SWORD_CANCEL,	// 剣スキル解除
		SKILL_MAX
	}SKILL_INPUT_STATE;

	CSkill();
	~CSkill();
	void Init();
	void Uninit();
	void Update();
	void StateUpdate();
	void SwordUpdate();

private:
	SKILL_INPUT_STATE m_InputState;	// スキルボタンの状態
	int m_nDurationtime_Skill;		// スキルの継続時間(フレーム数)
	int m_nDurationtime_Sword;		// 剣スキルの継続時間(フレーム数)
	int m_nCooltime_Skill;			// 通常スキルのクールタイム(フレーム数)
	int m_nCooltime_Sword;			// 剣スキルのクールタイム(フレーム数)
	bool m_bUseSkill;				// スキル使用中かどうか(true 使用中
	bool m_bUseSword;				// 剣スキル使用中かどうか(true 使用中
	bool m_bUsePossibleSkill;		// スキル使用可能かどうか(true 使用可能
	bool m_bUsePossibleSword;		// 剣スキル使用可能かどうか(true 使用可能

public:
	// set.get

	// 状態
	void SetInputSkillState(SKILL_INPUT_STATE state) { m_InputState = state; }
	SKILL_INPUT_STATE GetInputSkillState() { return m_InputState; }

	// 通常スキル
	void SetSkillDurationtime(int time) { m_nDurationtime_Skill = time; }
	int GetSkillDurationtime() { return m_nDurationtime_Skill; }
	void SetSkillCooltime(int time) { m_nCooltime_Skill = time; }
	int GetSkillCoolTime() { return m_nCooltime_Skill; }
	void ActSkill() { m_bUseSkill = true; }
	bool GetActSkill() { return m_bUseSkill; }

	// 剣スキル
	void SetSwordDurationtime(int time) { m_nDurationtime_Sword = time; }
	int GetSwordDurationtime() { return m_nDurationtime_Sword; }
	void SetSwordCooltime(int time) { m_nCooltime_Sword = time; }
	int GetSwordCoolTime() { return m_nCooltime_Sword; }
	void ActSword(bool buse) { m_bUseSword = buse; }
	bool GetActSword() { return m_bUseSword; }
	bool GetPossibleSword() { return m_bUsePossibleSword; }

};



#endif // !SKILL_H_