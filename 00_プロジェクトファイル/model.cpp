//===========================================================================================================================================================
// 
// ���f���̊Ǘ�
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "model.h"
#include "player3D.h"
#include "manager.h"
#include "editor.h"

//===================================================================================
// �R���X�g���N�^
//===================================================================================
CModel::CModel(int nPriority) :CObject3D(nPriority)
, m_Direction(0)
, m_ModelType(MODEL_TYPE::NORMAL)
, m_nIdx(-1)
, m_bMat(false)
, m_Diffuse{}
, m_pModelAddress{}
, m_apTexture{}
{
	m_ModelInfo = {};
}

//===================================================================================
// �f�X�g���N�^
//===================================================================================
CModel::~CModel()
{
}

//===================================================================================
// �����ݒ�
//===================================================================================
HRESULT CModel::Init()
{
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_pEffect = nullptr;	// ������
	CRenderer* pRender = CManager::GetRenderer();	// �����_���[���擾
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();	// �f�o�C�X���擾

#if 1

	// �G�t�F�N�g����
	D3DXCreateEffectFromFile(
		pDevice,	// �`��f�o�C�X
		"test.fx",	// �t�@�C���p�X
		nullptr,	// �v���v���Z�b�T��`
		nullptr,	// ID3DXInclude�C���^�[�t�F�C�X�̒�`
		0,			// �ǂݍ��ݎ��̃I�v�V�����t���O
		nullptr,	// ID3DXEffectPool�C���^�[�t�F�C�X
		&m_pEffect,	// ID3DXEffect�̃C���^�[�t�F�C�X�̃|�C���^
		nullptr		// �R���p�C���G���[���̏��
	);

#endif // 0

	m_ModelType = (MODEL_TYPE)CEditor::GetModelType();

	UpdateMatrix();	// �}�g���b�N�X�̍X�V

	// �f�o�b�O�p�̕ϐ��̏�����
	m_Debug_Radian = 50.0f;
	m_Debug_f = 0.0f;

	return S_OK;
}

//===================================================================================
// �I������
//===================================================================================
void CModel::Uninit()
{
	//CPlayer3D* pPlayer3D = dynamic_cast<CPlayer3D*>(this);
	//if (pPlayer3D == nullptr)
	//{
		//if (m_ModelInfo.Mesh != nullptr)
		//{
		//	m_ModelInfo.Mesh->Release();
		//	m_ModelInfo.Mesh = nullptr;
		//}
	//}
	
	//if (m_ModelInfo.BuffMat != nullptr)
	//{
	//	m_ModelInfo.BuffMat->Release();
	//	m_ModelInfo.BuffMat = nullptr;
	//}

	//if (m_pEffect != nullptr)
	//{
	//	m_pEffect = nullptr;
	//}

	Release();
}

//===================================================================================
// �X�V����
//===================================================================================
void CModel::Update()
{
	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();	// �����Ń}�g���b�N�X�̐ݒ�����Ă���

	FixRot();	// �p�x�̏C��
}

//===================================================================================
// �`�揈��
//===================================================================================
void CModel::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;				// �}�e���A���f�[�^�̃|�C���^
	CInputKeyBoard*keyboad = CManager::GetKeyboard();	// �L�[�{�[�h�̎擾

	// �f�o�b�O�p�̑���
	if (keyboad->GetPress(DIK_UPARROW))
	{
		m_Debug_Radian++;
	}
	else if (keyboad->GetPress(DIK_DOWNARROW))
	{
		m_Debug_Radian--;
	}

#if 1

	//// �r���[�ϊ��E�ˉe�ϊ�
	//D3DXMATRIX View, Proj;
	//D3DXMatrixPerspectiveFovLH(&Proj, D3DXToRadian(m_Debug_Radian), SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 10000.0f);

