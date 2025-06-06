//===========================================================================================================================================================
// 
// goal.cpp�̃w�b�_�[ [goal.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _GOAL_H_
#define _GOAL_H_
#include "main.h"
#include "model.h"

class CGoal :CModel
{
public:
	CGoal();
	~CGoal()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CGoal* Create(D3DXVECTOR3 pos);   // ��������
private:

};
#endif // !GOAL_H_