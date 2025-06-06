#pragma once
//===========================================================================================================================================================
// 
// UI_key.cpp�̃w�b�_�[ [UI_key.h]
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
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	void UpdateUIType();
	void SetKey();
	void SetJoyKey();

	void KeySetTex(CUISkill::UI_SKILL skilltype);

	static CUIKey* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CUISkill* uiskill,CUISkill::UI_SKILL skilltype);   // ��������
private:
	CUISkill* m_pUISkill;	// UI�X�L���̃|�C���^
	CUISkill::UI_SKILL m_skilltype;	// UI�̎��
	bool m_UIJoy;	// ���삳��Ă���̂��W���C�p�b�h�̎� true
};



#endif // !UI_KEY_H_