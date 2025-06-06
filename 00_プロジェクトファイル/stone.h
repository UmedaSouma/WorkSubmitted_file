//===========================================================================================================================================================
// 
// stone.cpp�̃w�b�_�[ [stone.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _STONE_H_
#define _STONE_H_
#include "main.h"
#include "block3D.h"
#include "manager.h"


class CStone :CBlock3D
{
public:
	CStone();
	~CStone()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CStone* Create(D3DXVECTOR3 pos);   // ��������
private:

};

#endif // !STONE_H_