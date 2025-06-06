//===========================================================================================================================================================
// 
// 3Dプレイヤーの処理
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "player3D.h"
#include "block3D.h"
#include "manager.h"
#include "particle3D.h"
#include "fragmentlife.h"
#include "slime_player.h"
#include "slime_enemy.h"
#include "game.h"
#include "search.h"
#include "golem_player.h"
#include "buttleUI.h"
#include "slime_boss.h"
#include "hp_meter.h"
#include "UI_skill.h"
#include "sound.h"

// 静的メンバ変数の初期化
int CPlayer3D::m_nFragmentNum = 0;
CSkill* CPlayer3D::m_pSkill = nullptr;
CSword* CPlayer3D::m_pSword = nullptr;
CButtleUI* CPlayer3D::m_pButtleUI[CButtleUI::BUTTLE_MESSE_MAX] = {};

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CPlayer3D::CPlayer3D(int nPriority) :CModel(nPriority)
, m_Move{ 0.0f,0.0f,0.0f }
, m_fSpeed(0)
, m_oldPos{ 0.0f,0.0f,0.0f }
, m_skillstate(SKILL_NONE)
, m_bUseJump(false)
, m_FrontMessengerTag(CEnemy3D::TAG_ENEMY::TAG_NONE)
, m_NextMessengerTag(CEnemy3D::TAG_ENEMY::TAG_NONE)
, m_PrevMessengerTag(CEnemy3D::TAG_ENEMY::TAG_NONE)
, m_bSlip(false)
, m_nSlipCnt(0)
, m_bDammage(false)
, m_nLife(0)
, m_nCntDamage(0)
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CPlayer3D::~CPlayer3D()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CPlayer3D::Init()
{

	CModel::Init();

	// モデルの設定
	SetModelAddress("data\\model\\player_001.x");		// アドレスを保存しておく
	CModeldata* pModeldata = CManager::GetModeldata();	// modeldata のポインタを持ってくる
	int nIdx = pModeldata->Regist(GetModelAddress());	// モデルデータの登録
	BindModel(pModeldata->GetAddress(nIdx));			// モデル情報をセットする
	SetModelIdx(nIdx);

	for (int nCntTag = 0; nCntTag <= CEnemy3D::TAG_ENEMY::TAG_MAX; nCntTag++)
	{
		m_pMessenger[nCntTag] = nullptr;
	}

	m_nFragmentNum = 0;

	// 剣の生成
	m_pSword = CSword::Create(GetPos());

	// 戦闘UIの設定
	SetButtleUI();

	SetSize({ 10.0f, 20.0f, 15.0f });
	m_fSpeed = 1.0f;

	m_nLife = m_MaxLife;

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CPlayer3D::Uninit()
{
	if (m_pSkill != nullptr)
	{
		delete m_pSkill;
		m_pSkill = nullptr;
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pButtleUI[nCnt] != nullptr)
		{
			m_pButtleUI[nCnt]->Uninit();
		}
	}

	for (int nCntMessenger = 0; nCntMessenger < CEnemy3D::TAG_MAX; nCntMessenger++)
	{
		if (m_pMessenger[nCntMessenger] != nullptr)
		{
			m_pMessenger[nCntMessenger]->Uninit();
		}
	}

	CModel::Uninit();

	//if (m_pSword != nullptr)
	//{
	//	delete m_pSword;
	//	m_pSword = nullptr;
	//}
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CPlayer3D::Update()
{
	D3DXVECTOR3 pos = GetPos();

	// 当たり判定消す(後々関数化
	{
		if (m_nSlipCnt == 0)
		{
			m_bSlip = false;
		}
		if (m_bSlip)
		{
			m_nSlipCnt--;
		}

		DeathPlayer();
	}

	// プレイヤー操作
	InputPosPlayer();

	if (!m_bSlip)
	{
		m_bUseJump = true;

		// Y軸の当たり判定・更新
		pos.y += m_Move.y;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_HIGHLOW);

		// X軸の当たり判定・更新
		pos.x += m_Move.x;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_HORIZON);

		// Z軸の当たり判定・更新
		pos.z += m_Move.z;
		SetPos(pos);
		pos = LookObjectCollision(COLLISION_VERTICAL);
	}

	// カメラ追尾
	SetTargetCamera();

	// 速度徐々に減衰
	m_Move.x += (0.0f - m_Move.x) * 0.5f;
	m_Move.z += (0.0f - m_Move.z) * 0.5f;
	//m_Move.y += (0.0f - m_Move.y) * 0.2f;

	CModel::Update();

	m_pSword->TargetPos({ pos.x,pos.y + 10.0f,pos.z - 50.0f });

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CreateTarget();
	}

	if (m_pSkill != nullptr)
	{
		UpdateSkillState();	// スキル状態のアップデート
	}

	if (m_pSkill != nullptr)
	{
		m_pSkill->Update();
	}

	// テクスチャを設定する
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_FRONT]->SetTex_SelectMessenger(m_FrontMessengerTag);
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_NEXT]->SetTex_SelectMessenger(m_NextMessengerTag);
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_PREV]->SetTex_SelectMessenger(m_PrevMessengerTag);

	MessengerUpdate();

	if (m_bDammage)
	{
		UpdateDamage();
	}

	// 過去の位置を登録
	m_oldPos = pos;
}

