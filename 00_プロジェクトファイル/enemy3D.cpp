//==============================================================================================================================================
// 
// 3D���f���̓G�Ǘ�
// Author umeda souma
// 
//==============================================================================================================================================
#include "enemy3D.h"
#include "player3D.h"
#include "manager.h"
#include "block3D.h"
#include "fragmentlife.h"
#include "game.h"
#include "search.h"
#include "bullet3D.h"

int CEnemy3D::m_nFragmentNum = 0;

//===========================================================================================================
// �R���X�g���N�^
//===========================================================================================================
CEnemy3D::CEnemy3D()
	: m_TargetPos{}
	, m_fLife(0)
	, m_fPower(0.0f)
	, m_fAtkInterval(0)
	, m_bBattle(false)
	, m_StateButtle(BUTTLE_NONE)
	, m_Move{ 0.0f,0.0f,0.0f }
	, m_fDistance(0.0f)
	, m_fSpeed(0.0f)
	, m_State(STATE_CREATE)
	, m_OldState(STATE_NORMAL)
	, m_Follow(0.0f)
	, m_tag(TAG_NONE)
	, m_nCreateCnt(0)
	, m_nCntDamage(0)
	, m_bIsSkill(false)
	, m_bSufferKnockback(false)
	, m_nMaxLife(0)
	, m_nBulletInterval(0)
	, m_nRespawnCnt(0)
{
}

//===========================================================================================================
// �f�X�g���N�^
//===========================================================================================================
CEnemy3D::~CEnemy3D()
{
}

//===========================================================================================================
// �����ݒ�
//===========================================================================================================
HRESULT CEnemy3D::Init()
{
	m_fLife = m_nMaxLife;
	m_fPower = 1.0f;
	m_fAtkInterval = 120;
	m_Follow = 0.03f;
	m_fDistance = 30.0f;
	m_fSpeed = 0.5f;
	m_nCreateCnt = 3;

	CModel::Init();

	return S_OK;
}

//===========================================================================================================
// �I������
//===========================================================================================================
void CEnemy3D::Uninit()
{
	CModel::Uninit();
}

//===========================================================================================================
// �X�V����
//===========================================================================================================
void CEnemy3D::Update()
{
	m_OldState = m_State;

	D3DXVECTOR3 pos = GetPos();

	TargetPlayer();	// �v���C���[�� pos ���^�[�Q�b�g�ɂ���
	CreateCountDown();

	CSkill* pSkill = CPlayer3D::GetSkill();

	//if (pSkill != nullptr)
	//{
	//	pSkill->Update();
	//}
	

	if (CScene::GetMode() == CScene::MODE_EDITOR)
	{
		m_Move = { 0.0f,0.0f,0.0f };
	}

	if (m_State != STATE_CREATE && m_State != STATE_DEAD)
	{
		// Y���̓����蔻��E�X�V
		pos.y += m_Move.y;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_HIGHLOW);

		// �^�[�Q�b�g�ɒx��Ă��Ă���	
		if (pos.x < (m_TargetPos.x /*- m_fDistance*/))
		{
			SetDirection(DIRECTION::DIRECTION_RIGHT);

			//m_Move.x += m_fSpeed;
		}
		else if (pos.x > (m_TargetPos.x /*+ m_fDistance*/))
		{
			SetDirection(DIRECTION::DIRECTION_LEFT);

			//m_Move.x -= m_fSpeed;
		}

		// X���̓����蔻��E�X�V
		pos.x += m_Move.x;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_HORIZON);

		// Z���̓����蔻��E�X�V
		pos.z += m_Move.z;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_VERTICAL);

		// ���x���X�Ɍ���
		m_Move.x += (0.0f - m_Move.x) * 0.05f;
		m_Move.z += (0.0f - m_Move.z) * 0.1f;
		//m_Move.y += (0.0f - m_Move.y) * 0.2f;

		//pos.y += (m_TargetPos.y - pos.y) * m_Follow;

		if (m_fLife <= 0 && GetType() != TYPE::ENEMY_MESSE || GetPos().y <= -50.0f && GetType() != TYPE::ENEMY_MESSE)
		{
			CFlagmentLife::SetNumParticle({pos.x,pos.y+GetSize().y*0.5f,pos.z}, 30, 10, m_tag);	// ���̌��Ђ��o��

			Uninit();	// �G������
			return;
		}
		else if (m_fLife <= 0 && GetType() == TYPE::ENEMY_MESSE)
		{
			if (m_State != STATE_DEAD)
			{
				m_nRespawnCnt = 0;
				m_State = STATE_DEAD;
			}
		}

	}

	if (GetType() != CObject::ENEMY_MESSE)
	{
		UpdateState();
	}

	CModel::Update();

	// �ߋ��̈ʒu��o�^
	m_oldPos = pos;	
}

