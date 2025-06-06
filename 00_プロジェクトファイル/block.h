//===========================================================================================================================================================
// 
// block.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "object2D.h"

//========================================================================
// �N���X
//========================================================================

// �u���b�N�N���X
class CBlock:public CObject2D
{
public:
	CBlock();					// �R���X�g���N�^
	~CBlock();					// �f�X�g���N�^
	HRESULT Init()override;		// �����ݒ菈��
	void Uninit()override;		// �I������
	void Update()override;		// �X�V����
	void Draw()override;		// �`�揈��

	static CBlock* Create(D3DXVECTOR3 pos);	// ��������
	static void TextureLoad();				// �e�N�X�`���ǂݍ��ݏ���
	static void TextureUnload();			// �e�N�X�`���j������
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���̃|�C���^�m�ەϐ�
};

//class CObstacle :public CBlock
//{
//public:
//	CObstacle();
//	~CObstacle();
//	HRESULT Init()override;
//	void Uninit()override;
//	void Update()override;
//	void Draw()override;
//	void UpDownCollision();
//
//private:
//
//};



#endif // !_BLOCK_H_
