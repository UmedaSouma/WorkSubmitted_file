//===========================================================================================================================================================
// 
// 弾の処理 [bullet3D.cpp]
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
// コンストラクタ
//========================================================================================================================
CBullet3D::CBullet3D() :
	m_nLife(0)
	, m_move({ 0.0f,0.0f,0.0f })
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CBullet3D::~CBullet3D()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CBullet3D::Init()
{
	SetSize({ 5.0f,5.0f,0.0f });

	PlaySound(SOUND_LABEL_SHOT_SE);

	CModel::Init();

	// モデルデータをセット
	CModeldata* pModeldata = CManager::GetModeldata();	// modeldata のポインタを持ってくる

	SetModelAddress("data\\model\\attack_bullet_000.x");	// アドレスを保存しておく
	int nIdx = pModeldata->Regist(GetModelAddress());	// モデルデータの登録
	BindModel(pModeldata->GetAddress(nIdx));		 	// モデル情報をセットする

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CBullet3D::Uninit()
{
	CModel::Uninit();
}

//========================================================================================================================
// 更新処理
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
// 描画処理
//========================================================================================================================
void CBullet3D::Draw()
{
	CModel::Draw();
}

//========================================================================================================================
// 生成処理
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
// 種類ごとの更新
//===========================================================================================================
void CBullet3D::UpdateType()
{
	// オブジェクトのタイプごとに更新を変える
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
// 味方の更新
//===========================================================================================================
void CBullet3D::UpdateAlly()
{
	D3DXVECTOR3 targetpos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 targetsize = { 0.0f,0.0f,0.0f };

	int nDamage = 0;

	std::vector<CEnemy3D*>pEnemy;
	pEnemy = CSearch::SearchMultiObject(pEnemy, CObject::ENEMY);

	for (unsigned int nCntMesse = 0; nCntMesse < pEnemy.size(); nCntMesse++)
	{// 今存在する敵の数回す
		if (pEnemy[nCntMesse] != nullptr)
		{// nullptr じゃなかったら
			targetpos = pEnemy[nCntMesse]->GetPos();	// ターゲットの pos 取得
			targetsize = pEnemy[nCntMesse]->GetSize();	// ターゲットの size 取得

			targetsize.x *= 0.5f;

			nDamage = Collision(targetpos, targetsize);	// 当たり判定：引数で与えるダメージをとってくる

			if (nDamage != 0)
			{// ターゲットがダメージを与えられたらこの下の処理を通さない

				pEnemy[nCntMesse]->Sufferdamage(nDamage);	// 引数で持ってきたダメージを与える
				return;
			}
		}
	}

	std::vector<CEnemy3D*>pEnemyBoss;
	pEnemyBoss = CSearch::SearchMultiObject(pEnemyBoss, CObject::ENEMY_BOSS);

	for (unsigned int nCntBoss = 0; nCntBoss < pEnemyBoss.size(); nCntBoss++)
	{// 今存在するボスの数回す
		if (pEnemyBoss[nCntBoss] != nullptr)
		{// nullptr じゃなかったら
			targetpos = pEnemyBoss[nCntBoss]->GetPos();		// ターゲットの pos 取得
			targetsize = pEnemyBoss[nCntBoss]->GetSize();	// ターゲットの size 取得);

			targetsize.x *= 0.5f;

			nDamage = Collision(targetpos, targetsize);		// 当たり判定：引数で与えるダメージをとってくる

			if (nDamage != 0)
			{// ターゲットがダメージを与えられたらこの下の処理を通さない

				pEnemyBoss[nCntBoss]->Sufferdamage(nDamage);	// 引数で持ってきたダメージを与える;
				return;
			}
		}
	}
}

//===========================================================================================================
// 敵の更新
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
		{// ターゲットがダメージを与えられたらこの下の処理を通さない

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
				{// ターゲットがダメージを与えられたらこの下の処理を通さない

					pEnemy[nCntMesse]->Sufferdamage(nDamage);
					return;
				}
			}
		}
	}

}

//===========================================================================================================
// 当たり判定
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