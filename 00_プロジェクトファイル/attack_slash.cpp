//===========================================================================================================================================================
// 
// スラッシュ攻撃 [attack_slash.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "attack_slash.h"
#include "manager.h"
#include "texture.h"
#include "player3D.h"
#include "search.h"

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CAttackSlash::CAttackSlash():
	m_Move({ 0.0f,0.0f,0.0f })
	, m_nLife(0)
	, m_bReturn(false)
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CAttackSlash::~CAttackSlash()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CAttackSlash::Init()
{
	int nTexIdx = 0;
	m_Move = { 2.0f,0.0f,0.0f };

	CObject3D::Init();

	CTexture* pTex = CManager::GetTexture();

	if (m_bReturn)
	{
		nTexIdx = pTex->Regist("data\\TEXTURE\\effect_Slash_003.png");
	}
	else
	{
		nTexIdx = pTex->Regist("data\\TEXTURE\\effect_Slash_004.png");
	}
	
	BindTexture(pTex->GetAddress(nTexIdx));

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CAttackSlash::Uninit()
{
	CObject3D::Uninit();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CAttackSlash::Update()
{
	m_nLife--;

	D3DXVECTOR3 pos_Slash = GetPos();
	D3DXVECTOR3 move = m_Move;

	// 反転する
	if (m_bReturn)
	{	move *= -1;	}
	else
	{	move *= 1;	}

	pos_Slash += move;
	SetPos(pos_Slash);

	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}

	Collision();

	CObject3D::Update();
}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CAttackSlash::Draw()
{
	CObject3D::Draw();
}

//===========================================================================================================
// 当たり判定
//===========================================================================================================
void CAttackSlash::Collision()
{
	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);

	D3DXVECTOR3 pos_Player = pPlayer->GetPos();
	D3DXVECTOR3 size_Player = pPlayer->GetSize() * 0.5f;

	D3DXVECTOR3 pos_Slash = GetPos();
	D3DXVECTOR3 size_Slash = GetSize();

	if (m_bReturn)
	{
		pos_Slash.x -= size_Slash.x;
		size_Slash.x = 10.0f;
		pos_Slash.x += size_Slash.x;
	}
	else
	{
		pos_Slash.x += size_Slash.x;
		size_Slash.x = 10.0f;
		pos_Slash.x -= size_Slash.x;
	}

	if (pos_Player.x - size_Player.x <= pos_Slash.x + size_Slash.x
		&& pos_Player.x + size_Player.x >= pos_Slash.x - size_Slash.x
		&& pos_Player.y - size_Player.y <= pos_Slash.y + size_Slash.y
		&& pos_Player.y + size_Player.y >= pos_Slash.y - size_Slash.y)
	{
		Knockback();	// ノックバック処理

		pPlayer->AddDamage(m_SlashDamage);
		Uninit();
		return;
	}
}

//===========================================================================================================
// ノックバック処理
//===========================================================================================================
void CAttackSlash::Knockback()
{
	CPlayer3D* pPlayer = nullptr;
	pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);
	int Dire = 0;

	if (pPlayer != nullptr)
	{
		if (m_bReturn)
		{
			Dire = -1;
		}
		else
		{
			Dire = 1;
		}

		pPlayer->SetMove({
			20.0f * Dire,
			5.0f,
			0.0f
			});

	}

}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CAttackSlash* CAttackSlash::Create(D3DXVECTOR3 pos, int life, bool breturn)
{
	CAttackSlash* pAttackSlash = new CAttackSlash;

	pAttackSlash->SetSize({ 50.0f,10.0f,10.0f });
	pAttackSlash->m_bReturn = breturn;

	pAttackSlash->SetRot({ D3DX_PI * 0.5f,0.0f,0.0f });

	pAttackSlash->SetPos({ pos.x,(pos.y + pAttackSlash->GetSize().y),pos.z });
	pAttackSlash->m_nLife = life;

	pAttackSlash->Init();

	return pAttackSlash;
}
