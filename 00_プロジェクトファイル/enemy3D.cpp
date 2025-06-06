//==============================================================================================================================================
// 
// 3Dモデルの敵管理
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
// コンストラクタ
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
// デストラクタ
//===========================================================================================================
CEnemy3D::~CEnemy3D()
{
}

//===========================================================================================================
// 初期設定
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
// 終了処理
//===========================================================================================================
void CEnemy3D::Uninit()
{
	CModel::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void CEnemy3D::Update()
{
	m_OldState = m_State;

	D3DXVECTOR3 pos = GetPos();

	TargetPlayer();	// プレイヤーの pos をターゲットにする
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
		// Y軸の当たり判定・更新
		pos.y += m_Move.y;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_HIGHLOW);

		// ターゲットに遅れてついてくる	
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

		// X軸の当たり判定・更新
		pos.x += m_Move.x;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_HORIZON);

		// Z軸の当たり判定・更新
		pos.z += m_Move.z;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_VERTICAL);

		// 速度徐々に減衰
		m_Move.x += (0.0f - m_Move.x) * 0.05f;
		m_Move.z += (0.0f - m_Move.z) * 0.1f;
		//m_Move.y += (0.0f - m_Move.y) * 0.2f;

		//pos.y += (m_TargetPos.y - pos.y) * m_Follow;

		if (m_fLife <= 0 && GetType() != TYPE::ENEMY_MESSE || GetPos().y <= -50.0f && GetType() != TYPE::ENEMY_MESSE)
		{
			CFlagmentLife::SetNumParticle({pos.x,pos.y+GetSize().y*0.5f,pos.z}, 30, 10, m_tag);	// 命の欠片を出す

			Uninit();	// 敵を消す
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

	// 過去の位置を登録
	m_oldPos = pos;	
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void CEnemy3D::Draw()
{
	if (m_State != STATE_DEAD)
	{
		CModel::Draw();
	}
}

//===========================================================================================================
// 生成処理
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
// ターゲット位置の設定
//===========================================================================================================
void CEnemy3D::SetTarget(D3DXVECTOR3 pos)
{
	m_TargetPos = pos;
}

//===========================================================================================================
// 状態の設定
//===========================================================================================================
void CEnemy3D::SetState(STATE_ENEMY state)
{
	m_State = state;

	if (state == STATE_CREATE) { m_nCreateCnt = MAX_CREATECOUNT; }
}

//===========================================================================================================
// 状態の設定
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
// プレイヤーをターゲットに設定
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
// STATE_CREATE時のフレームカウント
//===========================================================================================================
void CEnemy3D::CreateCountDown()
{
	if (m_State == STATE_CREATE)
	{ 
		m_nCreateCnt--; 	// カウントを減らす

		if (m_nCreateCnt <= 0)
		{
			m_State = STATE_NORMAL;	// 敵を普通の状態に戻す
		}
	}

	
}

//========================================================================================================================
// 各オブジェクトを確認する
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
				// 縦の当たり判定(z軸)
			case COLLISION_VERTICAL:
				pos = VerticalCollision(pObj);
				break;

				// 横の当たり判定(x軸)
			case COLLISION_HORIZON:
				pos = HorizonCollision(pObj);
				break;

				// 上下の当たり判定(y軸)
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
	

	//// 各オブジェクトを確認する
	//for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	//{
	//	CObject* pObj = GetOBJECT(PRIORITY_DEFAULT, nCntObj);

	//	if (pObj != nullptr)
	//	{// nullptrでなかったら各軸の当たり判定をする
	//		switch (type)
	//		{
	//			// 縦の当たり判定(z軸)
	//		case COLLISION_VERTICAL:
	//			pos = VerticalCollision(pObj);
	//			break;

	//			// 横の当たり判定(x軸)
	//		case COLLISION_HORIZON:
	//			pos = HorizonCollision(pObj);
	//			break;

	//			// 上下の当たり判定(y軸)
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
// 縦の当たり判定
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

		if (BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z		// プレイヤーがブロックにめり込んだ時
			&& BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x	// プレイヤーがブロックの
			&& BlockPos.y /*- BlockSize.y*/ < EnemyPos.y + EnemySize.y*2	// xy範囲内にいたとき
			&& BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// -------------------------------------------
			&& m_oldPos.z + EnemySize.z <= BlockPos.z - BlockSize.z	// プレイヤーの過去の位置がブロックより手前にあったとき
			)
		{// 上に乗る
			m_Move.z = 0;

			SetPos({ EnemyPos.x,EnemyPos.y,EnemyPos.z = (BlockPos.z - BlockSize.z - EnemySize.z) });
		}

		if (BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z		// プレイヤーがブロックにめり込んだ時
			&& BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x	// プレイヤーがブロックの
			&& BlockPos.y/* - BlockSize.y*/ < EnemyPos.y + EnemySize.y*2	// xy範囲内にいたとき
			&& BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// -------------------------------------------
			&& m_oldPos.z - EnemySize.z >= BlockPos.z + BlockSize.z	// プレイヤーの過去の位置がブロックより奥にあったとき
			)
		{
			m_Move.z = 0;

			SetPos({ EnemyPos.x,EnemyPos.y ,EnemyPos.z = (BlockPos.z + BlockSize.z + EnemySize.z) });
		}
	}

	return EnemyPos;
}

