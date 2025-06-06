//===========================================================================================================================================================
// 
// 3D�v���C���[�̏���
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "player3D.h"
#include "block3D.h"
#include "manager.h"
#include "particle3D.h"
#include "fragmentlife.h"
#include "slime_player.h"
#include "slime_enemy.h"
#include "game.h"
#include "search.h"
#include "golem_player.h"
#include "buttleUI.h"
#include "slime_boss.h"
#include "hp_meter.h"
#include "UI_skill.h"
#include "sound.h"

// �ÓI�����o�ϐ��̏�����
int CPlayer3D::m_nFragmentNum = 0;
CSkill* CPlayer3D::m_pSkill = nullptr;
CSword* CPlayer3D::m_pSword = nullptr;
CButtleUI* CPlayer3D::m_pButtleUI[CButtleUI::BUTTLE_MESSE_MAX] = {};

//========================================================================================================================
// �R���X�g���N�^
//========================================================================================================================
CPlayer3D::CPlayer3D(int nPriority) :CModel(nPriority)
, m_Move{ 0.0f,0.0f,0.0f }
, m_fSpeed(0)
, m_oldPos{ 0.0f,0.0f,0.0f }
, m_skillstate(SKILL_NONE)
, m_bUseJump(false)
, m_FrontMessengerTag(CEnemy3D::TAG_ENEMY::TAG_NONE)
, m_NextMessengerTag(CEnemy3D::TAG_ENEMY::TAG_NONE)
, m_PrevMessengerTag(CEnemy3D::TAG_ENEMY::TAG_NONE)
, m_bSlip(false)
, m_nSlipCnt(0)
, m_bDammage(false)
, m_nLife(0)
, m_nCntDamage(0)
{
}

//========================================================================================================================
// �f�X�g���N�^
//========================================================================================================================
CPlayer3D::~CPlayer3D()
{
}

//========================================================================================================================
// �����ݒ�
//========================================================================================================================
HRESULT CPlayer3D::Init()
{

	CModel::Init();

	// ���f���̐ݒ�
	SetModelAddress("data\\model\\player_001.x");		// �A�h���X��ۑ����Ă���
	CModeldata* pModeldata = CManager::GetModeldata();	// modeldata �̃|�C���^�������Ă���
	int nIdx = pModeldata->Regist(GetModelAddress());	// ���f���f�[�^�̓o�^
	BindModel(pModeldata->GetAddress(nIdx));			// ���f�������Z�b�g����
	SetModelIdx(nIdx);

	for (int nCntTag = 0; nCntTag <= CEnemy3D::TAG_ENEMY::TAG_MAX; nCntTag++)
	{
		m_pMessenger[nCntTag] = nullptr;
	}

	m_nFragmentNum = 0;

	// ���̐���
	m_pSword = CSword::Create(GetPos());

	// �퓬UI�̐ݒ�
	SetButtleUI();

	SetSize({ 10.0f, 20.0f, 15.0f });
	m_fSpeed = 1.0f;

	m_nLife = m_MaxLife;

	return S_OK;
}

//========================================================================================================================
// �I������
//========================================================================================================================
void CPlayer3D::Uninit()
{
	if (m_pSkill != nullptr)
	{
		delete m_pSkill;
		m_pSkill = nullptr;
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pButtleUI[nCnt] != nullptr)
		{
			m_pButtleUI[nCnt]->Uninit();
		}
	}

	for (int nCntMessenger = 0; nCntMessenger < CEnemy3D::TAG_MAX; nCntMessenger++)
	{
		if (m_pMessenger[nCntMessenger] != nullptr)
		{
			m_pMessenger[nCntMessenger]->Uninit();
		}
	}

	CModel::Uninit();

	//if (m_pSword != nullptr)
	//{
	//	delete m_pSword;
	//	m_pSword = nullptr;
	//}
}

