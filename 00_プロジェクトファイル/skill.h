//===========================================================================================================================================================
// 
// skill.cpp�̃w�b�_�[ [skill.h]
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
		SKILL_TRIGGER,		// �X�L���{�^���������ꂽ�u��
		SKILL_PRESS,		// ������Ă����
		SKILL_RELEASE,		// �����ꂽ�Ƃ�
		SKILL_CANCELL,		// �X�L������
		SKILL_SWORD_BEFORE, // ���X�L�������O
		SKILL_SWORD,		// ���X�L������
		SKILL_SWORD_CANCEL,	// ���X�L������
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
	SKILL_INPUT_STATE m_InputState;	// �X�L���{�^���̏��
	int m_nDurationtime_Skill;		// �X�L���̌p������(�t���[����)
	int m_nDurationtime_Sword;		// ���X�L���̌p������(�t���[����)
	int m_nCooltime_Skill;			// �ʏ�X�L���̃N�[���^�C��(�t���[����)
	int m_nCooltime_Sword;			// ���X�L���̃N�[���^�C��(�t���[����)
	bool m_bUseSkill;				// �X�L���g�p�����ǂ���(true �g�p��
	bool m_bUseSword;				// ���X�L���g�p�����ǂ���(true �g�p��
	bool m_bUsePossibleSkill;		// �X�L���g�p�\���ǂ���(true �g�p�\
	bool m_bUsePossibleSword;		// ���X�L���g�p�\���ǂ���(true �g�p�\

public:
	// set.get

	// ���
	void SetInputSkillState(SKILL_INPUT_STATE state) { m_InputState = state; }
	SKILL_INPUT_STATE GetInputSkillState() { return m_InputState; }

	// �ʏ�X�L��
	void SetSkillDurationtime(int time) { m_nDurationtime_Skill = time; }
	int GetSkillDurationtime() { return m_nDurationtime_Skill; }
	void SetSkillCooltime(int time) { m_nCooltime_Skill = time; }
	int GetSkillCoolTime() { return m_nCooltime_Skill; }
	void ActSkill() { m_bUseSkill = true; }
	bool GetActSkill() { return m_bUseSkill; }

	// ���X�L��
	void SetSwordDurationtime(int time) { m_nDurationtime_Sword = time; }
	int GetSwordDurationtime() { return m_nDurationtime_Sword; }
	void SetSwordCooltime(int time) { m_nCooltime_Sword = time; }
	int GetSwordCoolTime() { return m_nCooltime_Sword; }
	void ActSword(bool buse) { m_bUseSword = buse; }
	bool GetActSword() { return m_bUseSword; }
	bool GetPossibleSword() { return m_bUsePossibleSword; }

};



#endif // !SKILL_H_