//===========================================================================================================
// 使役敵毎の更新
//===========================================================================================================
void CPlayer3D::MessengerUpdate()
{
	if (m_pMessenger[m_FrontMessengerTag] != nullptr)
	{
		m_pMessenger[m_FrontMessengerTag]->Update();
	}

	for (int nCnt = 0; nCnt < CEnemy3D::TAG_MAX; nCnt++)
	{
		if (m_pMessenger[nCnt] != nullptr)
		{
			m_pMessenger[nCnt]->UpdateState();
		}
	}
}

//===========================================================================================================
// 使役敵の前後を設定する
//===========================================================================================================
void CPlayer3D::UpddateMessengerList()
{
	int nPrev = m_FrontMessengerTag - 1;
	int nNext = m_FrontMessengerTag + 1;

	// 戦闘に出ている前の使役タグを設定
	if (m_pMessenger[m_FrontMessengerTag] != nullptr)
	{
		if (m_pMessenger[nPrev] == nullptr)
		{
			if (m_pMessenger[m_FrontMessengerTag]->GetTag() - 1 == CEnemy3D::TAG_NONE)
			{
				nPrev = CEnemy3D::TAG_MAX;
			}
			
			do {
				nPrev--;

				if (nPrev == CEnemy3D::TAG_NONE)
				{
					nPrev = CEnemy3D::TAG_MAX;
				}
			} while (m_pMessenger[nPrev] == nullptr);
		}

		m_PrevMessengerTag = (CEnemy3D::TAG_ENEMY)nPrev;
	}

	// 戦闘に出ている後の使役タグを設定
	if (m_pMessenger[m_FrontMessengerTag] != nullptr)
	{
		if (m_pMessenger[nNext] == nullptr)
		{// 次の使役敵が存在していないとき
			if (m_pMessenger[m_FrontMessengerTag]->GetTag() + 1 == CEnemy3D::TAG_MAX)
			{// 正面に出ているタグの次のタグが最大値と一致したとき
				nNext = CEnemy3D::TAG_NONE;	// 最小値に設定
			}

			do {
				nNext++;	// タグナンバーを進める

				if (nNext == CEnemy3D::TAG_MAX)
				{// 最大値になったとき
					nNext = CEnemy3D::TAG_NONE;	// ループ用に最小値に戻す
				}

			} while (m_pMessenger[nNext] == nullptr);	// nullptr ならループを回す
		}

		m_NextMessengerTag = (CEnemy3D::TAG_ENEMY)nNext;	// 次のタグに設定する
	}

}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CPlayer3D::Draw()
{
	MessengerDraw();

	CModel::Draw();
}