//========================================================================================================================
// �X�V����
//========================================================================================================================
void CPlayer3D::Update()
{
	D3DXVECTOR3 pos = GetPos();

	// �����蔻�����(��X�֐���
	{
		if (m_nSlipCnt == 0)
		{
			m_bSlip = false;
		}
		if (m_bSlip)
		{
			m_nSlipCnt--;
		}

		DeathPlayer();
	}

	// �v���C���[����
	InputPosPlayer();

	if (!m_bSlip)
	{
		m_bUseJump = true;

		// Y���̓����蔻��E�X�V
		pos.y += m_Move.y;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_HIGHLOW);

		// X���̓����蔻��E�X�V
		pos.x += m_Move.x;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_HORIZON);

		// Z���̓����蔻��E�X�V
		pos.z += m_Move.z;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_VERTICAL);
	}

	// �J�����ǔ�
	SetTargetCamera();

	// ���x���X�Ɍ���
	m_Move.x += (0.0f - m_Move.x) * 0.5f;
	m_Move.z += (0.0f - m_Move.z) * 0.5f;
	//m_Move.y += (0.0f - m_Move.y) * 0.2f;

	CModel::Update();

	m_pSword->TargetPos({ pos.x,pos.y + 10.0f,pos.z - 50.0f });

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CreateTarget();
	}

	if (m_pSkill != nullptr)
	{
		UpdateSkillState();	// �X�L����Ԃ̃A�b�v�f�[�g
	}

	if (m_pSkill != nullptr)
	{
		m_pSkill->Update();
	}

	// �e�N�X�`����ݒ肷��
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_FRONT]->SetTex_SelectMessenger(m_FrontMessengerTag);
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_NEXT]->SetTex_SelectMessenger(m_NextMessengerTag);
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_PREV]->SetTex_SelectMessenger(m_PrevMessengerTag);

	MessengerUpdate();

	if (m_bDammage)
	{
		UpdateDamage();
	}

	// �ߋ��̈ʒu��o�^
	m_oldPos = pos;
}

//===========================================================================================================
// �g��G���̍X�V
//===========================================================================================================
void CPlayer3D::MessengerUpdate()
{
	if (m_pMessenger[m_FrontMessengerTag] != nullptr)
	{
		m_pMessenger[m_FrontMessengerTag]->Update();
	}

	for (int nCnt = 0; nCnt < CEnemy3D::TAG_MAX; nCnt++)
	{
		if (m_pMessenger[nCnt] != nullptr)
		{
			m_pMessenger[nCnt]->UpdateState();
		}
	}
}

//===========================================================================================================
// �g��G�̑O���ݒ肷��
//===========================================================================================================
void CPlayer3D::UpddateMessengerList()
{
	int nPrev = m_FrontMessengerTag - 1;
	int nNext = m_FrontMessengerTag + 1;

	// �퓬�ɏo�Ă���O�̎g���^�O��ݒ�
	if (m_pMessenger[m_FrontMessengerTag] != nullptr)
	{
		if (m_pMessenger[nPrev] == nullptr)
		{
			if (m_pMessenger[m_FrontMessengerTag]->GetTag() - 1 == CEnemy3D::TAG_NONE)
			{
				nPrev = CEnemy3D::TAG_MAX;
			}
			
			do {
				nPrev--;

				if (nPrev == CEnemy3D::TAG_NONE)
				{
					nPrev = CEnemy3D::TAG_MAX;
				}
			} while (m_pMessenger[nPrev] == nullptr);
		}

		m_PrevMessengerTag = (CEnemy3D::TAG_ENEMY)nPrev;
	}

	// �퓬�ɏo�Ă����̎g���^�O��ݒ�
	if (m_pMessenger[m_FrontMessengerTag] != nullptr)
	{
		if (m_pMessenger[nNext] == nullptr)
		{// ���̎g��G�����݂��Ă��Ȃ��Ƃ�
			if (m_pMessenger[m_FrontMessengerTag]->GetTag() + 1 == CEnemy3D::TAG_MAX)
			{// ���ʂɏo�Ă���^�O�̎��̃^�O���ő�l�ƈ�v�����Ƃ�
				nNext = CEnemy3D::TAG_NONE;	// �ŏ��l�ɐݒ�
			}

			do {
				nNext++;	// �^�O�i���o�[��i�߂�

				if (nNext == CEnemy3D::TAG_MAX)
				{// �ő�l�ɂȂ����Ƃ�
					nNext = CEnemy3D::TAG_NONE;	// ���[�v�p�ɍŏ��l�ɖ߂�
				}

			} while (m_pMessenger[nNext] == nullptr);	// nullptr �Ȃ烋�[�v����
		}

		m_NextMessengerTag = (CEnemy3D::TAG_ENEMY)nNext;	// ���̃^�O�ɐݒ肷��
	}

}

