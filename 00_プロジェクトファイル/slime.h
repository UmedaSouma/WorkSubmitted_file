//===========================================================================================================================================================
// 
// slime.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SLAIME_H_
#define _SLAIME_H_
#include "main.h"
#include "enemyland3D.h"

class CSlime :public CEnemyland3D
{
public:
	CSlime();
	~CSlime()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CSlime* Create(D3DXVECTOR3 pos);   // ��������

	void Attack();	// �U���̏���
private:
	int m_nCntJumptime;	// �W�����v�̃N�[���^�C��

};



#endif // !SLAIME_H_