//===========================================================================================================================================================
// 
// enemyland3D.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ENEMYLAND3D_H_
#define _ENEMYLAND3D_H_
#include "main.h"
#include "enemy3D.h"

//===========================================================================================================
// ���G�l�~�[�N���X
//===========================================================================================================
class CEnemyland3D :public CEnemy3D
{
public:
	CEnemyland3D();
	~CEnemyland3D()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	void Gravity();		// �d�͂̏���

	static CEnemyland3D* Create(D3DXVECTOR3 pos);	// ��������

	void SetSwitchGravity(bool Zero) { m_bZeroGravity = Zero; }
private:
	static inline const float m_fGravity = 0.3f;	// �d��
	float m_fMove;	// �d�͂����Z���邽�߂����� move �ϐ�
	bool m_bZeroGravity;	// true �̂Ƃ����d��
};

#endif // !ENEMYLAND3D_H_