//========================================================================================================================
// �`�揈��
//========================================================================================================================
void CPlayer3D::Draw()
{
	MessengerDraw();

	CModel::Draw();
}

//===========================================================================================================
// �g��G�̕`�揈��
//===========================================================================================================
void CPlayer3D::MessengerDraw()
{
	if (m_pMessenger[m_FrontMessengerTag] != nullptr && m_pSkill->GetActSword() == false)
	{// �g��G�������� ���� ���X�L�����g�p��ԂłȂ��ꍇ

		m_pMessenger[m_FrontMessengerTag]->Draw();
	}
}

//========================================================================================================================
// �v���C���[�̓��͏���
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::InputPosPlayer()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	if (keyboard->GetPress(DIK_A) || joypad->GetPress(CInputJoypad::JOYKEY_LEFT))
	{
		m_Move.x += sinf(-D3DX_PI * 0.5f) * m_fSpeed;
		m_Move.y += cosf(-D3DX_PI * 0.5f) * m_fSpeed;

		SetDirection(CModel::DIRECTION_LEFT);
	}
	if (keyboard->GetPress(DIK_D)||joypad->GetPress(CInputJoypad::JOYKEY_RIGHT))
	{
		m_Move.x += sinf(-D3DX_PI * 0.5f) * -m_fSpeed;
		m_Move.y += cosf(-D3DX_PI * 0.5f) * -m_fSpeed;

		SetDirection(CModel::DIRECTION_RIGHT);
	}
	//if (keyboard->GetPress(DIK_W))
	//{
	//	m_Move.z += sinf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//	m_Move.x += cosf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//}
	//if (keyboard->GetPress(DIK_S))
	//{
	//	m_Move.z -= sinf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//	m_Move.x -= cosf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//}

	// �W�����v�d�͏���
	Jump();

	//if (keyboard->GetPress(DIK_C))
	//{
	//	m_Move.y -= m_fSpeed;
	//}
	//if (keyboard->GetPress(DIK_Z))
	//{
	//	m_Move.y += m_fSpeed;
	//}

	//// ��ʂ�h�炷����
	//if (keyboard->GetTrigger(DIK_F1))
	//{
	//	CManager::GetCamera()->SetShake(120, 5.0f);
	//}

	//// ������]
	//if (keyboard->GetTrigger(DIK_F2))
	//{
	//	m_pSword->TargetRot({ 0.0f,0.0f,2.2f }, 10);
	//}

	if (keyboard->GetTrigger(DIK_RETURN) || joypad->GetTriggerPedal(CInputJoypad::JOYKEY_RIGHT_TRIGGER))
	{
		Attack();
		m_pSword->TargetRot({ 0.0f,0.0f,GetDirection() * 2.2f }, 10);
		//m_pButtleUI[0]->SetBPush(true);
	}

	if (m_FrontMessengerTag != CEnemy3D::TAG_NONE)
	{
		FrontChange();

		UpddateMessengerList();
	}

	return m_Move;
}

