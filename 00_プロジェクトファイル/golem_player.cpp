//===========================================================================================================================================================
// 
// 使役ゴーレムの処理 [golem_player.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "golem_player.h"
#include "manager.h"


//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CGolemPlayer::CGolemPlayer()
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CGolemPlayer::~CGolemPlayer()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CGolemPlayer::Init()
{

	CGolem::Init();

	// モデルデータをセット
	SetModelAddress("data\\model\\enemy_golem100.x");	// アドレスを保存しておく
	CModeldata* pModeldata = CManager::GetModeldata();	// modeldata のポインタを持ってくる
	int nIdx = pModeldata->Regist(GetModelAddress());	// モデルデータの登録
	BindModel(pModeldata->GetAddress(nIdx));			// モデル情報をセットする

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CGolemPlayer::Uninit()
{
	CGolem::Uninit();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CGolemPlayer::Update()
{
	CGolem::Update();
}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CGolemPlayer::Draw()
{
	CGolem::Draw();
}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CGolemPlayer* CGolemPlayer::Create(D3DXVECTOR3 pos)
{
	CGolemPlayer* pGolemPlayer = new CGolemPlayer;

	pGolemPlayer->Init();
	pGolemPlayer->SetPos(pos);

	pGolemPlayer->SetType(ENEMY_MESSE);

	return pGolemPlayer;
}