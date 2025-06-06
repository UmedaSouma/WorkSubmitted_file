//===========================================================================================================================================================
// 
// golem.cpp�̃w�b�_�[ [golem.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#pragma once
#ifndef _GOLEM_H_
#define _GOLEM_H_
#include "main.h"
#include "enemyland3D.h"

class CGolem:public CEnemyland3D
{
public:
	CGolem();
	~CGolem()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CGolem* Create(D3DXVECTOR3 pos);   // ��������
private:

};

#endif // !GOLEM_H_