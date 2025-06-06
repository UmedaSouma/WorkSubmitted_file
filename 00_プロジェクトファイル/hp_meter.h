#pragma once
//===========================================================================================================================================================
// 
// hp_meter.cpp�̃w�b�_�[ [hp_meter.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _HP_METER_H_
#define _HP_METER_H_
#include "main.h"
#include "UI.h"

//===========================================================================================================
// HP�o�[�̃N���X
//===========================================================================================================
class CHpMeter :public CUI
{
public:
	// HP�o�[�̃^�C�v
	typedef enum
	{
		HP_LITERAL = 0,	// �s�ς�HP
		HP_VARIABLE,	// �ς�HP
		HP_MAX
	}HP_TYPE;

	CHpMeter();	// �R���X�g���N�^
	~CHpMeter()override;	// �f�X�g���N�^
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CHpMeter* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,HP_TYPE type,TYPE chartype);   // ��������
private:
	HP_TYPE m_hptype;	// hitpoint�o�[�̃^�C�v
	TYPE m_CharType;	// HP�̎�����̃^�C�v
};

#endif // !HP_METER_H_