#endif // m_Debug_IsShader


	

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &GetMatrix());

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (m_ModelInfo.BuffMat != nullptr
		&& m_ModelInfo.Mesh != nullptr
		&& m_ModelInfo.NumMat != 0)
	{
		m_Debug_f += 0.025f;

		// �}�e���A���f�[�^�̃|�C���^�̎擾
		pMat = (D3DXMATERIAL*)m_ModelInfo.BuffMat->GetBufferPointer();
		CModeldata* pModeldata = CManager::GetModeldata();	// Modeldata �̃|�C���^�������Ă���

		// �G�t�F�N�g���̃��[���h�r���[�ˉe�ϊ��s���ݒ�
		//D3DXMATRIX mat = GetMatrix();
		////D3DXMatrixLookAtLH(&View, &D3DXVECTOR3(30 * sin(f), 20, -30 * cos(f)), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));

		//D3DXVECTOR3 v1(CManager::GetCamera()->GetPos());
		//D3DXVECTOR3 v2(GetPos());
		//D3DXVECTOR3 v3(0, 1, 0);
		//D3DXMatrixLookAtLH(&View, &v1, &v2, &v3);

		//D3DXMatrixIdentity(&mat);
		//mat = mat * View * Proj;

		//m_pEffect->SetMatrix("matWorldViewProj", &mat);

		DemoUpdateSheder();

		if (pModeldata)
		{
			for (int nCntMat = 0; nCntMat < (int)m_ModelInfo.NumMat; nCntMat++)
			{
				

				if (m_bMat)
				{// �_���[�W���󂯂��Ƃ�
					pMat[nCntMat].MatD3D.Diffuse = { 1.0f,0.0f,0.0f,0.5f };	// �Ԃ�����
				}
				else
				{// ���̂ق��̏�Ԃ̎�

					//if (CScene::GetMode() != CScene::MODE_EDITOR)
					{
						pMat[nCntMat].MatD3D.Diffuse = *pModeldata->GetMatData(m_pModelAddress, nCntMat);	// �ʏ�F�ɂ���
					}
				}


				if (m_pEffect != nullptr)
				{
					m_pEffect->SetTechnique("BasicTec");
					UINT U_numpass;
					m_pEffect->Begin(&U_numpass, 0);

					// �F�����̂܂܉f���o��(�e�Ȃ�)
					D3DXCOLOR _LocalColor = (D3DXCOLOR)pMat[nCntMat].MatD3D.Diffuse;
					D3DXVECTOR4 LocalColor = (D3DXVECTOR4)_LocalColor;
					m_pEffect->SetVector("ScreenColor", &LocalColor);

					for (unsigned int i = 0; i < U_numpass; i++)
					{
						m_pEffect->BeginPass(i);

						// ���f���p�[�c�̕`��
						m_ModelInfo.Mesh->DrawSubset(nCntMat);

						m_pEffect->EndPass();
					}

					if (GetType() == CObject::BASE)
					{// �G�f�B�^�[�̃x�[�X�I�u�W�F�N�g�̎���������������
						pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;
					}

					

					
				}
				else
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, nullptr);

					// ���f���p�[�c�̕`��
					m_ModelInfo.Mesh->DrawSubset(nCntMat);
				}

				

				//D3DXVECTOR4 color = { pMat[nCntMat].MatD3D.Diffuse.r,pMat[nCntMat].MatD3D.Diffuse.g,pMat[nCntMat].MatD3D.Diffuse.b,pMat[nCntMat].MatD3D.Diffuse.a };

				//m_pEffect->SetVector("OutLineColor", &color);

				

				if (m_pEffect != nullptr)
				{
					m_pEffect->End();
				}
			}
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===========================================================================================================
// �p�x���C������
//===========================================================================================================
void CModel::FixRot()
{
	if (GetRot().x >= D3DX_PI*2 ||  GetRot().x <= -D3DX_PI*2)
	{
		SetRot({ 0.0f,GetRot().y,GetRot().z });
	}
	if (GetRot().y >= D3DX_PI * 2 || GetRot().y <= -D3DX_PI * 2)
	{
		SetRot({ GetRot().x,0.0f,GetRot().z });
	}
	if (GetRot().z >= D3DX_PI * 2 ||  GetRot().z <= -D3DX_PI * 2)
	{
		SetRot({ GetRot().x,GetRot().y,0.0f });
	}
}

