#pragma once
//===========================================================================================================================================================
// 
// sword.cpp�̃w�b�_�[ [sword.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SWORD_H_
#define _SWORD_H_
#include "main.h"
#include "model.h"

//===========================================================================================================
// < debugmemo >
// weapon �N���X�Ƃ����h������h�����Ă����Ƃ����ȕ���N���X�����邩���H
// ����͗v��Ȃ����H���X�L���Ƃ��� weapon ����h���������������̂��H
//===========================================================================================================

class CSword:public CModel
{
public:
	CSword();
	~CSword()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CSword* Create(D3DXVECTOR3 pos);   // ��������

	void UpdateRot();
	void TargetPos(D3DXVECTOR3 pos);	// ���̃I�u�W�F�N�g�̐e�̈ʒu
	void TargetRot(D3DXVECTOR3 rot, int frame);	// ���̃I�u�W�F�N�g�̖ڕW�̊p�x + ���t���[���ŉ�]�����邩
	D3DXVECTOR3& GetTargetrot() { return m_Targetrot; }
private:
	D3DXVECTOR3 m_Targetrot;	// �ڕW�̊p�x
	D3DXVECTOR3 m_Moverot;		// �ڕW�̊p�x�܂ł̈ړ���
	int m_TargetFrame;			// �ڕW�̃t���[����
	int m_FrameCnt;			// �t���[����ێ�����ϐ�

};

#endif // !SWORD_H_
