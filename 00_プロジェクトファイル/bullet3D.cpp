//===========================================================================================================================================================
// 
// �e�̏��� [bullet3D.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "bullet3D.h"
#include "manager.h"
#include "player3D.h"
#include "enemy3D.h"
#include "search.h"
#include "sound.h"

//========================================================================================================================
// �R���X�g���N�^
//========================================================================================================================
CBullet3D::CBullet3D() :
	m_nLife(0)
	, m_move({ 0.0f,0.0f,0.0f })
{
}

//========================================================================================================================
// �f�X�g���N�^
//========================================================================================================================
CBullet3D::~CBullet3D()
{
}

//========================================================================================================================
// �����ݒ�
//========================================================================================================================
HRESULT CBullet3D::Init()
{
	SetSize({ 5.0f,5.0f,0.0f });

	PlaySound(SOUND_LABEL_SHOT_SE);

	CModel::Init();

	// ���f���f�[�^���Z�b�g
	CModeldata* pModeldata = CManager::GetModeldata();	// modeldata �̃|�C���^�������Ă���

	SetModelAddress("data\\model\\attack_bullet_000.x");	// �A�h���X��ۑ����Ă���
	int nIdx = pModeldata->Regist(GetModelAddress());	// ���f���f�[�^�̓o�^
	BindModel(pModeldata->GetAddress(nIdx));		 	// ���f�������Z�b�g����

	return S_OK;
}

//========================================================================================================================
// �I������
//========================================================================================================================
void CBullet3D::Uninit()
{
	CModel::Uninit();
}

//========================================================================================================================
// �X�V����
//========================================================================================================================
void CBullet3D::Update()
{

	UpdateType();

	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}

	D3DXVECTOR3 pos = GetPos();
	pos += m_move;

	SetPos(pos);
	m_nLife--;

	CModel::Update();
}

//========================================================================================================================
// �`�揈��
//========================================================================================================================
void CBullet3D::Draw()
{
	CModel::Draw();
}

//========================================================================================================================
// ��������
//========================================================================================================================
CBullet3D* CBullet3D::Create(CObject::TYPE type, D3DXVECTOR3 pos,D3DXVECTOR3 move, int life)
{
	CBullet3D* pBullet3D = new CBullet3D;

	pBullet3D->m_type = type;

	pBullet3D->SetPos(pos);
	pBullet3D->m_move = move;
	pBullet3D->m_nLife = life;

	pBullet3D->Init();

	return pBullet3D;
}

//===========================================================================================================
// ��ނ��Ƃ̍X�V
//===========================================================================================================
void CBullet3D::UpdateType()
{
	// �I�u�W�F�N�g�̃^�C�v���ƂɍX�V��ς���
	if (m_type == CObject::ENEMY_MESSE)
	{
		UpdateAlly();
	}
	else
	{
		UpdateEnemy();
	}
}

//===========================================================================================================
// �����̍X�V
//===========================================================================================================
void CBullet3D::UpdateAlly()
{
	D3DXVECTOR3 targetpos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 targetsize = { 0.0f,0.0f,0.0f };

	int nDamage = 0;

	std::vector<CEnemy3D*>pEnemy;
	pEnemy = CSearch::SearchMultiObject(pEnemy, CObject::ENEMY);

	for (unsigned int nCntMesse = 0; nCntMesse < pEnemy.size(); nCntMesse++)
	{// �����݂���G�̐���
		if (pEnemy[nCntMesse] != nullptr)
		{// nullptr ����Ȃ�������
			targetpos = pEnemy[nCntMesse]->GetPos();	// �^�[�Q�b�g�� pos �擾
			targetsize = pEnemy[nCntMesse]->GetSize();	// �^�[�Q�b�g�� size �擾

			targetsize.x *= 0.5f;

			nDamage = Collision(targetpos, targetsize);	// �����蔻��F�����ŗ^����_���[�W���Ƃ��Ă���

			if (nDamage != 0)
			{// �^�[�Q�b�g���_���[�W��^����ꂽ�炱�̉��̏�����ʂ��Ȃ�

				pEnemy[nCntMesse]->Sufferdamage(nDamage);	// �����Ŏ����Ă����_���[�W��^����
				return;
			}
		}
	}

	std::vector<CEnemy3D*>pEnemyBoss;
	pEnemyBoss = CSearch::SearchMultiObject(pEnemyBoss, CObject::ENEMY_BOSS);

	for (unsigned int nCntBoss = 0; nCntBoss < pEnemyBoss.size(); nCntBoss++)
	{// �����݂���{�X�̐���
		if (pEnemyBoss[nCntBoss] != nullptr)
		{// nullptr ����Ȃ�������
			targetpos = pEnemyBoss[nCntBoss]->GetPos();		// �^�[�Q�b�g�� pos �擾
			targetsize = pEnemyBoss[nCntBoss]->GetSize();	// �^�[�Q�b�g�� size �擾);

			targetsize.x *= 0.5f;

			nDamage = Collision(targetpos, targetsize);		// �����蔻��F�����ŗ^����_���[�W���Ƃ��Ă���

			if (nDamage != 0)
			{// �^�[�Q�b�g���_���[�W��^����ꂽ�炱�̉��̏�����ʂ��Ȃ�

				pEnemyBoss[nCntBoss]->Sufferdamage(nDamage);	// �����Ŏ����Ă����_���[�W��^����;
				return;
			}
		}
	}
}

//===========================================================================================================
// �G�̍X�V
//===========================================================================================================
void CBullet3D::UpdateEnemy()
{
	D3DXVECTOR3 targetpos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 targetsize = { 0.0f,0.0f,0.0f };

	int nDamage = 0;

	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, CObject::PLAYER);

	if (pPlayer != nullptr)
	{
		targetpos = pPlayer->GetPos();
		targetsize = pPlayer->GetSize();

		targetsize.x *= 0.5f;

		nDamage = Collision(targetpos, targetsize);

		if (nDamage != 0)
		{// �^�[�Q�b�g���_���[�W��^����ꂽ�炱�̉��̏�����ʂ��Ȃ�

			pPlayer->AddDamage(nDamage);
			return;
		}
	}

	std::vector<CEnemy3D*>pEnemy;
	pEnemy = CSearch::SearchMultiObject(pEnemy, CObject::ENEMY_MESSE);

	for (unsigned int nCntMesse = 0; nCntMesse < pEnemy.size(); nCntMesse++)
	{
		if (pEnemy[nCntMesse] != nullptr)
		{
			if (pEnemy[nCntMesse]->GetState() != CEnemy3D::STATE_DEAD)
			{
				targetpos = pEnemy[nCntMesse]->GetPos();
				targetsize = pEnemy[nCntMesse]->GetSize();

				targetsize.x *= 0.5f;

				nDamage = Collision(targetpos, targetsize);

				if (nDamage != 0)
				{// �^�[�Q�b�g���_���[�W��^����ꂽ�炱�̉��̏�����ʂ��Ȃ�

					pEnemy[nCntMesse]->Sufferdamage(nDamage);
					return;
				}
			}
		}
	}

}

//===========================================================================================================
// �����蔻��
//===========================================================================================================
int CBullet3D::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	D3DXVECTOR3 B_pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 B_size = { 0.0f,0.0f,0.0f };

	B_pos = GetPos();
	B_size = GetSize();

	if (pos.x - size.x <= B_pos.x + B_size.x
		&& pos.x + size.x >= B_pos.x - B_size.x
		&& pos.y /*- size.y*/ <= B_pos.y + B_size.y
		&& pos.y + size.y >= B_pos.y - B_size.y)
	{
		Uninit();
		return 15;
	}

	return 0;
}