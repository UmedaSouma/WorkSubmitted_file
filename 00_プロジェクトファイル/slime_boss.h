#pragma once
//===========================================================================================================================================================
// 
// slime_boss.cpp�̃w�b�_�[ [slime_boss.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SLIME_BOSS_H_
#define _SLIME_BOSS_H_
#include "main.h"
#include "enemy3D.h"
#include "enemyland3D.h"
#include "dangerzone.h"


class CSlimeBoss :public CEnemyland3D
{
public:
	typedef enum
	{
		ATTACK_NONE=0,
		ATTACK_RUSH,
		ATTACK_JUMP,
		ATTACK_BEAM,
		ATTACK_MAX
	}STATE_ATTACK_BOSS_SLIME;

	CSlimeBoss();
	~CSlimeBoss()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	void SelectAttackState();	// �U����Ԃ̑I��
	void UpdateAttack();	// �U���X�V(�U�����̓���)
	void Attack_Rush();		// �̓�����U��
	void Attack_Jump();		// �W�����v�U��
	void Attack_Beam();		// �r�[���U��

	void SetBossAppear(bool Appear) { m_bIsAppear = Appear; }
	bool GetBossAppear() { return m_bIsAppear; }

	static CSlimeBoss* Create(D3DXVECTOR3 pos);   // ��������

private:
	STATE_ATTACK_BOSS_SLIME m_StateAttack;	// �U���̏��
	int m_nStateNoneCnt;	// ���̏�Ԃ��J�E���g
	int m_nStateAttackCnt;	// �U�����̂̃J�E���g
	bool m_bIsAppear;	// �o���̗L��
	int m_nTurn;		// �U���̋��������߂�^�[��
	CDangerZone* m_pDangerZone;	// �댯�͈͂̃|�C���^
	D3DXVECTOR3 m_OriginPos;	// ���_��
	bool m_FirstApper;
	


};

#endif // !SLIME_BOSS_H_