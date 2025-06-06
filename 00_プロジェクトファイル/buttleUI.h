#pragma once
//===========================================================================================================================================================
// 
// buttleUI.cpp�̃w�b�_�[ [buttleUI.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BUTTLE_UI_H_
#define _BUTTLE_UI_H_
#include "main.h"
#include "object2D.h"
#include "enemy3D.h"

class CButtleUI :public CObject2D
{
public:
	// �I������Ă��閡�������X�^�[�Ƃ��̑O��̔ԍ�
	typedef enum
	{
		BUTTLE_MESSE_FRONT=0,	// �I������Ă��郂���X�^�[
		BUTTLE_MESSE_NEXT,	// ��
		BUTTLE_MESSE_PREV,	// �O
		BUTTLE_MESSE_MAX
	}BUTTLE_MESSE;

	CButtleUI(int nPriority = 4);
	~CButtleUI()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CButtleUI* Create(D3DXVECTOR3 pos,D3DXVECTOR3 size);   // ��������

	void SetTex_SelectMessenger(CEnemy3D::TAG_ENEMY tag);	// 
	void SetBPush(bool bPush) { m_bIsPush = bPush; }	// 
private:
	bool m_bIsPush;		// �{�^�������͂��ꂽ���A�m�F����
	bool m_bIsCoolTime;	// �N�[���^�C�������A�m�F����
};



#endif // !BUTTLE_UI_H_