//===========================================================================================================
// �`�揈��
//===========================================================================================================
void CEnemy3D::Draw()
{
	if (m_State != STATE_DEAD)
	{
		CModel::Draw();
	}
}

//===========================================================================================================
// ��������
//===========================================================================================================
CEnemy3D* CEnemy3D::Create(D3DXVECTOR3 pos)
{
	CEnemy3D* pEnemy3D = new CEnemy3D;

	pEnemy3D->SetPos(pos);
	pEnemy3D->SetSize({ MAX_SIZE,MAX_SIZE,MAX_SIZE });
	pEnemy3D->SetType(TYPE::ENEMY);

	pEnemy3D->Init();

	return pEnemy3D;
}

//===========================================================================================================
// �^�[�Q�b�g�ʒu�̐ݒ�
//===========================================================================================================
void CEnemy3D::SetTarget(D3DXVECTOR3 pos)
{
	m_TargetPos = pos;
}

//===========================================================================================================
// ��Ԃ̐ݒ�
//===========================================================================================================
void CEnemy3D::SetState(STATE_ENEMY state)
{
	m_State = state;

	if (state == STATE_CREATE) { m_nCreateCnt = MAX_CREATECOUNT; }
}

//===========================================================================================================
// ��Ԃ̐ݒ�
//===========================================================================================================
void CEnemy3D::UpdateState()
{
	if (m_State != STATE_SKILL && m_State != STATE_SWORD && m_State != STATE_BEFORE_SKILL)
	{
		switch (m_State)
		{
		case STATE_NORMAL:
			bMat(false);
			m_State = STATE_MOVE;
			break;

		case STATE_MOVE:

			break;

		case STATE_DAMAGE:
			bMat(true);

			if (m_nCntDamage > 0)
			{
				m_nCntDamage--;
			}
			else
			{
				m_State = STATE_NORMAL;
			}
			break;

		case STATE_ATTACK:
			m_State = STATE_NORMAL;

			break;

		case STATE_CREATE:
			break;

		case STATE_DEAD:

			if (m_nRespawnCnt >= 60 * 15)
			{
				m_fLife = m_nMaxLife;
				m_nRespawnCnt = 0;

				CPlayer3D* pPlayer = nullptr;
				pPlayer = CSearch::SearchObject(pPlayer, CObject::PLAYER);

				TargetWarp(pPlayer->GetPos());

				return;
			}

			m_nRespawnCnt++;
			break;
		}
	}

	
}

//===========================================================================================================
// �v���C���[���^�[�Q�b�g�ɐݒ�
//===========================================================================================================
void CEnemy3D::TargetPlayer()
{
	CPlayer3D* pPlayer = nullptr;
	D3DXVECTOR3 playerpos;
	D3DXVECTOR3 playersize;

	pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);

	if (pPlayer != nullptr)
	{
		playerpos = pPlayer->GetPos();
		playersize = pPlayer->GetSize();

		if (GetType() == ENEMY_MESSE)
		{ 
			float dis = 300.0f;
			D3DXVECTOR3 pos = GetPos();
			D3DXVECTOR3 size = GetSize();

			if (playerpos.x - dis <= pos.x + size.x
				&& playerpos.x + dis >= pos.x - size.x
				&& playerpos.y - dis <= pos.y + size.y
				&& playerpos.y + dis >= pos.y - size.y
				)
			{
			}
			else
			{
				TargetWarp(playerpos);
			}
		}

		SetTarget(playerpos);
	}

	
}

//===========================================================================================================
// STATE_CREATE���̃t���[���J�E���g
//===========================================================================================================
void CEnemy3D::CreateCountDown()
{
	if (m_State == STATE_CREATE)
	{ 
		m_nCreateCnt--; 	// �J�E���g�����炷

		if (m_nCreateCnt <= 0)
		{
			m_State = STATE_NORMAL;	// �G�𕁒ʂ̏�Ԃɖ߂�
		}
	}

	
}

