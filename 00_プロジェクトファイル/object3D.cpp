#include "manager.h"
#include "object3D.h"
#include "player3D.h"
#include "texture.h"

//===========================================================================================================
// �R���X�g���N�^
//===========================================================================================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
, m_pos{ 0.0f, 0.0f, 0.0f }
, m_rot{ 0.0f,0.0f,0.0f }
, m_size{ 0.0f,0.0f,0.0f }
, m_scale{ 1.0f,1.0f,1.0f }
, m_pVtxBuff(nullptr)
, m_pVtxTexture(nullptr)
, m_length(0.0f)
, m_Angle(0.0f)
, m_color{ 1.0f,1.0f,1.0f,1.0f }
{
}

//===========================================================================================================
// �f�X�g���N�^
//===========================================================================================================
CObject3D::~CObject3D()
{
}

//===========================================================================================================
// �����ݒ�
//===========================================================================================================
HRESULT CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	// ���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, 0.0f, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, 0.0f, -m_size.z);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �J���[�̐ݒ�
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{// ��
		pVtx[nCnt].col = m_color;
	}

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_rot.x = (D3DX_PI * -0.5f);

	m_pVtxBuff->Unlock();

	// ���������u�Ԃɕ`�悪���������Ȃ�Ȃ��悤�� Matrix �̌v�Z���s��
	UpdateMatrix();

	return S_OK;
}

//===========================================================================================================
// �I������
//===========================================================================================================
void CObject3D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//// �e�N�X�`���̔j��
	//if (m_pVtxTexture != nullptr)
	//{
	//	m_pVtxTexture->Release();
	//	m_pVtxTexture = nullptr;
	//}

	Release();
}

//===========================================================================================================
// �X�V����
//===========================================================================================================
void CObject3D::Update()
{
	UpdateMatrix();
}

//===========================================================================================================
// �`�揈��
//===========================================================================================================
void CObject3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//if (pPlayer3D == nullptr)
	{
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�`��
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pVtxTexture);

		// �|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//===========================================================================================================
// ��������
//===========================================================================================================
CObject3D* CObject3D::Create(D3DXVECTOR3 pos)
{
	CObject3D* pObject3D = nullptr;
	pObject3D = new CObject3D;

	pObject3D->m_pos = pos;

	pObject3D->Init();

	return pObject3D;
}

//===========================================================================================================
// �}�g���b�N�X�̍X�V
//===========================================================================================================
void CObject3D::UpdateMatrix()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//===========================================================================================================
// �ʒu��ݒ�
//===========================================================================================================
void CObject3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================================================================================================
// �ʒu���擾
//===========================================================================================================
D3DXVECTOR3& CObject3D::GetPos()
{
	return m_pos;
}

//===========================================================================================================
// �T�C�Y��ݒ�
//===========================================================================================================
void CObject3D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//===========================================================================================================
// �T�C�Y���擾
//===========================================================================================================
D3DXVECTOR3& CObject3D::GetSize()
{
	return m_size;
}

//========================================================================================================================
// �C�ӂ̃e�N�X�`����ݒ肷��
//========================================================================================================================
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pVtxTexture = pTex;
}

void CObject3D::SetLength(D3DXVECTOR3 sizeA, D3DXVECTOR3 sizeB)
{
}
