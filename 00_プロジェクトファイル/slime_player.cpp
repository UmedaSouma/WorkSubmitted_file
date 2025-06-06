//===========================================================================================================================================================
// 
// �g���X���C���̏��� [slime_player.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "slime_player.h"
#include "manager.h"
#include "player3D.h"
#include "search.h"
#include "skill.h"
#include "sound.h"

//========================================================================================================================
// �R���X�g���N�^
//========================================================================================================================
CSlimePlayer::CSlimePlayer():
	m_nSoundCnt(0)
{
}

//========================================================================================================================
// �f�X�g���N�^
//========================================================================================================================
CSlimePlayer::~CSlimePlayer()
{
}

//========================================================================================================================
// �����ݒ�
//========================================================================================================================
HRESULT CSlimePlayer::Init()
{
	SetMaxLife(159);

	CSlime::Init();

	// ���f���f�[�^���Z�b�g
	SetModelAddress("data\\model\\enemy_slime_001.x");		// �A�h���X��ۑ����Ă���
	CModeldata* pModeldata = CManager::GetModeldata();		// modeldata �̃|�C���^�������Ă���
	int nIdx = pModeldata->Regist(GetModelAddress());		// ���f���f�[�^�̓o�^
	BindModel(pModeldata->GetAddress(nIdx));				// ���f�������Z�b�g����

	return S_OK;
}

//========================================================================================================================
// �I������
//========================================================================================================================
void CSlimePlayer::Uninit()
{
	CSlime::Uninit();
}

//========================================================================================================================
// �X�V����
//========================================================================================================================
void CSlimePlayer::Update()
{

	if (GetState() != CEnemy3D::STATE_DEAD)
	{
		CPlayer3D* pPlayer = nullptr;

		pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);

		CSkill* pSkill = CPlayer3D::GetSkill();	// �X�L���̏����擾
		CSkill::SKILL_INPUT_STATE pSkillState = pSkill->GetInputSkillState();	// �X�L���̏�Ԃ��擾����

		STATE_ENEMY State_ = STATE_NORMAL;	// �G�̏��
		STATE_BUTTLE State_B = BUTTLE_NONE;	// �퓬���

		bool IsUseSkill = false;

		if (GetState() == STATE_BEFORE_SKILL
			|| GetState() == STATE_SKILL
			|| GetState() == STATE_SWORD)
		{
			IsUseSkill = true;
		}

		if (GetState() == STATE_SKILL)
		{
			UseSkillUpdate();
		}

		switch (pSkillState)
		{

		case CSkill::SKILL_INPUT_STATE::SKILL_TRIGGER:

			if (IsUseSkill)
			{
				break;
			}

			State_ = GetState();

			SetState(STATE_CREATE);

			break;

		case CSkill::SKILL_INPUT_STATE::SKILL_PRESS:

			if (IsUseSkill)
			{
				break;
			}

			SetPos({
				pPlayer->GetPos().x,
				pPlayer->GetPos().y + 5.0f,
				pPlayer->GetPos().z
				});

			//SetState(STATE_NORMAL);	// �X�L����Ԃɂ���

			break;

		case CSkill::SKILL_INPUT_STATE::SKILL_RELEASE:

			if (IsUseSkill)
			{
				break;
			}

			State_B = GetStateButtle();
			State_ = GetState();

			SetMove({
			(4.0f * pPlayer->GetDirection())
			,3.0f
			,0.0f
				});

			SetState(STATE_BEFORE_SKILL);	// �X�L����Ԃɂ���

			SetStateButtle(BUTTLE_SKILL_THROW);

			break;

		case CSkill::SKILL_INPUT_STATE::SKILL_CANCELL:
			SetStateButtle(BUTTLE_NONE);
			SetState(STATE_NORMAL);
			break;

		case CSkill::SKILL_INPUT_STATE::SKILL_SWORD_BEFORE:

			if (IsUseSkill)
			{
				break;
			}

			pSkill->SetInputSkillState(CSkill::SKILL_SWORD);	// ���X�L���p����Ԃɂ���
			pSkill->SetSwordDurationtime(600);					// �X�L���p�����Ԑݒ�

			break;

		case CSkill::SKILL_INPUT_STATE::SKILL_SWORD:

			SetState(STATE_SWORD);

			break;
		case CSkill::SKILL_INPUT_STATE::SKILL_SWORD_CANCEL:

			SetState(STATE_CREATE);	// �����蔻����Ȃ���

			SetPos({
				pPlayer->GetPos().x,
				pPlayer->GetPos().y + 5.0f,
				pPlayer->GetPos().z
				});

			pSkill->SetSwordCooltime(720);	// �N�[���^�C����ݒ肷��

			CSlime::Update();	// �����蔻����Ȃ�������Ԃ� Update ��ʂ� Player �̏�Ɏ����Ă���

			SetState(STATE_NORMAL);	// state ��߂�

			break;
		}
	}

	m_nSoundCnt--;

	CSlime::Update();
}


//========================================================================================================================
// �`�揈��
//========================================================================================================================
void CSlimePlayer::Draw()
{
	//if (GetState() != STATE_SWORD)
	{
		CSlime::Draw();
	}
}

//========================================================================================================================
// ��������
//========================================================================================================================
CSlimePlayer* CSlimePlayer::Create(D3DXVECTOR3 pos)
{
	CSlimePlayer* pSlime = new CSlimePlayer;

	pSlime->SetPos(pos);
	pSlime->SetType(ENEMY_MESSE);

	pSlime->Init();

	return pSlime;
}

//===========================================================================================================
// �X�L�����g���Ă���Ƃ��̃A�b�v�f�[�g
//===========================================================================================================
void CSlimePlayer::UseSkillUpdate()
{
	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);

	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
		D3DXVECTOR3 PlayerSize = pPlayer->GetSize();
		D3DXVECTOR3 PlayerOldPos = pPlayer->GetOldPos();
		D3DXVECTOR3 PlayerMove = pPlayer->GetMove();

		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 size = GetSize()*0.5f;

		// �v���C���[���u���b�N�ɏ��
		if (pos.y + size.y > PlayerPos.y - PlayerSize.y*0.5f		// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& pos.x - size.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& pos.x + size.x > PlayerPos.x - PlayerSize.x	// �v���C���[���u���b�N��
			&& pos.z - size.z < PlayerPos.z + PlayerSize.z	// xz �͈͓��ɂ����Ƃ�
			&& pos.y - size.y < PlayerPos.y + PlayerSize.y	// y ��
			&& pos.z + size.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			//&& pos.y + size.y <= PlayerOldPos.y - PlayerSize.y	// �v���C���[�̉ߋ��̈ʒu���u���b�N����ɂ������Ƃ�
			)
		{
			if (m_nSoundCnt <= 0)
			{
				PlaySound(SOUND_LABEL_SLIMEJUMP_SE);
				m_nSoundCnt = 30;
			}
			
			pPlayer->SetbUseJump(true);	// �W�����v��������ɂ���
			pPlayer->SetMove({ PlayerMove.x+0.0f,10.0f,PlayerMove.z*0.0f });	// �v���C���[����Ɉړ�������
		}
	}
}