//===========================================================================================================
// 使役敵の描画処理
//===========================================================================================================
void CPlayer3D::MessengerDraw()
{
	if (m_pMessenger[m_FrontMessengerTag] != nullptr && m_pSkill->GetActSword() == false)
	{// 使役敵がいる状態 かつ 剣スキルが使用状態でない場合

		m_pMessenger[m_FrontMessengerTag]->Draw();
	}
}

//========================================================================================================================
// プレイヤーの入力処理
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::InputPosPlayer()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	if (keyboard->GetPress(DIK_A) || joypad->GetPress(CInputJoypad::JOYKEY_LEFT))
	{
		m_Move.x += sinf(-D3DX_PI * 0.5f) * m_fSpeed;
		m_Move.y += cosf(-D3DX_PI * 0.5f) * m_fSpeed;

		SetDirection(CModel::DIRECTION_LEFT);
	}
	if (keyboard->GetPress(DIK_D)||joypad->GetPress(CInputJoypad::JOYKEY_RIGHT))
	{
		m_Move.x += sinf(-D3DX_PI * 0.5f) * -m_fSpeed;
		m_Move.y += cosf(-D3DX_PI * 0.5f) * -m_fSpeed;

		SetDirection(CModel::DIRECTION_RIGHT);
	}
	//if (keyboard->GetPress(DIK_W))
	//{
	//	m_Move.z += sinf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//	m_Move.x += cosf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//}
	//if (keyboard->GetPress(DIK_S))
	//{
	//	m_Move.z -= sinf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//	m_Move.x -= cosf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//}

	// ジャンプ重力処理
	Jump();

	//if (keyboard->GetPress(DIK_C))
	//{
	//	m_Move.y -= m_fSpeed;
	//}
	//if (keyboard->GetPress(DIK_Z))
	//{
	//	m_Move.y += m_fSpeed;
	//}

	//// 画面を揺らす処理
	//if (keyboard->GetTrigger(DIK_F1))
	//{
	//	CManager::GetCamera()->SetShake(120, 5.0f);
	//}

	//// 剣を回転
	//if (keyboard->GetTrigger(DIK_F2))
	//{
	//	m_pSword->TargetRot({ 0.0f,0.0f,2.2f }, 10);
	//}

	if (keyboard->GetTrigger(DIK_RETURN) || joypad->GetTriggerPedal(CInputJoypad::JOYKEY_RIGHT_TRIGGER))
	{
		Attack();
		m_pSword->TargetRot({ 0.0f,0.0f,GetDirection() * 2.2f }, 10);
		//m_pButtleUI[0]->SetBPush(true);
	}

	if (m_FrontMessengerTag != CEnemy3D::TAG_NONE)
	{
		FrontChange();

		UpddateMessengerList();
	}

	return m_Move;
}

