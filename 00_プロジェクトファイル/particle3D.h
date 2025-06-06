//===========================================================================================================================================================
// 
// particle.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"
#include "model.h"

//===========================================================================================================
// �N���X
//===========================================================================================================

// �p�[�e�B�N��(���f���p��)
class CParticle3D:public CModel
{
public:
	CParticle3D();
	~CParticle3D()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CParticle3D* Create(D3DXVECTOR3 pos,int life);
private:
	int m_nLife;				// ����(�t���[���P��)
	int m_nNum;					// �g�U��
	D3DXVECTOR3 m_move;

public:
	void SetLife(int life) { m_nLife = life; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove() { return m_move; }
};

#endif // !PARTICLE_H_