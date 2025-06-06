#pragma once
//===========================================================================================================================================================
// 
// golem_player.cpp�̃w�b�_�[ [golem_player.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _GOLEM_PLAYER_H_
#define _GOLEM_PLAYER_H_
#include "main.h"
#include "golem.h"

class CGolemPlayer:public CGolem
{
public:
	CGolemPlayer();
	~CGolemPlayer()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CGolemPlayer* Create(D3DXVECTOR3 pos);   // ��������
private:

};



#endif // !GOLEM_PLAYER_H_