//===========================================================================================================
// スキルの状態を更新
//===========================================================================================================
void CPlayer3D::UpdateSkillState()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	m_skillstate = SKILL_NONE;
	if (m_pSkill->GetInputSkillState() != CSkill::SKILL_SWORD)
	{// 剣スキル発動以外スキルをリセットする
		m_pSkill->SetInputSkillState(CSkill::SKILL_NONE);
	}

	std::vector<CEnemy3D*>pEnemy = CSearch::SearchMultiObject(pEnemy, ENEMY_MESSE);

	for (unsigned int nCnt = 0; nCnt < pEnemy.size(); nCnt++)
	{
		if (pEnemy[nCnt] != nullptr
			&& pEnemy[nCnt]->GetStateButtle() != CEnemy3D::BUTTLE_SKILL_THROW
			&& pEnemy[nCnt]->GetStateButtle() != CEnemy3D::BUTTLE_SKILL
			)
		{
			//// 剣スキル発動
			//if (keyboard->GetPress(DIK_LSHIFT) || joypad->GetPressPedal(CInputJoypad::JOYKEY_LEFT_TRIGGER))
			//{
			//	if (keyboard->GetTrigger(DIK_E) || joypad->GetTrigger(CInputJoypad::JOYKEY_Y))
			//	{// shift + E を押したとき

			//		if (m_pSkill->GetPossibleSword())
			//		{
			//			m_pSkill->SetInputSkillState(CSkill::SKILL_SWORD_BEFORE);
			//		}
			//	}
			//}
			
			if (m_pSkill->GetInputSkillState() != CSkill::SKILL_SWORD_BEFORE && m_pSkill->GetInputSkillState() != CSkill::SKILL_SWORD)
			{// 剣スキル使用時は、スキルボタンを押せないようにする
				if (keyboard->GetTrigger(DIK_E) || joypad->GetTrigger(CInputJoypad::JOYKEY_Y))
				{
					m_pSkill->SetInputSkillState(CSkill::SKILL_TRIGGER);
				}
				else if (keyboard->GetRelease(DIK_E) || joypad->GetRelese(CInputJoypad::JOYKEY_Y))
				{
					m_pSkill->SetInputSkillState(CSkill::SKILL_RELEASE);
					PlaySound(SOUND_LABEL_SLIMETHROW_SE);
				}
				else if (keyboard->GetPress(DIK_E) || joypad->GetPress(CInputJoypad::JOYKEY_Y))
				{
					m_pSkill->SetInputSkillState(CSkill::SKILL_PRESS);
					
				}
			}

			//===========================================================================================================
			// < debugmemo >
			// せっかくスキルクラスを作ったのに Update の順番の関係で、ここに書かないといけない事態になっている
			// 見づらいコードになっている。どうにかしたい。
			//===========================================================================================================
			if (m_pSkill->GetSwordDurationtime() <= 0&&m_pSkill->GetActSword())
			{// クールタイムがない + スキルが使用状態にあるとき

				//m_pSkill->SetInputSkillState(CSkill::SKILL_SWORD_CANCEL);	// 剣スキルを使用状態から解除する
				//m_pSkill->ActSword(false);	// スキルを未使用状態にする
			}
		}
	}

	if (m_pSkill->GetInputSkillState() == CSkill::SKILL_SWORD /*m_pSkill->GetActSword()*/)
	{// 剣スキル使用時

		if (keyboard->GetTrigger(DIK_Q))
		{
			m_pSkill->SetInputSkillState(CSkill::SKILL_SWORD_CANCEL);
		}
	}
	else
	{// 剣スキル以外使用時
		if (keyboard->GetTrigger(DIK_Q)|| joypad->GetTriggerPedal(CInputJoypad::JOYKEY_LEFT_TRIGGER))
		{
			//m_skillstate = SKILL_CANCELL;

			m_pSkill->SetInputSkillState(CSkill::SKILL_CANCELL);
		}
	}
}

//========================================================================================================================
// プレイヤーの入力処理
//========================================================================================================================
void CPlayer3D::Jump()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	if (!m_bUseJump)
	{
		if (keyboard->GetTrigger(DIK_SPACE) || joypad->GetTrigger(CInputJoypad::JOYKEY_A))
		{
			m_Move.y += m_fJumpPower;	// ジャンプ
			m_bUseJump = true;
		}
	}

	m_Move.y -= m_fGravity;	// 重力加算
}

//===========================================================================================================
// ダメージ増やす処理
//===========================================================================================================
void CPlayer3D::AddDamage(int nDamage)
{
	if (!m_bDammage)
	{// ダメージ状態ではないとき

		m_nCntDamage = m_MaxInterval;	// ダメージ状態のカウントを設定
		m_nLife -= nDamage;	// ダメージ分ライフを減らす
		m_bDammage = true;	// ダメージ状態にする
	}
}

//===========================================================================================================
// ダメージ状態の更新
//===========================================================================================================
void CPlayer3D::UpdateDamage()
{
	m_nCntDamage--;	// ダメージ状態のカウントを減らす
	bMat(true);

	if (m_nCntDamage <= 0)
	{
		bMat(false);
		m_bDammage = false;
	}
}

