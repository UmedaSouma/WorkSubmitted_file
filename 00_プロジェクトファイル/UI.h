#pragma once
//===========================================================================================================================================================
// 
// UI.cpp�̃w�b�_�[ [UI.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _UI_H_
#define _UI_H_
#include "main.h"
#include "object2D.h"

class CUI :public CObject2D
{
public:
	CUI(int nPriority = 4);
	~CUI()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��
	void DeleteUI() { m_nLife = 0; }

	static CUI* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int life, const char* filepass);   // ��������

	// �����̐ݒ�
	void SetLife(int life) { m_nLife = life; }
	int GetLife() { return m_nLife; }

	// �\���̐ݒ�
	void SetUse(bool use) { m_bUse = use; }
	bool GetUse() { return m_bUse; }
private:
	int m_nLife;	// ����
	bool m_Isblink;	// �_�ł��邩�ۂ�
	bool m_bUse;	// �\�����Ă��邩�ǂ��� 
};

#endif // !UI_H_