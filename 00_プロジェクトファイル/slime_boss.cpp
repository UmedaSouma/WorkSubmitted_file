//===========================================================================================================================================================
// 
// �{�X�X���C���̏��� [slime_boss.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "slime_boss.h"
#include "manager.h"
#include "search.h"
#include "player3D.h"
#include "fade.h"
#include "hp_meter.h"
#include "game.h"

#include "attack_beam.h"
#include "attack_slash.h"

//========================================================================================================================
// �R���X�g���N�^
//========================================================================================================================
CSlimeBoss::CSlimeBoss() :
	m_StateAttack(ATTACK_NONE)
	, m_nStateNoneCnt(0)
	, m_nStateAttackCnt(0)
	, m_bIsAppear(false)
	, m_pDangerZone(nullptr)
	, m_nTurn(0)
	, m_FirstApper(false)
{	
}

//========================================================================================================================
// �f�X�g���N�^
//========================================================================================================================
CSlimeBoss::~CSlimeBoss()
{
}

//========================================================================================================================
// �����ݒ�
//========================================================================================================================
HRESULT CSlimeBoss::Init()
{
	m_bIsAppear = false;
	SetTag(CEnemy3D::TAG_SLIME);

	SetSize({ 20.0f,30.0f,20.0f });

	SetMaxLife(350);

	CEnemyland3D::Init();

	// ���f���f�[�^���Z�b�g
	SetModelAddress("data\\model\\enemy_slime_boss.x");		// �A�h���X��ۑ����Ă���
	CModeldata* pModeldata = CManager::GetModeldata();		// modeldata �̃|�C���^�������Ă���
	int nIdx = pModeldata->Regist(GetModelAddress());		// ���f���f�[�^�̓o�^
	BindModel(pModeldata->GetAddress(nIdx));				// ���f�������Z�b�g����


	return S_OK;
}

//========================================================================================================================
// �I������
//========================================================================================================================
void CSlimeBoss::Uninit()
{
	if (m_pDangerZone != nullptr)
	{
		m_pDangerZone->Uninit();
	}

	CEnemyland3D::Uninit();
}

//========================================================================================================================
// �X�V����
//========================================================================================================================
void CSlimeBoss::Update()
{
	CFade*pFade= CManager::GetFade();

	if (m_bIsAppear)
	{
		if (!m_FirstApper)
		{
			// HP �o�[�̐���
			CHpMeter::Create(
				{ 700.0f,60.0f,0.0f },
				{ 510.0f,30.0f,0.0f },
				CHpMeter::HP_TYPE::HP_LITERAL,
				GetType()
			);

			CHpMeter::Create(
				{ 700.0f,60.0f,0.0f },
				{ 510.0f,30.0f,0.0f },
				CHpMeter::HP_TYPE::HP_VARIABLE,
				GetType()
			);

			m_FirstApper = true;
		}

		if (m_nStateNoneCnt >= 60)
		{
			SelectAttackState();
		}

		if (m_StateAttack == ATTACK_NONE)
		{
			m_nStateNoneCnt++;
		}

		UpdateAttack();
	}
	else
	{
		SetPos(m_OriginPos);
		m_nStateAttackCnt = 0;
		m_nStateNoneCnt = 0;
		m_nTurn = 0;

		if (m_pDangerZone != nullptr)
		{
			m_pDangerZone->Uninit();
			m_pDangerZone = nullptr;
		}
	}

	if (GetLife() <= 0)
	{
		Uninit();
		//pFade->SetFade(CScene::MODE_RESULT);
		CGame::SetDelay();
		return;
	}

	CEnemyland3D::Update();
	
}

//========================================================================================================================
// �`�揈��
//========================================================================================================================
void CSlimeBoss::Draw()
{
	CEnemyland3D::Draw();
}

//===========================================================================================================
// �U����Ԃ̑I��
//===========================================================================================================
void CSlimeBoss::SelectAttackState()
{
	m_nStateNoneCnt = 0;

	int min = ATTACK_NONE + 1;	// �����̍ŏ��l
	int max = ATTACK_MAX - 1;	// �����̍ő�l

	int random = min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));	// �ŏ��l + (rand() * �ő�l - �ŏ��l + 1.0) / (1.0 + RAND_MAX)

	m_StateAttack = (STATE_ATTACK_BOSS_SLIME)random;
}