//===========================================================================================================
// 死んだときの処理
//===========================================================================================================
void CPlayer3D::DeathPlayer()
{
	if (GetPos().y <= -50.0f || m_nLife <= 0)
	{// 穴に落ちたとき || 体力がなくなったとき
		m_nLife = m_MaxLife;			// 体力を最大値に戻す
		SetPos({ 40.0f, 25.0f, 0.0f });	// リスポーンさせる
		m_bSlip = true;					// すり抜けさせる
		m_nSlipCnt = 5;					// すり抜けさせる時間を設定

		if (m_pMessenger[m_FrontMessengerTag] == nullptr)
		{ return; }

		m_pMessenger[m_FrontMessengerTag]->TargetWarp(GetPos());	// 現在の使役敵を player にワープさせる
	}
}

//===========================================================================================================
// プレイヤーの攻撃処理
//===========================================================================================================
void CPlayer3D::Attack()
{
	//CEnemy3D* pEnemy = nullptr;

	std::vector<CEnemy3D*>pEnemyMulti{};
	std::vector<CSlimeBoss*>pSlimeMulti{};

	pEnemyMulti = CSearch::SearchMultiObject(pEnemyMulti, TYPE::ENEMY);
	pSlimeMulti = CSearch::SearchMultiObject(pSlimeMulti, TYPE::ENEMY_BOSS);

	for (unsigned int nCnt = 0; nCnt < pEnemyMulti.size(); nCnt++)
	{
		D3DXVECTOR3 EnemyPos, EnemySize;

		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 size = GetSize();

		EnemyPos = pEnemyMulti[nCnt]->GetPos();
		EnemySize = pEnemyMulti[nCnt]->GetSize();
		int Direction = GetDirection();

		if (EnemyPos.x - EnemySize.x <= pos.x + size.x + (20.0f * Direction)
			&& EnemyPos.x + EnemySize.x >= pos.x - size.x + (20.0f * Direction)
			&& EnemyPos.y - EnemySize.y <= pos.y + size.y
			&& EnemyPos.y + EnemySize.y >= pos.y - size.y
			)
		{
			if (pEnemyMulti[nCnt]->GetState() != CEnemy3D::STATE_DAMAGE)
			{
				if (m_pSkill != nullptr && m_pSkill->GetInputSkillState() == CSkill::SKILL_SWORD /*m_pSkill->GetActSword()*/)
				{
					pEnemyMulti[nCnt]->Sufferdamage(10);

					if (!pEnemyMulti[nCnt]->GetSufferKnockback())
					{
						SwordAttack(pEnemyMulti[nCnt]);	// 剣スキルの攻撃
					}
				}
				else
				{
					pEnemyMulti[nCnt]->Sufferdamage(15);
				}
			}
			
		}
	}

	for (unsigned int nCnt = 0; nCnt < pSlimeMulti.size(); nCnt++)
	{
		D3DXVECTOR3 EnemyPos, EnemySize;

		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 size = GetSize();

		EnemyPos = pSlimeMulti[nCnt]->GetPos();
		EnemySize = pSlimeMulti[nCnt]->GetSize();

		int Direction = GetDirection();

		if (EnemyPos.x - EnemySize.x <= pos.x + size.x + (20.0f * Direction)
			&& EnemyPos.x + EnemySize.x >= pos.x - size.x + (20.0f * Direction)
			&& EnemyPos.y - EnemySize.y <= pos.y + size.y
			&& EnemyPos.y + EnemySize.y >= pos.y - size.y
			)
		{
			if (pSlimeMulti[nCnt]->GetState() != CEnemy3D::STATE_DAMAGE)
			{
				if (m_pSkill != nullptr && m_pSkill->GetInputSkillState() == CSkill::SKILL_SWORD /*m_pSkill->GetActSword()*/)
				{
					pSlimeMulti[nCnt]->Sufferdamage(10);

					if (!pSlimeMulti[nCnt]->GetSufferKnockback())
					{
						SwordAttack(pSlimeMulti[nCnt]);	// 剣スキルの攻撃
					}
				}
				else
				{
					pSlimeMulti[nCnt]->Sufferdamage(15);
				}
			}

		}
	}
}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CPlayer3D* CPlayer3D::Create(D3DXVECTOR3 pos)
{
	CPlayer3D* pPlayer3D = new CPlayer3D;	// プレイヤーの型で動的確保

	pPlayer3D->SetPos(pos);	// 位置を設定
	pPlayer3D->SetType(TYPE::PLAYER);

	pPlayer3D->Init();	// ３Dの初期設定

	return pPlayer3D;
}

