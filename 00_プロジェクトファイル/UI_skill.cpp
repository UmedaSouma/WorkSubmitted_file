//===========================================================================================================================================================
// 
// スキルUIの描画処理 [UI_skill.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "UI_skill.h"
#include "texture.h"
#include "manager.h"
#include "search.h"
#include "player3D.h"
#include "enemy3D.h"
#include "UI_key.h"

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CUISkill::CUISkill() :
	m_type(UI_SKILL_NORMAL)
	, m_bFirstSkill(false)
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CUISkill::~CUISkill()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CUISkill::Init()
{
	TypeInit();

	CUI::Init();

	return S_OK;
}

//===========================================================================================================
// 種類ごとの初期設定
//===========================================================================================================
void CUISkill::TypeInit()
{
	int nIdx = 0;
	CTexture* pTexData = CManager::GetTexture();
	D3DXVECTOR3 pos = { 0.0f,0.0f ,0.0f };
	D3DXVECTOR3 keypos = { 0.0f,0.0f ,0.0f };
	float f = 0.0f;

	// 全ての UI を非表示にする
	SetUse(false);

	switch (m_type)
	{
	case UI_SKILL_NORMAL:

		nIdx = pTexData->Regist("data\\TEXTURE\\ui_enemy_000.png");
		pos = { SCREEN_WIDTH * 0.92f,SCREEN_HEIGHT * 0.9f,0.0f };
		
		break;

	//case UI_SKILL_SWORD:

	//	nIdx = pTexData->Regist("data\\TEXTURE\\ui_enemy_sword_000.png");
	//	pos = { SCREEN_WIDTH * 0.78f,SCREEN_HEIGHT * 0.9f,0.0f };

	//	f = -25.0f;

	//	break;

	//case UI_SKILL_SWORD_2:
	//	nIdx = pTexData->Regist("data\\TEXTURE\\ui_enemy_sword_000.png");
	//	pos = { SCREEN_WIDTH * 0.78f,SCREEN_HEIGHT * 0.9f,0.0f };

	//	f = 25.0f;
	//	break;

	case UI_SKILL_CANSELL:

		nIdx = pTexData->Regist("data\\TEXTURE\\ui_enemy_000.png");
		pos = { SCREEN_WIDTH * 0.85f,SCREEN_HEIGHT * 0.9f,0.0f };

		break;

	case UI_SKILL_ATTACK:

		nIdx = pTexData->Regist("data\\TEXTURE\\ui_sword_000.png");
		pos = { SCREEN_WIDTH * 0.85f,SCREEN_HEIGHT * 0.7f,0.0f };
		SetUse(true);

		break;

	case UI_SKILL_PREV:

		nIdx = pTexData->Regist("data\\TEXTURE\\joykey_LB.png");
		pos = { SCREEN_WIDTH * 0.4f,SCREEN_HEIGHT * 0.9f,0.0f };

		break;

	case UI_SKILL_NEXT:

		nIdx = pTexData->Regist("data\\TEXTURE\\joykey_RB.png");
		pos = { SCREEN_WIDTH * 0.6f,SCREEN_HEIGHT * 0.9f,0.0f };

		break;
	}

	// 付属キーの pos 設定
	keypos = { pos.x+f,pos.y - 60.0f,pos.z };
	CUIKey::Create(keypos, 
		{ 50.0f,50.0f,50.0f },
		this,
		m_type);

	SetPos(pos);
	SetSize({ MAX_SIZE,MAX_SIZE,0.0f });

	BindTexture(pTexData->GetAddress(nIdx));

	SetLife(-1);

}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CUISkill::Uninit()
{
	CUI::Uninit();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CUISkill::Update()
{
	CUI::Update();

	UpdateDisplay();

	if (!m_bFirstSkill)
	{
		FirstSkillSet();
	}

	//if (m_type == UI_SKILL_SWORD_2)
	//{
	//	SetColor({ 1.0f,1.0f, 1.0f, 0.0f });
	//}
}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CUISkill::Draw()
{
	if (/*m_type != UI_SKILL_SWORD_2*/
		m_type != UI_SKILL_PREV
		&& m_type != UI_SKILL_NEXT)
	{
		CUI::Draw();
	}
	
}

//===========================================================================================================
// スキル初使用時
//===========================================================================================================
void CUISkill::FirstSkillSet()
{
	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);

	if (pPlayer != nullptr)
	{
		if (pPlayer->GetFrontTag() != 0)
		{
			m_bFirstSkill = true;
			if (m_type != UI_SKILL_CANSELL)
			{
				SetUse(true);
			}
		}
	}
}

//===========================================================================================================
// それぞれの状態でのUI表示
//===========================================================================================================
void CUISkill::UpdateDisplay()
{
	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, CObject::PLAYER);

	std::vector<CEnemy3D*> pEnemy = {};
	pEnemy = CSearch::SearchMultiObject(pEnemy, TYPE::ENEMY_MESSE);

	CEnemy3D::STATE_ENEMY state = CEnemy3D::STATE_NORMAL;

	for (unsigned int nCntEme = 0; nCntEme < pEnemy.size(); nCntEme++)
	{
		if (pEnemy[nCntEme] != nullptr && pPlayer != nullptr)
		{
			if (pPlayer->GetFrontTag() == pEnemy[nCntEme]->GetTag())
			{
				state = pEnemy[nCntEme]->GetState();

				switch (state)
				{
				case CEnemy3D::STATE_SKILL:
					SetUICancel();
					break;

				case CEnemy3D::STATE_SWORD:
					SetUISkill();
					break;

				case CEnemy3D::STATE_MOVE:
					SetUISkill();
					break;
				}
			}
		}
	}
}

//===========================================================================================================
// キャンセルのみ表示
//===========================================================================================================
void CUISkill::SetUICancel()
{
	switch (m_type)
	{
	case UI_SKILL_NORMAL:
		SetUse(false);

		break;

	//case UI_SKILL_SWORD:
	//	SetUse(false);

	//	break;

	//case UI_SKILL_SWORD_2:
	//	SetUse(false);

	//	break;

	case UI_SKILL_CANSELL:
		SetUse(true);

		break;
	}
}

//===========================================================================================================
// 戦闘スキル表示
//===========================================================================================================
void CUISkill::SetUISkill()
{
	switch (m_type)
	{
	case UI_SKILL_NORMAL:
		SetUse(true);

		break;

	//case UI_SKILL_SWORD:
	//	SetUse(true);

	//	break;

	//case UI_SKILL_SWORD_2:
	//	SetUse(true);

	//	break;

	case UI_SKILL_CANSELL:
		SetUse(false);

		break;
	}
}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CUISkill* CUISkill::Create(UI_SKILL type)
{
	CUISkill* pUISkill = new CUISkill;

	pUISkill->m_type = type;

	pUISkill->Init();

	return pUISkill;
}