#pragma once
//===========================================================================================================================================================
// 
// attack_slash.cpp�̃w�b�_�[ [attack_slash.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ATTACK_SLASH_H_
#define _ATTACK_SLASH_H_
#include "main.h"
#include "object3D.h"

class CAttackSlash :public CObject3D
{
public:
	static const unsigned int m_SlashDamage = 25;

	CAttackSlash();
	~CAttackSlash()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CAttackSlash* Create(D3DXVECTOR3 pos, int life, bool breturn);   // ��������

	void Collision();
	void Knockback();
private:
	int m_nLife;	// ����
	D3DXVECTOR3 m_Move;	// ��������
	bool m_bReturn;	// ��]����
};

#endif // !ATTACK_SLASH_H_