//===========================================================================================================
// �X�L���̏�Ԃ��X�V
//===========================================================================================================
void CPlayer3D::UpdateSkillState()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	m_skillstate = SKILL_NONE;
	if (m_pSkill->GetInputSkillState() != CSkill::SKILL_SWORD)
	{// ���X�L�������ȊO�X�L�������Z�b�g����
		m_pSkill->SetInputSkillState(CSkill::SKILL_NONE);
	}

	std::vector<CEnemy3D*>pEnemy = CSearch::SearchMultiObject(pEnemy, ENEMY_MESSE);

	for (unsigned int nCnt = 0; nCnt < pEnemy.size(); nCnt++)
	{
		if (pEnemy[nCnt] != nullptr
			&& pEnemy[nCnt]->GetStateButtle() != CEnemy3D::BUTTLE_SKILL_THROW
			&& pEnemy[nCnt]->GetStateButtle() != CEnemy3D::BUTTLE_SKILL
			)
		{
			//// ���X�L������
			//if (keyboard->GetPress(DIK_LSHIFT) || joypad->GetPressPedal(CInputJoypad::JOYKEY_LEFT_TRIGGER))
			//{
			//	if (keyboard->GetTrigger(DIK_E) || joypad->GetTrigger(CInputJoypad::JOYKEY_Y))
			//	{// shift + E ���������Ƃ�

			//		if (m_pSkill->GetPossibleSword())
			//		{
			//			m_pSkill->SetInputSkillState(CSkill::SKILL_SWORD_BEFORE);
			//		}
			//	}
			//}
			
			if (m_pSkill->GetInputSkillState() != CSkill::SKILL_SWORD_BEFORE && m_pSkill->GetInputSkillState() != CSkill::SKILL_SWORD)
			{// ���X�L���g�p���́A�X�L���{�^���������Ȃ��悤�ɂ���
				if (keyboard->GetTrigger(DIK_E) || joypad->GetTrigger(CInputJoypad::JOYKEY_Y))
				{
					m_pSkill->SetInputSkillState(CSkill::SKILL_TRIGGER);
				}
				else if (keyboard->GetRelease(DIK_E) || joypad->GetRelese(CInputJoypad::JOYKEY_Y))
				{
					m_pSkill->SetInputSkillState(CSkill::SKILL_RELEASE);
					PlaySound(SOUND_LABEL_SLIMETHROW_SE);
				}
				else if (keyboard->GetPress(DIK_E) || joypad->GetPress(CInputJoypad::JOYKEY_Y))
				{
					m_pSkill->SetInputSkillState(CSkill::SKILL_PRESS);
					
				}
			}

			//===========================================================================================================
			// < debugmemo >
			// ���������X�L���N���X��������̂� Update �̏��Ԃ̊֌W�ŁA�����ɏ����Ȃ��Ƃ����Ȃ����ԂɂȂ��Ă���
			// ���Â炢�R�[�h�ɂȂ��Ă���B�ǂ��ɂ��������B
			//===========================================================================================================
			if (m_pSkill->GetSwordDurationtime() <= 0&&m_pSkill->GetActSword())
			{// �N�[���^�C�����Ȃ� + �X�L�����g�p��Ԃɂ���Ƃ�

				//m_pSkill->SetInputSkillState(CSkill::SKILL_SWORD_CANCEL);	// ���X�L�����g�p��Ԃ����������
				//m_pSkill->ActSword(false);	// �X�L���𖢎g�p��Ԃɂ���
			}
		}
	}

	if (m_pSkill->GetInputSkillState() == CSkill::SKILL_SWORD /*m_pSkill->GetActSword()*/)
	{// ���X�L���g�p��

		if (keyboard->GetTrigger(DIK_Q))
		{
			m_pSkill->SetInputSkillState(CSkill::SKILL_SWORD_CANCEL);
		}
	}
	else
	{// ���X�L���ȊO�g�p��
		if (keyboard->GetTrigger(DIK_Q)|| joypad->GetTriggerPedal(CInputJoypad::JOYKEY_LEFT_TRIGGER))
		{
			//m_skillstate = SKILL_CANCELL;

			m_pSkill->SetInputSkillState(CSkill::SKILL_CANCELL);
		}
	}
}

//========================================================================================================================
// �v���C���[�̓��͏���
//========================================================================================================================
void CPlayer3D::Jump()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	if (!m_bUseJump)
	{
		if (keyboard->GetTrigger(DIK_SPACE) || joypad->GetTrigger(CInputJoypad::JOYKEY_A))
		{
			m_Move.y += m_fJumpPower;	// �W�����v
			m_bUseJump = true;
		}
	}

	m_Move.y -= m_fGravity;	// �d�͉��Z
}

//===========================================================================================================
// �_���[�W���₷����
//===========================================================================================================
void CPlayer3D::AddDamage(int nDamage)
{
	if (!m_bDammage)
	{// �_���[�W��Ԃł͂Ȃ��Ƃ�

		m_nCntDamage = m_MaxInterval;	// �_���[�W��Ԃ̃J�E���g��ݒ�
		m_nLife -= nDamage;	// �_���[�W�����C�t�����炷
		m_bDammage = true;	// �_���[�W��Ԃɂ���
	}
}

//===========================================================================================================
// �_���[�W��Ԃ̍X�V
//===========================================================================================================
void CPlayer3D::UpdateDamage()
{
	m_nCntDamage--;	// �_���[�W��Ԃ̃J�E���g�����炷
	bMat(true);

	if (m_nCntDamage <= 0)
	{
		bMat(false);
		m_bDammage = false;
	}
}

