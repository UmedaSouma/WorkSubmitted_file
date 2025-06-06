//===========================================================================================================================================================
// 
// 使役スライムの処理 [slime_player.cpp]
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
// コンストラクタ
//========================================================================================================================
CSlimePlayer::CSlimePlayer():
	m_nSoundCnt(0)
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CSlimePlayer::~CSlimePlayer()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CSlimePlayer::Init()
{
	SetMaxLife(159);

	CSlime::Init();

	// モデルデータをセット
	SetModelAddress("data\\model\\enemy_slime_001.x");		// アドレスを保存しておく
	CModeldata* pModeldata = CManager::GetModeldata();		// modeldata のポインタを持ってくる
	int nIdx = pModeldata->Regist(GetModelAddress());		// モデルデータの登録
	BindModel(pModeldata->GetAddress(nIdx));				// モデル情報をセットする

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CSlimePlayer::Uninit()
{
	CSlime::Uninit();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CSlimePlayer::Update()
{

	if (GetState() != CEnemy3D::STATE_DEAD)
	{
		CPlayer3D* pPlayer = nullptr;

		pPlayer = CSearch::SearchObject(pPlayer, TYPE::PLAYER);

		CSkill* pSkill = CPlayer3D::GetSkill();	// スキルの情報を取得
		CSkill::SKILL_INPUT_STATE pSkillState = pSkill->GetInputSkillState();	// スキルの状態を取得する

		STATE_ENEMY State_ = STATE_NORMAL;	// 敵の状態
		STATE_BUTTLE State_B = BUTTLE_NONE;	// 戦闘状態

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

			//SetState(STATE_NORMAL);	// スキル状態にする

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

			SetState(STATE_BEFORE_SKILL);	// スキル状態にする

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

			pSkill->SetInputSkillState(CSkill::SKILL_SWORD);	// 剣スキル継続状態にする
			pSkill->SetSwordDurationtime(600);					// スキル継続時間設定

			break;

		case CSkill::SKILL_INPUT_STATE::SKILL_SWORD:

			SetState(STATE_SWORD);

			break;
		case CSkill::SKILL_INPUT_STATE::SKILL_SWORD_CANCEL:

			SetState(STATE_CREATE);	// 当たり判定をなくす

			SetPos({
				pPlayer->GetPos().x,
				pPlayer->GetPos().y + 5.0f,
				pPlayer->GetPos().z
				});

			pSkill->SetSwordCooltime(720);	// クールタイムを設定する

			CSlime::Update();	// 当たり判定をなくした状態で Update を通り Player の上に持ってくる

			SetState(STATE_NORMAL);	// state を戻す

			break;
		}
	}

	m_nSoundCnt--;

	CSlime::Update();
}


//========================================================================================================================
// 描画処理
//========================================================================================================================
void CSlimePlayer::Draw()
{
	//if (GetState() != STATE_SWORD)
	{
		CSlime::Draw();
	}
}

//========================================================================================================================
// 生成処理
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
// スキルを使っているときのアップデート
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

		// プレイヤーがブロックに乗る
		if (pos.y + size.y > PlayerPos.y - PlayerSize.y*0.5f		// プレイヤーがブロックにめり込んだ時
			&& pos.x - size.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& pos.x + size.x > PlayerPos.x - PlayerSize.x	// プレイヤーがブロックの
			&& pos.z - size.z < PlayerPos.z + PlayerSize.z	// xz 範囲内にいたとき
			&& pos.y - size.y < PlayerPos.y + PlayerSize.y	// y の
			&& pos.z + size.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			//&& pos.y + size.y <= PlayerOldPos.y - PlayerSize.y	// プレイヤーの過去の位置がブロックより上にあったとき
			)
		{
			if (m_nSoundCnt <= 0)
			{
				PlaySound(SOUND_LABEL_SLIMEJUMP_SE);
				m_nSoundCnt = 30;
			}
			
			pPlayer->SetbUseJump(true);	// ジャンプした判定にする
			pPlayer->SetMove({ PlayerMove.x+0.0f,10.0f,PlayerMove.z*0.0f });	// プレイヤーを上に移動させる
		}
	}
}