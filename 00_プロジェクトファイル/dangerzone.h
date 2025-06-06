#pragma once
//===========================================================================================================================================================
// 
// dangerzone.cpp�̃w�b�_�[ [dangerzone.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _DANGER_ZONE_H_
#define _DANGER_ZONE_H_
#include "main.h"
#include "object3D.h"

class CDangerZone :public CObject3D
{
public:
	CDangerZone();
	~CDangerZone()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CDangerZone* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);   // ��������
private:

};

#endif // !DANGER_ZONE_H_