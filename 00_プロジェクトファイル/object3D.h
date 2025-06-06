//===========================================================================================================================================================
// 
// object3D.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

class CObject3D:public CObject 
{
public:
	CObject3D(int nPriority = PRIORITY_DEFAULT);
	~CObject3D()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void UpdateMatrix();
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	// �e�N�X�`���̐ݒ�

	//----------------------------------------------------
	// Set / Get
	//----------------------------------------------------

	// �ʒu -------------------------------
	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3& GetPos();

	// �T�C�Y -----------------------------
	void SetSize(D3DXVECTOR3 size);
	D3DXVECTOR3& GetSize();

	// �p�x -------------------------------
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 &GetRot() { return m_rot; }

	// �g�k�� -----------------------------
	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
	D3DXVECTOR3& GetScale() { return m_scale; }

	// �Ίp���̒��� ------------------------
	void SetLength(D3DXVECTOR3 sizeA, D3DXVECTOR3 sizeB);
	float GetLength() { return m_length; }

	// �Ίp���̊p�x ------------------------
	void SetAngle(float sizeA, float sizeB);
	float GetAngle() { return m_Angle; }

	// �F ---------------------------------
	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor() { return m_color; }

	const D3DXMATRIX& GetMatrix() const { return m_mtxWorld; }	// �}�g���b�N�X�̎擾
	static CObject3D* Create(D3DXVECTOR3 pos);	// ��������
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pVtxTexture;	// ���_�e�N�X�`��
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// �p�x
	D3DXVECTOR3 m_size;	// �|���S���T�C�Y
	D3DXVECTOR3 m_scale;	// �g�k��
	D3DXCOLOR m_color;	// �F

	float m_length;	// �͈�	
					//	|�[>�~�̔���Ɏg����
	float m_Angle;	// �p�x
	D3DXMATRIX m_mtxWorld;
};

#endif // !_OBJECT3D_H_
