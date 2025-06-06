//===========================================================================================================================================================
// 
// �X�L���̏�� [skill.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "skill.h"

CSkill::CSkill():
	m_InputState(SKILL_NONE)
	,m_nCooltime_Skill(0)
	,m_nCooltime_Sword(0)
	,m_nDurationtime_Skill(0)
	,m_nDurationtime_Sword(0)
	, m_bUseSkill(false)
	, m_bUseSword(false)
	, m_bUsePossibleSword(true)
	, m_bUsePossibleSkill(true)
{
}

CSkill::~CSkill()
{
}

void CSkill::Init()
{

}
void CSkill::Uninit()
{

}
void CSkill::Update()
{
	//if (m_InputState == SKILL_SWORD_BEFORE)
	//{// ����������؂�ւ���
	//	m_InputState = SKILL_SWORD;
	//}

	if (m_InputState == SKILL_SWORD)
	{// �p����,�g�p���Ă��邱�Ƃɂ���
		m_bUseSword = true;
	}

	if (m_nDurationtime_Skill > 0)
	{
		m_nDurationtime_Skill--;
	}

	if (m_nCooltime_Skill > 0)
	{// �N�[���^�C�����c���Ă����Ԃ̎�

		m_nCooltime_Skill--;	// �N�[���^�C�������X�Ɍ��炷
	}
	else
	{// �N�[���^�C�����ݒ肳��Ă��Ȃ��Ƃ�

		m_bUseSkill = false;	// �X�L�����g����悤�ɂ���
	}

	SwordUpdate();

	StateUpdate();
	
}

void CSkill::StateUpdate()
{
	switch(m_InputState)
	{
	case SKILL_SWORD_CANCEL:
		m_bUseSword = false;
		m_nDurationtime_Sword = 0;
		m_bUsePossibleSword = false;
		m_nCooltime_Sword = 300;
		break;
	}
}

void CSkill::SwordUpdate()
{
	switch (m_bUsePossibleSword)
	{
	case true:
		if (m_nDurationtime_Sword >= 0 && m_bUseSword == true)
		{
			if (m_nDurationtime_Sword == 0)
			{
				m_InputState = SKILL_SWORD_CANCEL;
				return;
			}

			m_nDurationtime_Sword--;
		}
		break;

	case false:
		if (m_nCooltime_Sword >= 0 && m_bUsePossibleSword == false)
		{
			if (m_nCooltime_Sword == 0)
			{
				m_bUsePossibleSword = true;
				return;
			}

			m_nCooltime_Sword--;
		}

		
		break;
	}

	

	
}