//===========================================================================================================
// ���񂾂Ƃ��̏���
//===========================================================================================================
void CPlayer3D::DeathPlayer()
{
	if (GetPos().y <= -50.0f || m_nLife <= 0)
	{// ���ɗ������Ƃ� || �̗͂��Ȃ��Ȃ����Ƃ�
		m_nLife = m_MaxLife;			// �̗͂��ő�l�ɖ߂�
		SetPos({ 40.0f, 25.0f, 0.0f });	// ���X�|�[��������
		m_bSlip = true;					// ���蔲��������
		m_nSlipCnt = 5;					// ���蔲�������鎞�Ԃ�ݒ�

		if (m_pMessenger[m_FrontMessengerTag] == nullptr)
		{ return; }

		m_pMessenger[m_FrontMessengerTag]->TargetWarp(GetPos());	// ���݂̎g��G�� player �Ƀ��[�v������
	}
}

//===========================================================================================================
// �v���C���[�̍U������
//===========================================================================================================
void CPlayer3D::Attack()
{
	//CEnemy3D* pEnemy = nullptr;

	std::vector<CEnemy3D*>pEnemyMulti{};
	std::vector<CSlimeBoss*>pSlimeMulti{};

	pEnemyMulti = CSearch::SearchMultiObject(pEnemyMulti, TYPE::ENEMY);
	pSlimeMulti = CSearch::SearchMultiObject(pSlimeMulti, TYPE::ENEMY_BOSS);

	for (unsigned int nCnt = 0; nCnt < pEnemyMulti.size(); nCnt++)
	{
		D3DXVECTOR3 EnemyPos, EnemySize;

		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 size = GetSize();

		EnemyPos = pEnemyMulti[nCnt]->GetPos();
		EnemySize = pEnemyMulti[nCnt]->GetSize();
		int Direction = GetDirection();

		if (EnemyPos.x - EnemySize.x <= pos.x + size.x + (20.0f * Direction)
			&& EnemyPos.x + EnemySize.x >= pos.x - size.x + (20.0f * Direction)
			&& EnemyPos.y - EnemySize.y <= pos.y + size.y
			&& EnemyPos.y + EnemySize.y >= pos.y - size.y
			)
		{
			if (pEnemyMulti[nCnt]->GetState() != CEnemy3D::STATE_DAMAGE)
			{
				if (m_pSkill != nullptr && m_pSkill->GetInputSkillState() == CSkill::SKILL_SWORD /*m_pSkill->GetActSword()*/)
				{
					pEnemyMulti[nCnt]->Sufferdamage(10);

					if (!pEnemyMulti[nCnt]->GetSufferKnockback())
					{
						SwordAttack(pEnemyMulti[nCnt]);	// ���X�L���̍U��
					}
				}
				else
				{
					pEnemyMulti[nCnt]->Sufferdamage(15);
				}
			}
			
		}
	}

	for (unsigned int nCnt = 0; nCnt < pSlimeMulti.size(); nCnt++)
	{
		D3DXVECTOR3 EnemyPos, EnemySize;

		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 size = GetSize();

		EnemyPos = pSlimeMulti[nCnt]->GetPos();
		EnemySize = pSlimeMulti[nCnt]->GetSize();

		int Direction = GetDirection();

		if (EnemyPos.x - EnemySize.x <= pos.x + size.x + (20.0f * Direction)
			&& EnemyPos.x + EnemySize.x >= pos.x - size.x + (20.0f * Direction)
			&& EnemyPos.y - EnemySize.y <= pos.y + size.y
			&& EnemyPos.y + EnemySize.y >= pos.y - size.y
			)
		{
			if (pSlimeMulti[nCnt]->GetState() != CEnemy3D::STATE_DAMAGE)
			{
				if (m_pSkill != nullptr && m_pSkill->GetInputSkillState() == CSkill::SKILL_SWORD /*m_pSkill->GetActSword()*/)
				{
					pSlimeMulti[nCnt]->Sufferdamage(10);

					if (!pSlimeMulti[nCnt]->GetSufferKnockback())
					{
						SwordAttack(pSlimeMulti[nCnt]);	// ���X�L���̍U��
					}
				}
				else
				{
					pSlimeMulti[nCnt]->Sufferdamage(15);
				}
			}

		}
	}
}

