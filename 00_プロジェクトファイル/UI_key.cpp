//===========================================================================================================================================================
// 
// �L�[��UI�̏��� [UI_key.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "UI_key.h"
#include "texture.h"
#include "manager.h"
#include "input.h"

//========================================================================================================================
// �R���X�g���N�^
//========================================================================================================================
CUIKey::CUIKey() :
	m_skilltype(CUISkill::UI_SKILL_NORMAL)
	, m_pUISkill(nullptr)
	, m_UIJoy(false)
{
}

//========================================================================================================================
// �f�X�g���N�^
//========================================================================================================================
CUIKey::~CUIKey()
{
}

//========================================================================================================================
// �����ݒ�
//========================================================================================================================
HRESULT CUIKey::Init()
{
	CUI::Init();

	return S_OK;
}

//========================================================================================================================
// �I������
//========================================================================================================================
void CUIKey::Uninit()
{
	CUI::Uninit();
}

//========================================================================================================================
// �X�V����
//========================================================================================================================
void CUIKey::Update()
{
	// �\����Ԃ�e�̃|�C���^�ƈꏏ�ɂ���
	SetUse(m_pUISkill->GetUse());

	CUI::Update();

	UpdateUIType();

	//CUI::Draw();
}

//========================================================================================================================
// �`�揈��
//========================================================================================================================
void CUIKey::Draw()
{
	CUI::Draw();
}

//===========================================================================================================
// UI�̃^�C�v���X�V����
//===========================================================================================================
void CUIKey::UpdateUIType()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	// �L�[���� for ��
	for (int n = 0; n < MAX_KEY; n++)
	{
		// �L�[�{�[�h�̂ǂꂩ�������ꂽ��
		if (keyboard->GetTrigger(n))
		{
			m_UIJoy = false;	// �W���C�p�b�h��UI������
		}
		else if (keyboard->GetPress(n))
		{
			m_UIJoy = false;	// �W���C�p�b�h��UI������
		}
	}

	// �W���C�p�b�h�̃{�^������ for ��
	for (int n = 0; n < CInputJoypad::JOYKEY_MAX; n++)
	{
		// �{�^���̂ǂꂩ�������ꂽ�Ƃ�
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
// �L�[�{�[�h��UI��ݒ肷��
//===========================================================================================================
void CUIKey::SetKey()
{
	int nIdx = 0;
	CTexture* pTexData = CManager::GetTexture();

	// ��ނ��ƂɃe�N�X�`����ς���
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
// �W���C�p�b�h��UI��ݒ肷��
//===========================================================================================================
void CUIKey::SetJoyKey()
{
	int nIdx = 0;
	CTexture* pTexData = CManager::GetTexture();

	//===========================================================================================================
	// < debugmemo >
	// �t�@�C���̃p�X��z��̒��ɓ���� for ������
	//===========================================================================================================

	// ��ނ��Ƃ̃e�N�X�`����ݒ肷��
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
// �L�[�̃e�N�X�`����ݒ�
//===========================================================================================================
void CUIKey::KeySetTex(CUISkill::UI_SKILL skilltype)
{
	int nIdx = 0;
	CTexture* pTexData = CManager::GetTexture();

	// ��ނ��Ƃ̃e�N�X�`����ݒ肷��
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
// ��������
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
