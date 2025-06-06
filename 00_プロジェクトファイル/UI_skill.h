#pragma once
//===========================================================================================================================================================
// 
// UI_skill.cpp�̃w�b�_�[ [UI_skill.h]
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
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	void TypeInit();	// ��ނ��Ƃ̏����ݒ�
	void FirstSkillSet();	// �X�L���� UI �̕\��
	void UpdateDisplay();	// �\���̍X�V
	void SetUICancel();		// �L�����Z���̂ݕ\���̐ݒ�
	void SetUISkill();		// SKILL �g�p���̕\��

	static CUISkill* Create(UI_SKILL type);   // ��������
private:
	UI_SKILL m_type;	// UI�̎��
	bool m_bFirstSkill;	// ����̃X�L���g�p
};



#endif // !UI_SKILL_H_