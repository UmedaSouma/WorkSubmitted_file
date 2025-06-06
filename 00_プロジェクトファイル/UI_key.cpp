//===========================================================================================================================================================
// 
// キーのUIの処理 [UI_key.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "UI_key.h"
#include "texture.h"
#include "manager.h"
#include "input.h"

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CUIKey::CUIKey() :
	m_skilltype(CUISkill::UI_SKILL_NORMAL)
	, m_pUISkill(nullptr)
	, m_UIJoy(false)
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CUIKey::~CUIKey()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CUIKey::Init()
{
	CUI::Init();

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CUIKey::Uninit()
{
	CUI::Uninit();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CUIKey::Update()
{
	// 表示状態を親のポインタと一緒にする
	SetUse(m_pUISkill->GetUse());

	CUI::Update();

	UpdateUIType();

	//CUI::Draw();
}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CUIKey::Draw()
{
	CUI::Draw();
}

//===========================================================================================================
// UIのタイプを更新する
//===========================================================================================================
void CUIKey::UpdateUIType()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	// キー数分 for 回す
	for (int n = 0; n < MAX_KEY; n++)
	{
		// キーボードのどれかが押された時
		if (keyboard->GetTrigger(n))
		{
			m_UIJoy = false;	// ジョイパッドのUIを消す
		}
		else if (keyboard->GetPress(n))
		{
			m_UIJoy = false;	// ジョイパッドのUIを消す
		}
	}

	// ジョイパッドのボタン数分 for 回す
	for (int n = 0; n < CInputJoypad::JOYKEY_MAX; n++)
	{
		// ボタンのどれかが押されたとき
		if (joypad->GetTrigger((CInputJoypad::JOYKEY)n))
		{
			m_UIJoy = true;
		}
		if (joypad->GetPress((CInputJoypad::JOYKEY)n))
		{
			m_UIJoy = true;
		}
	}

	if (m_UIJoy)
	{
		SetJoyKey();
	}
	else
	{
		SetKey();
	}
	
}

//===========================================================================================================
// キーボードのUIを設定する
//===========================================================================================================
void CUIKey::SetKey()
{
	int nIdx = 0;
	CTexture* pTexData = CManager::GetTexture();

	// 種類ごとにテクスチャを変える
	switch (m_skilltype)
	{
	case CUISkill::UI_SKILL::UI_SKILL_NORMAL:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_E_000.png");
		break;

	//case CUISkill::UI_SKILL::UI_SKILL_SWORD:
	//	nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_long_shift_000.png");
	//	break;

	//case CUISkill::UI_SKILL::UI_SKILL_SWORD_2:
	//	nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_E_000.png");
	//	break;

	case CUISkill::UI_SKILL::UI_SKILL_ATTACK:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_long_enter_000.png");
		break;

	case CUISkill::UI_SKILL::UI_SKILL_CANSELL:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_Q_000.png");
		break;

	case CUISkill::UI_SKILL::UI_SKILL_PREV:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_1_000.png");
		break;

	case CUISkill::UI_SKILL::UI_SKILL_NEXT:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_2_000.png");
		break;
	}

	BindTexture(pTexData->GetAddress(nIdx));
}

//===========================================================================================================
// ジョイパッドのUIを設定する
//===========================================================================================================
void CUIKey::SetJoyKey()
{
	int nIdx = 0;
	CTexture* pTexData = CManager::GetTexture();

	//===========================================================================================================
	// < debugmemo >
	// ファイルのパスを配列の中に入れて for 文を回す
	//===========================================================================================================

	// 種類ごとのテクスチャを設定する
	switch (m_skilltype)
	{
	case CUISkill::UI_SKILL::UI_SKILL_NORMAL:
		nIdx = pTexData->Regist("data\\TEXTURE\\joykey_Y.png");
		break;

	//case CUISkill::UI_SKILL::UI_SKILL_SWORD:
	//	nIdx = pTexData->Regist("data\\TEXTURE\\joykey_LT.png");
	//	break;

	//case CUISkill::UI_SKILL::UI_SKILL_SWORD_2:
	//	nIdx = pTexData->Regist("data\\TEXTURE\\joykey_Y.png");
	//	break;

	case CUISkill::UI_SKILL::UI_SKILL_ATTACK:
		nIdx = pTexData->Regist("data\\TEXTURE\\joykey_RT.png");
		break;

	case CUISkill::UI_SKILL::UI_SKILL_CANSELL:
		nIdx = pTexData->Regist("data\\TEXTURE\\joykey_LT.png");
		break;

	case CUISkill::UI_SKILL::UI_SKILL_PREV:
		nIdx = pTexData->Regist("data\\TEXTURE\\joykey_LB.png");
		break;

	case CUISkill::UI_SKILL::UI_SKILL_NEXT:
		nIdx = pTexData->Regist("data\\TEXTURE\\joykey_RB.png");
		break;
	}

	BindTexture(pTexData->GetAddress(nIdx));
}

//===========================================================================================================
// キーのテクスチャを設定
//===========================================================================================================
void CUIKey::KeySetTex(CUISkill::UI_SKILL skilltype)
{
	int nIdx = 0;
	CTexture* pTexData = CManager::GetTexture();

	// 種類ごとのテクスチャを設定する
	switch (skilltype)
	{
	case CUISkill::UI_SKILL::UI_SKILL_NORMAL:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_E_000.png");
		break;

	//case CUISkill::UI_SKILL::UI_SKILL_SWORD:
	//	nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_long_shift_000.png");
	//	break;

	//case CUISkill::UI_SKILL::UI_SKILL_SWORD_2:
	//	nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_E_000.png");
	//	break;

	case CUISkill::UI_SKILL::UI_SKILL_ATTACK:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_long_enter_000.png");
		break;

	case CUISkill::UI_SKILL::UI_SKILL_CANSELL:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_Q_000.png");
		break;

	case CUISkill::UI_SKILL::UI_SKILL_PREV:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_1_000.png");
		break;

	case CUISkill::UI_SKILL::UI_SKILL_NEXT:
		nIdx = pTexData->Regist("data\\TEXTURE\\ui_key_square_2_000.png");
		break;
	}

	BindTexture(pTexData->GetAddress(nIdx));
}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CUIKey* CUIKey::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CUISkill* uiskill, CUISkill::UI_SKILL skilltype)
{
	CUIKey* pUIkey = new CUIKey;

	pUIkey->SetPos(pos);
	pUIkey->SetSize(size);

	pUIkey->m_pUISkill = uiskill;
	pUIkey->KeySetTex(skilltype);
	pUIkey->m_skilltype = skilltype;

	pUIkey->Init();

	return pUIkey;
}
