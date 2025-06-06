//===========================================================================================================================================================
// 
// slime_enemy.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SLIME_ENEMY_H_
#define _SLIME_ENEMY_H_
#include "main.h"
#include "slime.h"

class CSlimeEnemy :public CSlime
{
public:
	CSlimeEnemy();
	~CSlimeEnemy()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CSlimeEnemy* Create(D3DXVECTOR3 pos);   // ��������
private:

};

#endif // !SLIME_ENEMY_H_