//===========================================================================================================
// �U���X�V(�U�����̓���)
//===========================================================================================================
void CSlimeBoss::UpdateAttack()
{
	switch (m_StateAttack)
	{
	case ATTACK_RUSH:
		////SetMove({ 5.0f,0.3f,0.0f });
		//m_nStateAttackCnt++;

		m_StateAttack = ATTACK_NONE;
		break;

	case ATTACK_JUMP:

		Attack_Jump();

		break;

	case ATTACK_BEAM:
		Attack_Beam();

		break;

	case ATTACK_NONE:
		SetSwitchGravity(false);

		break;
	}

	//if (m_nStateAttackCnt >= 180)
	//{
	//	m_StateAttack = ATTACK_NONE;
	//	m_nStateAttackCnt = 0;
	//}


}

//===========================================================================================================
// �̓�����U��
//===========================================================================================================
void CSlimeBoss::Attack_Rush()
{
}

//===========================================================================================================
// �W�����v�U��
//===========================================================================================================
void CSlimeBoss::Attack_Jump()
{
	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);
	D3DXVECTOR3 pos;
	float posy = 0.0f;


	switch (m_nTurn)
	{
	case 0:
		SetState(STATE_CREATE);
		m_nStateAttackCnt++;
		SetSwitchGravity(true);

		SetMove({ 0.0f,15.0f,0.0f });

		if (m_nStateAttackCnt >= 45)
		{
			SetMove({ 0.0f,0.0f,0.0f });
			m_nStateAttackCnt = 0;
			m_nTurn = 1;
		}

		break;

	case 1:
		m_nStateAttackCnt++;

		pos = GetPos();

		SetSwitchGravity(true);
		SetPos({ pPlayer->GetPos().x,pos.y+50.0f,pos.z });

		if (m_nStateAttackCnt >= 90)
		{
			m_nStateAttackCnt = 0;
			m_nTurn = 2;
		}

		break;

	case 2:
		SetState(STATE_CREATE);
		SetSwitchGravity(false);

		posy = GetPos().y;
		SetPos({ GetPos().x ,GetPos().y - 50.0f ,GetPos().z });

		if (GetPos().y <= 5.0f)
		{
			m_nTurn = 3;
		}


		break;

	case 3:

		SetState(STATE_NORMAL);

		CAttackSlash::Create(GetPos(), 180, false);
		CAttackSlash::Create(GetPos(), 180, true);
		m_nStateAttackCnt = 0;
		m_nTurn = 0;
		m_StateAttack = ATTACK_NONE;

		break;
	}
}

//===========================================================================================================
// �r�[���U��
//===========================================================================================================
void CSlimeBoss::Attack_Beam()
{

	switch (m_nTurn)
	{
	case 0:
		SetPos({ 1000.0f,5.0f,0.0f });
		SetSwitchGravity(true);

		m_nTurn = 1;
		break;

	case 1:
		SetSwitchGravity(true);

		if (m_nStateAttackCnt == 0)
		{
			m_pDangerZone = CDangerZone::Create({ GetPos().x,GetPos().y + 15.0f,GetPos().z },
				{ GetSize().x * 10.0f,GetSize().y * 5.0f,GetSize().z });
		}

		m_nStateAttackCnt++;

		if (m_nStateAttackCnt >= 120)
		{
			if (m_pDangerZone != nullptr)
			{
				m_pDangerZone->Uninit();
				m_pDangerZone = nullptr;
			}
			m_nStateAttackCnt = 0;
			m_nTurn = 2;
		}

		SetSufferKnockback(true);

		break;

	case 2:

		//CAttackBeam::Create(GetPos(), 60, true);
		CAttackBeam::Create(GetPos(), 60, false);

		m_nStateAttackCnt++;

		if (m_nStateAttackCnt >= 60)
		{
			m_nStateAttackCnt = 0;
			m_nTurn = 0;
			m_StateAttack = ATTACK_NONE;
			SetSufferKnockback(false);
		}
		break;
	}
}

//========================================================================================================================
// ��������
//========================================================================================================================
CSlimeBoss* CSlimeBoss::Create(D3DXVECTOR3 pos)
{
	CSlimeBoss* pSlimeBoss = new CSlimeBoss;

	pSlimeBoss->m_OriginPos = pos;

	pSlimeBoss->SetPos(pos);
	pSlimeBoss->SetType(TYPE::ENEMY_BOSS);

	pSlimeBoss->Init();

	return pSlimeBoss;
}