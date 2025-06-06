//===========================================================================================================================================================
// 
// slime_player.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SLIME_PLAYER_H_
#define _SLIME_PLAYER_H_
#include "main.h"
#include "slime.h"

class CSlimePlayer :public CSlime
{
public:
	CSlimePlayer();
	~CSlimePlayer()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	void UseSkillUpdate();
	static CSlimePlayer* Create(D3DXVECTOR3 pos);   // ��������
private:
	int m_nSoundCnt;
};

#endif // !SLIME_PLAYER_H_