//========================================================================================================================
// ��������
//========================================================================================================================
CPlayer3D* CPlayer3D::Create(D3DXVECTOR3 pos)
{
	CPlayer3D* pPlayer3D = new CPlayer3D;	// �v���C���[�̌^�œ��I�m��

	pPlayer3D->SetPos(pos);	// �ʒu��ݒ�
	pPlayer3D->SetType(TYPE::PLAYER);

	pPlayer3D->Init();	// �RD�̏����ݒ�

	return pPlayer3D;
}

//===========================================================================================================
// �g��G�𐶐�����
//===========================================================================================================
void CPlayer3D::CreateTarget()
{
	//===========================================================================================================
	// < debugmemo >
	// ���̏����A����� player.cpp �ł����̂��H
	// ���� player �� pos ���K�v�����炱���ɏ����Ă��邪����̓N���X�Ƃ��āA���������Ԉ���Ă���̂ł́H
	// ���������Afragment_manager �͂���ł����̂��H�����̃N���X�݌v����������B
	//===========================================================================================================

	D3DXVECTOR3 pos = GetPos();

	// fragmentmanager�������Ă���
	CFragmentManager* pFragMG = CGame::GetFragmentMG();

	for (int nCntTag = 0; nCntTag < CEnemy3D::TAG_MAX; nCntTag++)
	{// �S�Ẵ^�O�ŒT��

		if (!pFragMG->GetAppear((CEnemy3D::TAG_ENEMY)nCntTag))
		{// ���̎g��G���g���ς݂łȂ����

			int Fragnum = pFragMG->GetFragmentNum((CEnemy3D::TAG_ENEMY)nCntTag);	// ���̃^�O�̌��Ђ̐����擾����

			if (Fragnum >= 10)
			{// �^�O�������W�܂�����

				// ���ꂼ��̃^�O�ŃN���G�C�g����
				switch (nCntTag)
				{
				case CEnemy3D::TAG_SLIME:
					m_pMessenger[nCntTag] = CSlimePlayer::Create({ pos.x,pos.y + 10,pos.z });	// ��������Ɠ����� player �Ƀ|�C���^��n���Ă���

					//m_FrontMessengerTag = (CEnemy3D::TAG_ENEMY)nCntTag;	// �������ꂽ�g��G��O�ɏo��

					pFragMG->SetAppear((CEnemy3D::TAG_ENEMY)nCntTag);	// ���������Ƃ����̂����� bool

					break;

				case CEnemy3D::TAG_GOLEM:
					m_pMessenger[nCntTag] = CGolemPlayer::Create({ pos.x,pos.y + 10,pos.z });	// ��������Ɠ����� player �Ƀ|�C���^��n���Ă���

					pFragMG->SetAppear((CEnemy3D::TAG_ENEMY)nCntTag);	// ���������Ƃ����̂����� bool

					break;

				case CEnemy3D::TAG_NONE:
					assert(0);
					break;

				default:
					assert(0);
					break;
				}

				if (m_FrontMessengerTag == CEnemy3D::TAG_NONE)
				{
					m_FrontMessengerTag = (CEnemy3D::TAG_ENEMY)nCntTag;	// �������ꂽ�g��G��O�ɏo��
				}

				if (m_pSkill == nullptr)
				{
					m_pSkill = new CSkill;	// �X���C���X�L���𐶐�����
				}
			}
		}
	}
}

void CPlayer3D::SetButtleUI()
{
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_FRONT] = CButtleUI::Create({ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.9f,0.0f }, { 150.0f,150.0f,0.0f });	// �퓬UI�̃|�C���^���擾
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_NEXT] = CButtleUI::Create({ SCREEN_WIDTH * 0.6f,SCREEN_HEIGHT * 0.9f,0.0f }, { 100.0f,100.0f,0.0f });	// �퓬UI�̃|�C���^���擾
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_PREV] = CButtleUI::Create({ SCREEN_WIDTH * 0.4f,SCREEN_HEIGHT * 0.9f,0.0f }, { 100.0f,100.0f,0.0f });	// �퓬UI�̃|�C���^���擾


	for (int n = 0; n < CUISkill::UI_SKILL_MAX; n++)
	{
		CUISkill::Create((CUISkill::UI_SKILL)n);
	}

	// HP �o�[�̐���
	CHpMeter::Create(
		{ 10.0f,680.0f,0.0f },
		{ 400.0f,20.0f,0.0f },
		CHpMeter::HP_TYPE::HP_LITERAL,
		GetType()
	);

	CHpMeter::Create(
		{ 10.0f,680.0f,0.0f },
		{ 400.0f,20.0f,0.0f },
		CHpMeter::HP_TYPE::HP_VARIABLE,
		GetType()
	);
}

