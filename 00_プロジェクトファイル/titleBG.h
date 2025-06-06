#pragma once
//===========================================================================================================================================================
// 
// titleBG.cpp�̃w�b�_�[ [titleBG.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _TITLE_BG_H_
#define _TITLE_BG_H_
#include "main.h"
#include "object2D.h"

class CTitleBG :public CObject2D
{
public:
	CTitleBG(int nPriority=4);
	~CTitleBG()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CTitleBG* Create();   // ��������
private:

};


#endif // !TITLE_BG_H_