//========================================================================================================================
// 横の当たり判定
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

		if (BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x		// プレイヤーがブロックにめり込んだ時
			&& BlockPos.y /*- BlockSize.y*/ < EnemyPos.y + EnemySize.y*2	// -------------------------------------------
			&& BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// プレイヤーがブロックの
			&& BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z	// yz範囲内にいたとき
			&& BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z	// -------------------------------------------
			&& m_oldPos.x - EnemySize.x >= BlockPos.x + BlockSize.x	// プレイヤーの過去の位置がブロックより右にあったとき
			)
		{
			//m_Move.x = 0;

			SetPos({ EnemyPos.x = (BlockPos.x + BlockSize.x + EnemySize.x),EnemyPos.y,EnemyPos.z });
		}

		else if (BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// プレイヤーがブロックにめり込んだ時
			&& BlockPos.y /*- BlockSize.y*/ < EnemyPos.y + EnemySize.y*2	// -------------------------------------------
			&& BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// プレイヤーがブロックの
			&& BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z	// yz範囲内にいたとき
			&& BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z	// -------------------------------------------
			&& m_oldPos.x + EnemySize.x <= BlockPos.x - BlockSize.x	// プレイヤーの過去の位置がブロックより左にあったとき
			)
		{
			//m_Move.x = 0;

			SetPos({ EnemyPos.x = (BlockPos.x - BlockSize.x - EnemySize.x),EnemyPos.y,EnemyPos.z });
		}
	}

	return EnemyPos;
}

//========================================================================================================================
// 横の当たり判定
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
	{// タイプがブロックだった場合

		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		//BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		// プレイヤーがブロックに頭をぶつける
		if (BlockPos.y /*- BlockSize.y*/ < EnemyPos.y + EnemySize.y*2		// プレイヤーがブロックにめり込んだ時
			&& BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x	// プレイヤーがブロックの
			&& BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z	// xz範囲内にいたとき
			&& BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z	// -------------------------------------------
			&& BlockPos.y /*- BlockSize.y*/ >= m_oldPos.y + EnemySize.y*2	// プレイヤーの過去の位置がブロックより下にあったとき
			)
		{
			m_Move.y = 0;

			//if (GetType() == TYPE::ENEMY_MESSE) { m_Move.x = 0; }

			SetPos({ EnemyPos.x,EnemyPos.y = (BlockPos.y /*- BlockSize.y*/ - EnemySize.y*2),EnemyPos.z });	// プレイヤーがブロックの下に行く
		}

		// プレイヤーがブロックに乗る
		else if (BlockPos.y + BlockSize.y > EnemyPos.y /*- EnemySize.y*/	// プレイヤーがブロックにめり込んだ時
			&& BlockPos.x - BlockSize.x < EnemyPos.x + EnemySize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > EnemyPos.x - EnemySize.x	// プレイヤーがブロックの
			&& BlockPos.z - BlockSize.z < EnemyPos.z + EnemySize.z	// xz範囲内にいたとき
			&& BlockPos.z + BlockSize.z > EnemyPos.z - EnemySize.z	// -------------------------------------------
			&& BlockPos.y + BlockSize.y <= m_oldPos.y /*- EnemySize.y*/	// プレイヤーの過去の位置がブロックより上にあったとき
			)
		{
			m_Move.y = 0;

			// スライムが地面に乗ったとき、横の移動量を消してスライムっぽい動きを作る。
			if (GetTag() == TAG_SLIME) 
			{
				m_Move.x = 0.0f; 
			}

			if (GetType() == TYPE::ENEMY_MESSE && GetTag() == TAG_SLIME &&  /*m_StateButtle == BUTTLE_SKILL_THROW*/ m_State == STATE_BEFORE_SKILL) 
			{ 
				/*m_StateButtle = BUTTLE_SKILL;*/ m_State = STATE_SKILL;	// スライムはスキル準備の後、スキル状態になる
			}	

			SetPos({ EnemyPos.x,EnemyPos.y = (BlockPos.y + BlockSize.y /*+ EnemySize.y*/),EnemyPos.z });	// プレイヤーがブロックの上に乗る
		}
	}

	return EnemyPos;
}

//===========================================================================================================
// 敵の取得
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
// ターゲットにワープする
//===========================================================================================================
void CEnemy3D::TargetWarp(D3DXVECTOR3 playerpos)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();

	{// プレイヤーとの距離が離れたとき

		CPlayer3D* pPlayer = nullptr;
		pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);

		m_State = STATE_CREATE;	// 当たり判定をなくす
		m_nCreateCnt = 3;	// なくしているフレーム
		SetPos({ playerpos.x,playerpos.y+pPlayer->GetSize().y,playerpos.z });	// プレイヤーまで移動する
		SetStateButtle(BUTTLE_NONE);
	}
}

//===========================================================================================================
// ダメージを受けたときの処理
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