//========================================================================================================================
// player��pos��camera��target�ɃZ�b�g����
//========================================================================================================================
void CPlayer3D::SetTargetCamera()
{
	CCamera* pCamera = CManager::GetCamera();	// camera �������Ă���
	pCamera->SetTargetPos(GetPos());			// camera �� playerpos ������
}

//===========================================================================================================
// ���X�L���̍U��
//===========================================================================================================
void CPlayer3D::SwordAttack(CEnemy3D* pEnemy)
{
	int Direction = GetDirection();

	// �\�ɏo�Ă���g��G�̎�ނŔ���
	switch (m_FrontMessengerTag)
	{
	case CEnemy3D::TAG_SLIME:

		// �m�b�N�o�b�N������
		pEnemy->SetMove({
			5.0f * Direction,
			3.0f,
			0.0f
			});

		break;
	}
}

//===========================================================================================================
// �퓬�ɏo���g��G��ς���
//===========================================================================================================
void CPlayer3D::FrontChange()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();
	int tagnext = m_FrontMessengerTag;
	int tagprev = m_FrontMessengerTag;
	tagnext += 1;
	tagprev -= 1;

	if (keyboard->GetTrigger(DIK_1) || joypad->GetTrigger(CInputJoypad::JOYKEY_RIGHT_SHOULDER))
	{
		while (m_pMessenger[tagprev] == nullptr)
		{
			if (tagprev == CEnemy3D::TAG_NONE)
			{
				tagprev = CEnemy3D::TAG_MAX;
			}

			tagprev--;
		}

		m_FrontMessengerTag = (CEnemy3D::TAG_ENEMY)tagprev;

		if (m_pMessenger[m_FrontMessengerTag]->GetState() != CEnemy3D::STATE_DEAD)
		{
			m_pMessenger[m_FrontMessengerTag]->TargetWarp(GetPos());
		}
	}
	else if (keyboard->GetTrigger(DIK_2) || joypad->GetTrigger(CInputJoypad::JOYKEY_LEFT_SHOULDER))
	{
		while (m_pMessenger[tagnext] == nullptr)
		{
			if (tagnext == CEnemy3D::TAG_MAX)
			{
				tagnext = CEnemy3D::TAG_NONE;
			}

			tagnext++;
		}

		m_FrontMessengerTag = (CEnemy3D::TAG_ENEMY)tagnext;

		if (m_pMessenger[m_FrontMessengerTag]->GetState() != CEnemy3D::STATE_DEAD)
		{
			m_pMessenger[m_FrontMessengerTag]->TargetWarp(GetPos());
		}
	}
}

//========================================================================================================================
// �e�I�u�W�F�N�g���m�F����
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::LookObjectCollision(COLLISION_TYPE type)
{
	D3DXVECTOR3 pos = {};

	for (int nPrio = 0; nPrio < CObject::PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = CObject::GetTop(nPrio);

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();

			switch (type)
			{
				// �c�̓����蔻��(z��)
			case COLLISION_VERTICAL:
				pos = VerticalCollision(pObj);
				break;

				// ���̓����蔻��(x��)
			case COLLISION_HORIZON:
				pos = HorizonCollision(pObj);
				break;

				// �㉺�̓����蔻��(y��)
			case COLLISION_HIGHLOW:
				pos = HighLowCollision(pObj);
				break;

			default:
				assert(1);
				break;
			}

			pObj = pNext;
		}
	}

	return pos;
}