//===========================================================================================================
// 使役敵を生成する
//===========================================================================================================
void CPlayer3D::CreateTarget()
{
	//===========================================================================================================
	// < debugmemo >
	// 下の処理、これは player.cpp でいいのか？
	// 今は player の pos が必要だからここに書いているがそれはクラスとして、そもそも間違っているのでは？
	// そもそも、fragment_manager はこれでいいのか？そこのクラス設計から怪しい。
	//===========================================================================================================

	D3DXVECTOR3 pos = GetPos();

	// fragmentmanagerを持ってくる
	CFragmentManager* pFragMG = CGame::GetFragmentMG();

	for (int nCntTag = 0; nCntTag < CEnemy3D::TAG_MAX; nCntTag++)
	{// 全てのタグで探す

		if (!pFragMG->GetAppear((CEnemy3D::TAG_ENEMY)nCntTag))
		{// この使役敵が使役済みでなければ

			int Fragnum = pFragMG->GetFragmentNum((CEnemy3D::TAG_ENEMY)nCntTag);	// このタグの欠片の数を取得する

			if (Fragnum >= 10)
			{// タグが数分集まったら

				// それぞれのタグでクリエイトする
				switch (nCntTag)
				{
				case CEnemy3D::TAG_SLIME:
					m_pMessenger[nCntTag] = CSlimePlayer::Create({ pos.x,pos.y + 10,pos.z });	// 生成すると同時に player にポインタを渡しておく

					//m_FrontMessengerTag = (CEnemy3D::TAG_ENEMY)nCntTag;	// 生成された使役敵を前に出す

					pFragMG->SetAppear((CEnemy3D::TAG_ENEMY)nCntTag);	// 召喚したというのを示す bool

					break;

				case CEnemy3D::TAG_GOLEM:
					m_pMessenger[nCntTag] = CGolemPlayer::Create({ pos.x,pos.y + 10,pos.z });	// 生成すると同時に player にポインタを渡しておく

					pFragMG->SetAppear((CEnemy3D::TAG_ENEMY)nCntTag);	// 召喚したというのを示す bool

					break;

				case CEnemy3D::TAG_NONE:
					assert(0);
					break;

				default:
					assert(0);
					break;
				}

				if (m_FrontMessengerTag == CEnemy3D::TAG_NONE)
				{
					m_FrontMessengerTag = (CEnemy3D::TAG_ENEMY)nCntTag;	// 生成された使役敵を前に出す
				}

				if (m_pSkill == nullptr)
				{
					m_pSkill = new CSkill;	// スライムスキルを生成する
				}
			}
		}
	}
}

void CPlayer3D::SetButtleUI()
{
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_FRONT] = CButtleUI::Create({ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.9f,0.0f }, { 150.0f,150.0f,0.0f });	// 戦闘UIのポインタを取得
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_NEXT] = CButtleUI::Create({ SCREEN_WIDTH * 0.6f,SCREEN_HEIGHT * 0.9f,0.0f }, { 100.0f,100.0f,0.0f });	// 戦闘UIのポインタを取得
	m_pButtleUI[CButtleUI::BUTTLE_MESSE_PREV] = CButtleUI::Create({ SCREEN_WIDTH * 0.4f,SCREEN_HEIGHT * 0.9f,0.0f }, { 100.0f,100.0f,0.0f });	// 戦闘UIのポインタを取得


	for (int n = 0; n < CUISkill::UI_SKILL_MAX; n++)
	{
		CUISkill::Create((CUISkill::UI_SKILL)n);
	}

	// HP バーの生成
	CHpMeter::Create(
		{ 10.0f,680.0f,0.0f },
		{ 400.0f,20.0f,0.0f },
		CHpMeter::HP_TYPE::HP_LITERAL,
		GetType()
	);

	CHpMeter::Create(
		{ 10.0f,680.0f,0.0f },
		{ 400.0f,20.0f,0.0f },
		CHpMeter::HP_TYPE::HP_VARIABLE,
		GetType()
	);
}

