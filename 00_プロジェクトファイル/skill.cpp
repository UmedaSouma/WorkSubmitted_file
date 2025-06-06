//===========================================================================================================================================================
// 
// スキルの情報 [skill.cpp]
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
	//{// 発動時から切り替える
	//	m_InputState = SKILL_SWORD;
	//}

	if (m_InputState == SKILL_SWORD)
	{// 継続時,使用していることにする
		m_bUseSword = true;
	}

	if (m_nDurationtime_Skill > 0)
	{
		m_nDurationtime_Skill--;
	}

	if (m_nCooltime_Skill > 0)
	{// クールタイムが残っている状態の時

		m_nCooltime_Skill--;	// クールタイムを徐々に減らす
	}
	else
	{// クールタイムが設定されていないとき

		m_bUseSkill = false;	// スキルを使えるようにする
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
