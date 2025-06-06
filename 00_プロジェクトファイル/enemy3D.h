//===========================================================================================================================================================
// 
// enemy3D.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ENEMY3D_H_
#define _ENEMY3D_H_

#include "main.h"
#include "model.h"
//===========================================================================================================
// クラス
//===========================================================================================================
class CEnemy3D :public CModel
{
public:

	// 敵ごとのタグ
	typedef enum
	{
		TAG_NONE = 0, 
		TAG_SLIME,	// スライム
		TAG_GOLEM,	// ゴーレム
		TAG_MAX
	}TAG_ENEMY;

	// 当たり判定の場所
	typedef enum
	{
		COLLISION_HORIZON = 0,
		COLLISION_VERTICAL,
		COLLISION_HIGHLOW,
		COLLISION_MAX
	}COLLISION_TYPE;

	// 戦闘時の状態
	typedef enum
	{
		BUTTLE_NONE = 0,	// 何もない状態
		BUTTLE_DISTANCE,	// 攻撃前の距離をとる状態
		BUTTLE_ATTACK,		// 攻撃時
		BUTTLE_SKILL_THROW,	// スキル投げ時
		BUTTLE_SKILL,		// 使役時のスキル使用状態
		BUTTLE_MAX
	}STATE_BUTTLE;

	//===========================================================================================================
	// < debugmemo >
	// state の管理をしっかりする
	// 下の state は敵の全体的な状態を管理したい
	// 使役した敵の行動もこっちで管理させる
	//===========================================================================================================

	// 敵自体の状態
	typedef enum
	{
		STATE_NORMAL = 0,	// 何もない状態
		STATE_MOVE,			// 移動状態
		STATE_ATTACK,		// 攻撃状態
		STATE_BEFORE_SKILL,	// スキル使用前
		STATE_SKILL,		// スキル使用時
		STATE_SWORD,		// 剣スキル使用時
		STATE_DAMAGE,		// ダメージを受けた状態
		STATE_DEAD,			// 死んだ状態
		STATE_CREATE,		// 生成された瞬間
		STATE_MAX
	}STATE_ENEMY;

	CEnemy3D();				// コンストラクタ
	~CEnemy3D()override;			// デストラクタ
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画
	void SetTarget(D3DXVECTOR3 pos);			// ターゲット位置を設定
	void SetState(STATE_ENEMY state);			// 状態を設定
	void TargetPlayer();						// プレイヤーにターゲットを向ける
	void CreateCountDown();						// 生成された時のカウント
	void UpdateState();
	void UpdateSkillSwordDamage();	// 剣スキルの攻撃を受けた場合の更新処理

	D3DXVECTOR3 LookObjectCollision(COLLISION_TYPE type);
	D3DXVECTOR3 VerticalCollision(CObject* pObj);	// 縦の当たり判定
	D3DXVECTOR3 HorizonCollision(CObject* pObj);	// 横の当たり判定
	D3DXVECTOR3 HighLowCollision(CObject* pObj);	// 上下の当たり判定
	void Sufferdamage(int damage);	// 被ダメージ処理

	void TargetWarp(D3DXVECTOR3 playerpos);

	static CEnemy3D* Create(D3DXVECTOR3 pos);	// プレイヤーの生成
private:
	static inline const float MAX_SIZE = 20.0f;	// プレイヤーのサイズ
	static const unsigned int MAX_CREATECOUNT = 3;
	D3DXVECTOR3 m_TargetPos;	// 目標の位置
	int m_nMaxLife;				// 体力最大値
	int m_fLife;				// 体力
	float m_fSpeed;				// 速度
	float m_fPower;				// 攻撃力
	int m_fAtkInterval;			// 攻撃間隔(フレーム数)
	STATE_BUTTLE m_StateButtle;	// 戦闘態勢状態
	STATE_ENEMY m_State;		// 敵の状態
	STATE_ENEMY m_OldState;		// 1フレーム前の敵の状態
	bool m_bBattle;				// 戦闘状態か否か(ture だったら戦闘状態)
	float m_Follow;				// 遅れてターゲットに追いついてくる係数
	float m_fDistance;			// ターゲットとの距離
	TAG_ENEMY m_tag;			// 敵の種類
	static int m_nFragmentNum;	// 欠片の数
	bool m_bIsSkill;	// スキル使用有無
	int m_nCreateCnt;	// 生成された時に
	int m_nCntDamage;	// ダメージ状態のカウント
	bool m_bSufferKnockback;	// true -> ノックバックを受ける
	unsigned int m_nBulletInterval;	// 弾の発動間隔
	int m_nRespawnCnt;	// リスポーンするまでの時間をカウント

	D3DXVECTOR3 m_Move;	// 移動処理
	D3DXVECTOR3 m_oldPos;	// 1フレーム前のpos

public:
	void SetLife(int Life) { m_fLife = Life; }
	int GetLife() { return m_fLife; }

	void SetMaxLife(int MaxLife) { m_nMaxLife = MaxLife;}	// 体力最大値を設定する
	int GetMaxLife() { return m_nMaxLife; }						// 体力最大値を取得する

	void SetStateEnemy(STATE_ENEMY state) { m_State = state; }
	STATE_ENEMY &GetState() { return m_State; }

	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	D3DXVECTOR3 GetMove() { return m_Move; }

	void SetTargetpos(D3DXVECTOR3 targetpos) { m_TargetPos = targetpos; }
	D3DXVECTOR3 GetTargetpos() { return m_TargetPos; }

	void SetTag(TAG_ENEMY tag) { m_tag = tag; }
	TAG_ENEMY GetTag() { return m_tag; }

	static void SetFragmentNum(int num) { m_nFragmentNum = num; }
	static int GetFragmentNum() { return m_nFragmentNum; }

	void SetStateButtle(const STATE_BUTTLE state) { m_StateButtle = state; }
	STATE_BUTTLE& GetStateButtle() { return m_StateButtle; }

	void SetSufferKnockback(bool bSuffer) { m_bSufferKnockback = bSuffer; }
	bool GetSufferKnockback() { return m_bSufferKnockback; }

	void SetBulletInterval(unsigned int interval) { m_nBulletInterval = interval; }
	int GetBulletInterval() { return m_nBulletInterval; }

	int GetRespawnCnt() { return m_nRespawnCnt; }

	STATE_ENEMY &GetOldState() { return m_OldState; }

	static CEnemy3D* enemyget();	// デバック用のエネミー取得
};

#endif // !_ENEMY3D_H_