//========================================================================================================================
// �c�̓����蔻��
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::VerticalCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 PlayerPos = GetPos();
	D3DXVECTOR3 PlayerSize = GetSize();

	PlayerSize.x *= 0.5f;
	PlayerSize.y *= 0.5f;
	PlayerSize.z *= 0.5f;

	if (type == TYPE::BLOCK)
	{
		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		if (BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z		// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// �v���C���[���u���b�N��
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// xy�͈͓��ɂ����Ƃ�
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// -------------------------------------------
			&& m_oldPos.z + PlayerSize.z <= BlockPos.z - BlockSize.z	// �v���C���[�̉ߋ��̈ʒu���u���b�N����O�ɂ������Ƃ�
			)
		{// ��ɏ��
			m_Move.z = 0;

			SetPos({ PlayerPos.x,PlayerPos.y,PlayerPos.z = (BlockPos.z - BlockSize.z - PlayerSize.z) });
		}

		if (BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z		// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// �v���C���[���u���b�N��
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// xy�͈͓��ɂ����Ƃ�
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// -------------------------------------------
			&& m_oldPos.z - PlayerSize.z >= BlockPos.z + BlockSize.z	// �v���C���[�̉ߋ��̈ʒu���u���b�N��艜�ɂ������Ƃ�
			)
		{
			m_Move.z = 0;

			SetPos({ PlayerPos.x,PlayerPos.y ,PlayerPos.z = (BlockPos.z + BlockSize.z + PlayerSize.z) });
		}
	}

	return PlayerPos;
}

//========================================================================================================================
// ���̓����蔻��
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::HorizonCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 PlayerPos = GetPos();
	D3DXVECTOR3 PlayerSize = GetSize();

	PlayerSize.x *= 0.5f;
	PlayerSize.y *= 0.5f;
	PlayerSize.z *= 0.5f;

	if (type == TYPE::BLOCK)
	{
		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		if (BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x		// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// -------------------------------------------
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// �v���C���[���u���b�N��
			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// yz�͈͓��ɂ����Ƃ�
			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			&& m_oldPos.x - PlayerSize.x >= BlockPos.x + BlockSize.x	// �v���C���[�̉ߋ��̈ʒu���u���b�N���E�ɂ������Ƃ�
			)
		{
			m_Move.x = 0;

			SetPos({ PlayerPos.x = (BlockPos.x + BlockSize.x + PlayerSize.x),PlayerPos.y,PlayerPos.z });
		}

		else if (BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// -------------------------------------------
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// �v���C���[���u���b�N��
			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// yz�͈͓��ɂ����Ƃ�
			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			&& m_oldPos.x + PlayerSize.x <= BlockPos.x - BlockSize.x	// �v���C���[�̉ߋ��̈ʒu���u���b�N��荶�ɂ������Ƃ�
			)
		{
			m_Move.x = 0;

			SetPos({ PlayerPos.x = (BlockPos.x - BlockSize.x - PlayerSize.x),PlayerPos.y,PlayerPos.z });
		}
	}

	return PlayerPos;
}

//========================================================================================================================
// ���̓����蔻��
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::HighLowCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 PlayerPos = GetPos();
	D3DXVECTOR3 PlayerSize = GetSize();

	PlayerSize.x *= 0.5f;
	PlayerSize.z *= 0.5f;
	PlayerSize.y *= 0.5f;

	if (type == TYPE::BLOCK)
	{// �^�C�v���u���b�N�������ꍇ

		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		// �v���C���[���u���b�N�ɓ����Ԃ���
		if (BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y		// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// �v���C���[���u���b�N��
			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// xz�͈͓��ɂ����Ƃ�
			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			&& BlockPos.y - BlockSize.y >= m_oldPos.y + PlayerSize.y	// �v���C���[�̉ߋ��̈ʒu���u���b�N��艺�ɂ������Ƃ�
			)
		{
			m_Move.y = 0;

			SetPos({ PlayerPos.x,PlayerPos.y = (BlockPos.y - BlockSize.y - PlayerSize.y),PlayerPos.z });	// �v���C���[���u���b�N�̉��ɍs��
		}

		// �v���C���[���u���b�N�ɏ��
		else if (BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// �v���C���[���u���b�N��
			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// xz�͈͓��ɂ����Ƃ�
			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			&& BlockPos.y + BlockSize.y <= m_oldPos.y - PlayerSize.y	// �v���C���[�̉ߋ��̈ʒu���u���b�N����ɂ������Ƃ�
			)
		{
			m_bUseJump = false;
			m_Move.y = 0;
			SetPos({ PlayerPos.x,PlayerPos.y = (BlockPos.y + BlockSize.y + PlayerSize.y),PlayerPos.z });	// �v���C���[���u���b�N�̏�ɏ��
		}
	}

	return PlayerPos;
}