//========================================================================================================================
// �e�I�u�W�F�N�g���m�F����
//========================================================================================================================
D3DXVECTOR3 CEnemy3D::LookObjectCollision(COLLISION_TYPE type)
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
	

	//// �e�I�u�W�F�N�g���m�F����
	//for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	//{
	//	CObject* pObj = GetOBJECT(PRIORITY_DEFAULT, nCntObj);

	//	if (pObj != nullptr)
	//	{// nullptr�łȂ�������e���̓����蔻�������
	//		switch (type)
	//		{
	//			// �c�̓����蔻��(z��)
	//		case COLLISION_VERTICAL:
	//			pos = VerticalCollision(pObj);
	//			break;

	//			// ���̓����蔻��(x��)
	//		case COLLISION_HORIZON:
	//			pos = HorizonCollision(pObj);
	//			break;

	//			// �㉺�̓����蔻��(y��)
	//		case COLLISION_HIGHLOW:
	//			pos = HighLowCollision(pObj);
	//			break;

	//		default:
	//			assert(1);
	//			break;
	//		}
	//	}
	//}

	return pos;
}

//========================================================================================================================
// �c�̓����蔻��
//========================================================================================================================
D3DXVECTOR3 CEnemy3D::VerticalCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 EnemyPos = GetPos();
	D3DXVECTOR3 EnemySize = GetSize();

	EnemySize.x *= 0.5f;
	EnemySize.y *= 0.5f;
	EnemySize.z *= 0.5f;

	if (type == TYPE::BLOCK)
	{
		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		//BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		if (BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z		// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x	// �v���C���[���u���b�N��
			&& BlockPos.y /*- BlockSize.y*/ < EnemyPos.y + EnemySize.y*2	// xy�͈͓��ɂ����Ƃ�
			&& BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// -------------------------------------------
			&& m_oldPos.z + EnemySize.z <= BlockPos.z - BlockSize.z	// �v���C���[�̉ߋ��̈ʒu���u���b�N����O�ɂ������Ƃ�
			)
		{// ��ɏ��
			m_Move.z = 0;

			SetPos({ EnemyPos.x,EnemyPos.y,EnemyPos.z = (BlockPos.z - BlockSize.z - EnemySize.z) });
		}

		if (BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z		// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x	// �v���C���[���u���b�N��
			&& BlockPos.y/* - BlockSize.y*/ < EnemyPos.y + EnemySize.y*2	// xy�͈͓��ɂ����Ƃ�
			&& BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// -------------------------------------------
			&& m_oldPos.z - EnemySize.z >= BlockPos.z + BlockSize.z	// �v���C���[�̉ߋ��̈ʒu���u���b�N��艜�ɂ������Ƃ�
			)
		{
			m_Move.z = 0;

			SetPos({ EnemyPos.x,EnemyPos.y ,EnemyPos.z = (BlockPos.z + BlockSize.z + EnemySize.z) });
		}
	}

	return EnemyPos;
}

//========================================================================================================================
// ���̓����蔻��
//========================================================================================================================
D3DXVECTOR3 CEnemy3D::HorizonCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 EnemyPos = GetPos();
	D3DXVECTOR3 EnemySize = GetSize();

	EnemySize.x *= 0.5f;
	EnemySize.y *= 0.5f;
	EnemySize.z *= 0.5f;

	if (type == TYPE::BLOCK)
	{
		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		//BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		if (BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x		// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.y /*- BlockSize.y*/ < EnemyPos.y + EnemySize.y*2	// -------------------------------------------
			&& BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// �v���C���[���u���b�N��
			&& BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z	// yz�͈͓��ɂ����Ƃ�
			&& BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z	// -------------------------------------------
			&& m_oldPos.x - EnemySize.x >= BlockPos.x + BlockSize.x	// �v���C���[�̉ߋ��̈ʒu���u���b�N���E�ɂ������Ƃ�
			)
		{
			//m_Move.x = 0;

			SetPos({ EnemyPos.x = (BlockPos.x + BlockSize.x + EnemySize.x),EnemyPos.y,EnemyPos.z });
		}

		else if (BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.y /*- BlockSize.y*/ < EnemyPos.y + EnemySize.y*2	// -------------------------------------------
			&& BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// �v���C���[���u���b�N��
			&& BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z	// yz�͈͓��ɂ����Ƃ�
			&& BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z	// -------------------------------------------
			&& m_oldPos.x + EnemySize.x <= BlockPos.x - BlockSize.x	// �v���C���[�̉ߋ��̈ʒu���u���b�N��荶�ɂ������Ƃ�
			)
		{
			//m_Move.x = 0;

			SetPos({ EnemyPos.x = (BlockPos.x - BlockSize.x - EnemySize.x),EnemyPos.y,EnemyPos.z });
		}
	}

	return EnemyPos;
}

