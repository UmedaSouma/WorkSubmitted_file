//===========================================================================================================================================================
// 
// bg.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BG_H_
#define _BG_H_

#include "object.h"

//========================================================================
// �N���X
//========================================================================

// �w�i�̃N���X
class CBg :public CObject
{
public:
	CBg(int nPriorty=0);	// �R���X�g���N�^(�`�����ԉ���
	~CBg();					// �f�X�g���N�^
	HRESULT Init()override;	// ����������
	void Uninit()override;	// �I������
	void Update()override;	// �X�V����
	void Draw()override;	// �`�揈��
	static CBg* Create();	// �쐬����
private:
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_size;	// �傫��
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pVtxTexture;	// �e�N�X�`���̃|�C���^
	D3DXVECTOR2 m_nTexSplit;	// �e�N�X�`���̕�����
};

#endif // !_BG_H_