//========================================================================================================================
// playerのposをcameraのtargetにセットする
//========================================================================================================================
void CPlayer3D::SetTargetCamera()
{
	CCamera* pCamera = CManager::GetCamera();	// camera を持ってくる
	pCamera->SetTargetPos(GetPos());			// camera に playerpos を入れる
}

//===========================================================================================================
// 剣スキルの攻撃
//===========================================================================================================
void CPlayer3D::SwordAttack(CEnemy3D* pEnemy)
{
	int Direction = GetDirection();

	// 表に出ている使役敵の種類で判別
	switch (m_FrontMessengerTag)
	{
	case CEnemy3D::TAG_SLIME:

		// ノックバックさせる
		pEnemy->SetMove({
			5.0f * Direction,
			3.0f,
			0.0f
			});

		break;
	}
}

//===========================================================================================================
// 戦闘に出す使役敵を変える
//===========================================================================================================
void CPlayer3D::FrontChange()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();
	int tagnext = m_FrontMessengerTag;
	int tagprev = m_FrontMessengerTag;
	tagnext += 1;
	tagprev -= 1;

	if (keyboard->GetTrigger(DIK_1) || joypad->GetTrigger(CInputJoypad::JOYKEY_RIGHT_SHOULDER))
	{
		while (m_pMessenger[tagprev] == nullptr)
		{
			if (tagprev == CEnemy3D::TAG_NONE)
			{
				tagprev = CEnemy3D::TAG_MAX;
			}

			tagprev--;
		}

		m_FrontMessengerTag = (CEnemy3D::TAG_ENEMY)tagprev;

		if (m_pMessenger[m_FrontMessengerTag]->GetState() != CEnemy3D::STATE_DEAD)
		{
			m_pMessenger[m_FrontMessengerTag]->TargetWarp(GetPos());
		}
	}
	else if (keyboard->GetTrigger(DIK_2) || joypad->GetTrigger(CInputJoypad::JOYKEY_LEFT_SHOULDER))
	{
		while (m_pMessenger[tagnext] == nullptr)
		{
			if (tagnext == CEnemy3D::TAG_MAX)
			{
				tagnext = CEnemy3D::TAG_NONE;
			}

			tagnext++;
		}

		m_FrontMessengerTag = (CEnemy3D::TAG_ENEMY)tagnext;

		if (m_pMessenger[m_FrontMessengerTag]->GetState() != CEnemy3D::STATE_DEAD)
		{
			m_pMessenger[m_FrontMessengerTag]->TargetWarp(GetPos());
		}
	}
}

//========================================================================================================================
// 各オブジェクトを確認する
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::LookObjectCollision(COLLISION_TYPE type)
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

	return pos;
}

//========================================================================================================================
// 縦の当たり判定
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::VerticalCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 PlayerPos = GetPos();
	D3DXVECTOR3 PlayerSize = GetSize();

	PlayerSize.x *= 0.5f;
	PlayerSize.y *= 0.5f;
	PlayerSize.z *= 0.5f;

	if (type == TYPE::BLOCK)
	{
		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		if (BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z		// プレイヤーがブロックにめり込んだ時
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// プレイヤーがブロックの
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// xy範囲内にいたとき
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// -------------------------------------------
			&& m_oldPos.z + PlayerSize.z <= BlockPos.z - BlockSize.z	// プレイヤーの過去の位置がブロックより手前にあったとき
			)
		{// 上に乗る
			m_Move.z = 0;

			SetPos({ PlayerPos.x,PlayerPos.y,PlayerPos.z = (BlockPos.z - BlockSize.z - PlayerSize.z) });
		}

		if (BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z		// プレイヤーがブロックにめり込んだ時
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// プレイヤーがブロックの
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// xy範囲内にいたとき
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// -------------------------------------------
			&& m_oldPos.z - PlayerSize.z >= BlockPos.z + BlockSize.z	// プレイヤーの過去の位置がブロックより奥にあったとき
			)
		{
			m_Move.z = 0;

			SetPos({ PlayerPos.x,PlayerPos.y ,PlayerPos.z = (BlockPos.z + BlockSize.z + PlayerSize.z) });
		}
	}

	return PlayerPos;
}

