#pragma once
//===========================================================================================================================================================
// 
// golem_enemy.cpp�̃w�b�_�[ [golem_enemy.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _GOLEM_ENEMY_H_
#define _GOLEM_ENEMY_H_
#include "main.h"
#include "golem.h"

class CGolemEnemy :public CGolem
{
public:
	CGolemEnemy();
	~CGolemEnemy()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CGolemEnemy* Create(D3DXVECTOR3 pos);   // ��������
private:

};



#endif // !GOLEM_ENEMY_H_