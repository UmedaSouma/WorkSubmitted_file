//===========================================================================================================================================================
// 
// �{�X�r�[���̏��� [attack_beam.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "attack_beam.h"
#include "manager.h"
#include "player3D.h"
#include "search.h"

//========================================================================================================================
// �R���X�g���N�^
//========================================================================================================================
CAttackBeam::CAttackBeam() :
	m_Move({ 0.0f,0.0f,0.0f })
	, m_nLife(0)
	, m_bReturn(false)

{
}

//========================================================================================================================
// �f�X�g���N�^
//========================================================================================================================
CAttackBeam::~CAttackBeam()
{
}

//========================================================================================================================
// �����ݒ�
//========================================================================================================================
HRESULT CAttackBeam::Init()
{
	m_Move = { -3.5f,0.0f,0.0f };
	SetSize({ 1.0f,10.0f,0.0f });

	CModel::Init();

	// ���f���f�[�^���Z�b�g
	SetModelAddress("data\\model\\attack_beam_000.x");	// �A�h���X��ۑ����Ă���
	CModeldata* pModeldata = CManager::GetModeldata();	// modeldata �̃|�C���^�������Ă���
	int nIdx = pModeldata->Regist(GetModelAddress());	// ���f���f�[�^�̓o�^
	BindModel(pModeldata->GetAddress(nIdx));		 	// ���f�������Z�b�g����

	return S_OK;
}

//========================================================================================================================
// �I������
//========================================================================================================================
void CAttackBeam::Uninit()
{
	CModel::Uninit();
}

//========================================================================================================================
// �X�V����
//========================================================================================================================
void CAttackBeam::Update()
{
	m_nLife--;

	D3DXVECTOR3 pos_Beam = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//if (m_bReturn)
	//{
	//	rot.x -= 0.1f;
	//}
	//else
	//{
	//	rot.x += 0.1f;
	//}

	SetRot(rot);

	pos_Beam += m_Move;
	SetPos(pos_Beam);

	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}

	Collision();

	CModel::Update();
}

//========================================================================================================================
// �`�揈��
//========================================================================================================================
void CAttackBeam::Draw()
{
	CModel::Draw();
}

//===========================================================================================================
// �����蔻��
//===========================================================================================================
void CAttackBeam::Collision()
{
	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);
	
	D3DXVECTOR3 pos_Player = pPlayer->GetPos();
	D3DXVECTOR3 size_Player = pPlayer->GetSize() * 0.5f;
	
	D3DXVECTOR3 pos_Beam = GetPos();
	D3DXVECTOR3 size_Beam = GetSize();

	if (pos_Player.x - size_Player.x <= pos_Beam.x + size_Beam.x
		&& pos_Player.x + size_Player.x >= pos_Beam.x - size_Beam.x
		&& pos_Player.y - size_Player.y <= pos_Beam.y + size_Beam.y
		&& pos_Player.y + size_Player.y >= pos_Beam.y - size_Beam.y)
	{
		pPlayer->AddDamage(m_BeamDamage);
	}

}

//========================================================================================================================
// ��������
//========================================================================================================================
CAttackBeam* CAttackBeam::Create(D3DXVECTOR3 pos_Beam, int life, bool breturn)
{
	CAttackBeam* pAttackBeam = new CAttackBeam;

	pAttackBeam->SetPos({ pos_Beam.x,pos_Beam.y + 15.0f,pos_Beam.z });
	pAttackBeam->m_nLife = life;
	pAttackBeam->m_bReturn = breturn;

	pAttackBeam->Init();

	return pAttackBeam;
}
