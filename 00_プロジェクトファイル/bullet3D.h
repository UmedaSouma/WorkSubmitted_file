#pragma once
//===========================================================================================================================================================
// 
// bullet3D.cpp�̃w�b�_�[ [bullet3D.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BULLET_3D_H_
#define _BULLET_3D_H_
#include "main.h"
#include "model.h"

class CBullet3D :public CModel
{
public:
	CBullet3D();
	~CBullet3D()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	void UpdateType();	// ��ނ��Ƃ� Update
	void UpdateAlly();	// ������ Update
	void UpdateEnemy();	// �G�� Update
	int Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// �����蔻��

	static CBullet3D* Create(CObject::TYPE type, D3DXVECTOR3 pos,D3DXVECTOR3 move,int life);   // ��������
private:
	int m_nLife;			// �e�̎���
	D3DXVECTOR3 m_move;		// �ړ���
	CObject::TYPE m_type;		// �e�𔭎˂������̃��f���̃^�C�v
};

#endif // !BULLET_3D_H_