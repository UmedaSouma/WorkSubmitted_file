//===========================================================================================================================================================
// 
// start.cpp�̃w�b�_�[ [start.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _START_H_
#define _START_H_
#include "main.h"
#include "model.h"


class CStart :CModel
{
public:
	CStart();
	~CStart()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CStart* Create(D3DXVECTOR3 pos);   // ��������
private:

};


#endif // !START_H_