//========================================================================================================================
// ���̓����蔻��
//========================================================================================================================
D3DXVECTOR3 CEnemy3D::HighLowCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 EnemyPos = GetPos();
	D3DXVECTOR3 EnemySize = GetSize();

	EnemySize.x *= 0.5f;
	EnemySize.z *= 0.5f;
	EnemySize.y *= 0.5f;

	if (type == TYPE::BLOCK)
	{// �^�C�v���u���b�N�������ꍇ

		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		//BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		// �v���C���[���u���b�N�ɓ����Ԃ���
		if (BlockPos.y /*- BlockSize.y*/ < EnemyPos.y + EnemySize.y*2		// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x	// �v���C���[���u���b�N��
			&& BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z	// xz�͈͓��ɂ����Ƃ�
			&& BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z	// -------------------------------------------
			&& BlockPos.y /*- BlockSize.y*/ >= m_oldPos.y + EnemySize.y*2	// �v���C���[�̉ߋ��̈ʒu���u���b�N��艺�ɂ������Ƃ�
			)
		{
			m_Move.y = 0;

			//if (GetType() == TYPE::ENEMY_MESSE) { m_Move.x = 0; }

			SetPos({ EnemyPos.x,EnemyPos.y = (BlockPos.y /*- BlockSize.y*/ - EnemySize.y*2),EnemyPos.z });	// �v���C���[���u���b�N�̉��ɍs��
		}

		// �v���C���[���u���b�N�ɏ��
		else if (BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// �v���C���[���u���b�N�ɂ߂荞�񂾎�
			&& BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x	// �v���C���[���u���b�N��
			&& BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z	// xz�͈͓��ɂ����Ƃ�
			&& BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z	// -------------------------------------------
			&& BlockPos.y + BlockSize.y <= m_oldPos.y /*- EnemySize.y*/	// �v���C���[�̉ߋ��̈ʒu���u���b�N����ɂ������Ƃ�
			)
		{
			m_Move.y = 0;

			// �X���C�����n�ʂɏ�����Ƃ��A���̈ړ��ʂ������ăX���C�����ۂ����������B
			if (GetTag() == TAG_SLIME) 
			{
				m_Move.x = 0.0f; 
			}

			if (GetType() == TYPE::ENEMY_MESSE && GetTag() == TAG_SLIME &&  /*m_StateButtle == BUTTLE_SKILL_THROW*/ m_State == STATE_BEFORE_SKILL) 
			{ 
				/*m_StateButtle = BUTTLE_SKILL;*/ m_State = STATE_SKILL;	// �X���C���̓X�L�������̌�A�X�L����ԂɂȂ�
			}	

			SetPos({ EnemyPos.x,EnemyPos.y = (BlockPos.y + BlockSize.y /*+ EnemySize.y*/),EnemyPos.z });	// �v���C���[���u���b�N�̏�ɏ��
		}
	}

	return EnemyPos;
}

//===========================================================================================================
// �G�̎擾
//===========================================================================================================
CEnemy3D* CEnemy3D::enemyget()
{
	for (int nCntPrio = 0; nCntPrio < CObject::PRIORITY_MAX; nCntPrio++)
	{
		for (int nCntObj = 0; nCntObj < CObject::MAX_OBJECT; nCntObj++)
		{
			CObject* pObj = CObject::GetOBJECT(nCntPrio, nCntObj);

			if (pObj != nullptr)
			{
				if (pObj->GetType() == CObject::TYPE::ENEMY)
				{
					CEnemy3D* pEnemy = (CEnemy3D*)pObj;

					return pEnemy;
				}
			}
		}
	}

	return nullptr;
}

//===========================================================================================================
// �^�[�Q�b�g�Ƀ��[�v����
//===========================================================================================================
void CEnemy3D::TargetWarp(D3DXVECTOR3 playerpos)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();

	{// �v���C���[�Ƃ̋��������ꂽ�Ƃ�

		CPlayer3D* pPlayer = nullptr;
		pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);

		m_State = STATE_CREATE;	// �����蔻����Ȃ���
		m_nCreateCnt = 3;	// �Ȃ����Ă���t���[��
		SetPos({ playerpos.x,playerpos.y+pPlayer->GetSize().y,playerpos.z });	// �v���C���[�܂ňړ�����
		SetStateButtle(BUTTLE_NONE);
	}
}

//===========================================================================================================
// �_���[�W���󂯂��Ƃ��̏���
//===========================================================================================================
void CEnemy3D::Sufferdamage(int damage)
{
	m_fLife -= damage;
	m_State = STATE_DAMAGE;
	m_StateButtle = BUTTLE_NONE;
	m_nCntDamage = 15;

}

void CEnemy3D::UpdateSkillSwordDamage()
{
	
}