//========================================================================================================================
// 横の当たり判定
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::HorizonCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 PlayerPos = GetPos();
	D3DXVECTOR3 PlayerSize = GetSize();

	PlayerSize.x *= 0.5f;
	PlayerSize.y *= 0.5f;
	PlayerSize.z *= 0.5f;

	if (type == TYPE::BLOCK)
	{
		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		if (BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x		// プレイヤーがブロックにめり込んだ時
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// -------------------------------------------
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// プレイヤーがブロックの
			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// yz範囲内にいたとき
			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			&& m_oldPos.x - PlayerSize.x >= BlockPos.x + BlockSize.x	// プレイヤーの過去の位置がブロックより右にあったとき
			)
		{
			m_Move.x = 0;

			SetPos({ PlayerPos.x = (BlockPos.x + BlockSize.x + PlayerSize.x),PlayerPos.y,PlayerPos.z });
		}

		else if (BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// プレイヤーがブロックにめり込んだ時
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// -------------------------------------------
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// プレイヤーがブロックの
			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// yz範囲内にいたとき
			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			&& m_oldPos.x + PlayerSize.x <= BlockPos.x - BlockSize.x	// プレイヤーの過去の位置がブロックより左にあったとき
			)
		{
			m_Move.x = 0;

			SetPos({ PlayerPos.x = (BlockPos.x - BlockSize.x - PlayerSize.x),PlayerPos.y,PlayerPos.z });
		}
	}

	return PlayerPos;
}

//========================================================================================================================
// 横の当たり判定
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::HighLowCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 PlayerPos = GetPos();
	D3DXVECTOR3 PlayerSize = GetSize();

	PlayerSize.x *= 0.5f;
	PlayerSize.z *= 0.5f;
	PlayerSize.y *= 0.5f;

	if (type == TYPE::BLOCK)
	{// タイプがブロックだった場合

		CBlock3D* pBlock = (CBlock3D*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		BlockSize.y *= 0.5f;
		BlockSize.z *= 0.5f;

		// プレイヤーがブロックに頭をぶつける
		if (BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y		// プレイヤーがブロックにめり込んだ時
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// プレイヤーがブロックの
			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// xz範囲内にいたとき
			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			&& BlockPos.y - BlockSize.y >= m_oldPos.y + PlayerSize.y	// プレイヤーの過去の位置がブロックより下にあったとき
			)
		{
			m_Move.y = 0;

			SetPos({ PlayerPos.x,PlayerPos.y = (BlockPos.y - BlockSize.y - PlayerSize.y),PlayerPos.z });	// プレイヤーがブロックの下に行く
		}

		// プレイヤーがブロックに乗る
		else if (BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// プレイヤーがブロックにめり込んだ時
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// プレイヤーがブロックの
			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// xz範囲内にいたとき
			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
			&& BlockPos.y + BlockSize.y <= m_oldPos.y - PlayerSize.y	// プレイヤーの過去の位置がブロックより上にあったとき
			)
		{
			m_bUseJump = false;
			m_Move.y = 0;
			SetPos({ PlayerPos.x,PlayerPos.y = (BlockPos.y + BlockSize.y + PlayerSize.y),PlayerPos.z });	// プレイヤーがブロックの上に乗る
		}
	}

	return PlayerPos;
}