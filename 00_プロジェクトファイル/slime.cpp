#include "slime.h"
#include "scene.h"
#include "search.h"
#include "bullet3D.h"
#include "skill.h"

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CSlime::CSlime() :
	m_nCntJumptime(0)
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CSlime::~CSlime()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CSlime::Init()
{
	int maxlife = GetMaxLife();
	maxlife = 20;
	SetTag(TAG_SLIME);

	float size = 10.0f;

	CEnemyland3D::Init();

	SetSize({ size,size*1.5f,size });

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CSlime::Uninit()
{
	CEnemyland3D::Uninit();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CSlime::Update()
{
	D3DXVECTOR3 move = GetMove();
	int dire = GetDirection();
	CSkill* pSkill = CPlayer3D::GetSkill();

	if (GetState() == STATE_MOVE
		||GetState()==STATE_DAMAGE
		)
	{
		if (m_nCntJumptime % 60 == 0)
		{
			SetMove({ 2.0f * dire,3.7f,0.0f });
			SetBulletInterval(m_nCntJumptime + 30);	// 弾の発動間隔を設定する
		}
		if (GetBulletInterval() == m_nCntJumptime&&GetType()==CObject::TYPE::ENEMY)
		{// 弾の発動間隔がカウントを超えたら

			CBullet3D::Create(
				GetType()
				,{ GetPos().x,GetPos().y + (GetSize().y * 0.5f),GetPos().z }
				, { 1.0f * GetDirection(),0.0f,0.0f }
				, 60
			);
		}
		else if (GetBulletInterval() == m_nCntJumptime && GetType() == CObject::TYPE::ENEMY_MESSE)
		{
			if (pSkill->GetInputSkillState() != CSkill::SKILL_PRESS)
			{
				CBullet3D::Create(
					GetType()
					, { GetPos().x,GetPos().y + (GetSize().y * 0.5f),GetPos().z }
					, { 1.0f * GetDirection(),0.0f,0.0f }
					, 60
				);
			}
		}
	}


	CEnemyland3D::Update();

	Attack();
	
	m_nCntJumptime++;
}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CSlime::Draw()
{
	CEnemyland3D::Draw();
}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CSlime* CSlime::Create(D3DXVECTOR3 pos)
{
	CSlime* pSlime = new CSlime;

	pSlime->SetPos(pos);
	pSlime->SetType(ENEMY);

	pSlime->Init();

	return pSlime;
}

//===========================================================================================================
// 攻撃処理
//===========================================================================================================
void CSlime::Attack()
{
	// プレイヤーを探す
	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, CObject::TYPE::PLAYER);

	// Get してきた向きに合わせて正しい値に変換する
	int dire = GetDirection();
	if (dire == -1) { dire = CModel::DIRECTION_LEFT; }
	if (dire ==  1) { dire = CModel::DIRECTION_RIGHT; }

	bool bStartButtle = false;

	if(pPlayer==nullptr)
	{
		return;
	}

	switch (dire)
	{
	case CModel::DIRECTION_RIGHT:

		if (GetPos().x < pPlayer->GetPos().x + (pPlayer->GetSize().x * 0.5f)
			&& GetPos().x + (GetSize().x *3.0f) > pPlayer->GetPos().x - (pPlayer->GetSize().x * 0.5f)
			&& GetPos().y + (GetSize().y * 0.5f) > pPlayer->GetPos().y - (pPlayer->GetSize().y * 0.5f)
			&& GetPos().y - (GetSize().y * 0.5f) < pPlayer->GetPos().y + (pPlayer->GetSize().y * 0.5f)
			)
		{
			bStartButtle = true;
		}

		break;

	case CModel::DIRECTION_LEFT:


		break;
	}

	if (bStartButtle && GetState() != STATE_DAMAGE && GetOldState() != STATE_DAMAGE &&
		GetState() != STATE_SKILL && GetState() != STATE_SWORD && GetState() != STATE_BEFORE_SKILL)	// スキル中は攻撃をしない
	{
		//===========================================================================================================
		// < debugmemo >
		// 攻撃処理が完成したらここの処理実装
		//===========================================================================================================
		//SetState(CEnemy3D::STATE_ATTACK);
	}
	
}