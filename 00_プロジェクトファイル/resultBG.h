#pragma once
//===========================================================================================================================================================
// 
// resultBG.cpp�̃w�b�_�[ [resultBG.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _RESULT_BG_H_
#define _RESULT_BG_H_
#include "main.h"
#include "object2D.h"

class CResultBG :public CObject2D
{
public:
	CResultBG(int nPriority = 4);
	~CResultBG()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CResultBG* Create();   // ��������
private:

};


#endif // !TITLE_BG_H_