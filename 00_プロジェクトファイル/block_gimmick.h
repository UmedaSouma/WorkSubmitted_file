#pragma once
//===========================================================================================================================================================
// 
// block_gimmick.cpp�̃w�b�_�[ [block_gimmick.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BLOCK_GIMMICK_H_
#define _BLOCK_GIMMICK_H_
#include "main.h"
#include "block3D.h"

class CBlockGimmick:public CBlock3D
{
public:
	CBlockGimmick();
	~CBlockGimmick()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CBlockGimmick* Create(D3DXVECTOR3 pos/*, D3DXVECTOR3 size*/);   // ��������
private:
	bool m_Gimmick;
};



#endif // !BLOCK_GIMMICK_H_