//===================================================================================
// ��������
//===================================================================================
CModel* CModel::Create(D3DXVECTOR3 pos,TYPE type, D3DXVECTOR3 scale, D3DXVECTOR3 rot)
{
	CModel* pModel = new CModel;

	pModel->SetPos(pos);
	pModel->SetType(TYPE::MODEL);

	pModel->Init();

	return pModel;
}

//===================================================================================
// ���f���̏����Z�b�g����
//===================================================================================
void CModel::BindModel(CModeldata::ModelData *modelinfo)
{
	m_ModelInfo.Mesh = modelinfo->Mesh;
	m_ModelInfo.BuffMat = modelinfo->BuffMat;
	m_ModelInfo.NumMat = modelinfo->NumMat;
}

//===========================================================================================================
// ������ݒ�
//===========================================================================================================
void CModel::SetDirection(DIRECTION dire)
{
	switch (dire)
	{
	case DIRECTION_LEFT:
		m_Direction = -1;
		SetRot({
			0.0f
			,D3DX_PI * 0.5f
			,0.0f
			});
		break;

	case DIRECTION_RIGHT:
		m_Direction = 1;
		SetRot({
			0.0f
			,D3DX_PI * -0.5f
			,0.0f
			});
		break;

	default:
		break;
	}
}

//===========================================================================================================
// �������擾
//===========================================================================================================
int CModel::GetDirection()
{
	return m_Direction;
}

//===========================================================================================================
// ModelFile�̐ݒ�
//===========================================================================================================
void CModel::SetModelFile()
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		m_ModelFile[i].name = "a";
	}
}

//===========================================================================================================
// ���f���̌����ڐݒ�
//===========================================================================================================
void CModel::SetModel(const char* address)
{
	CModeldata* pModeldata = nullptr;
	int nIdx = 0;

	SetModelAddress(address);		// �A�h���X��ۑ����Ă���
	pModeldata = CManager::GetModeldata();
	nIdx = pModeldata->Regist(address);
	BindModel(CManager::GetModeldata()->GetAddress(nIdx));
}

//===========================================================================================================
// �V�F�[�_�[�̃}�g���b�N�X�v�Z
//===========================================================================================================
void CModel::DemoUpdateSheder()
{
	if (m_pEffect == nullptr)
		return;

	D3DXMATRIX World, View, w_Proj, Proj;
	World = GetMatrix();
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &Proj);

	D3DXMatrixMultiply(&w_Proj, &World, &View);
	D3DXMatrixMultiply(&w_Proj, &w_Proj, &Proj);

	//�V�F�[�_�[�ɍs���ݒ�
	m_pEffect->SetMatrix("matWVP", &w_Proj);
	
	//m_pEffect->SetMatrix("World", &World);

}

//===========================================================================================================
// ���f���̐ݒ茩����
//===========================================================================================================
void CModel::SetModel(int type)
{
	char* address = nullptr;

	switch (type)
	{
	case GRASS:
		// ���f���̐ݒ�
		SetModel("data\\model\\block_grass_000.x");
		break;

	case STONE:
		// ���f���̐ݒ�
		SetModel("data\\model\\block_stone_000.x");
		break;

	case START:
		// ���f���̐ݒ�
		SetModel("data\\model\\gimmick_start.x");
		break;

	case GOAL:
		// ���f���̐ݒ�
		SetModel("data\\model\\gimmick_goal.x");
		break;

	case SLIME:
		SetModel("data\\model\\enemy_slime_000.x");
		break;

	case GOLEM:
		SetModel("data\\model\\enemy_golem000.x");
		break;

	default:
		assert(0);
		break;
	}

	if (address != nullptr)
	{
		SetModel(address);
	}
}
