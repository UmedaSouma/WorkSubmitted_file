//===========================================================================================================================================================
// 
// ボスビームの処理 [attack_beam.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "attack_beam.h"
#include "manager.h"
#include "player3D.h"
#include "search.h"

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CAttackBeam::CAttackBeam() :
	m_Move({ 0.0f,0.0f,0.0f })
	, m_nLife(0)
	, m_bReturn(false)

{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CAttackBeam::~CAttackBeam()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CAttackBeam::Init()
{
	m_Move = { -3.5f,0.0f,0.0f };
	SetSize({ 1.0f,10.0f,0.0f });

	CModel::Init();

	// モデルデータをセット
	SetModelAddress("data\\model\\attack_beam_000.x");	// アドレスを保存しておく
	CModeldata* pModeldata = CManager::GetModeldata();	// modeldata のポインタを持ってくる
	int nIdx = pModeldata->Regist(GetModelAddress());	// モデルデータの登録
	BindModel(pModeldata->GetAddress(nIdx));		 	// モデル情報をセットする

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CAttackBeam::Uninit()
{
	CModel::Uninit();
}

//========================================================================================================================
// 更新処理
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
// 描画処理
//========================================================================================================================
void CAttackBeam::Draw()
{
	CModel::Draw();
}

//===========================================================================================================
// 当たり判定
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
// 生成処理
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
