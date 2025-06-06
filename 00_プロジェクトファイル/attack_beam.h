#pragma once
//===========================================================================================================================================================
// 
// attack_beam.cpp�̃w�b�_�[ [attack_beam.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ATTACK_BEAM_H_
#define _ATTACK_BEAM_H_
#include "main.h"
#include "model.h"

class CAttackBeam :public CModel
{
public:
	static const unsigned int m_BeamDamage = 60;

	CAttackBeam();
	~CAttackBeam()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CAttackBeam* Create(D3DXVECTOR3 pos,int life,bool breturn);   // ��������
	
	void Collision();
private:
	int m_nLife;	// ����
	D3DXVECTOR3 m_Move;	// ��������
	bool m_bReturn;	// ��]����
};


#endif // !ATTACK_BEAM_H_