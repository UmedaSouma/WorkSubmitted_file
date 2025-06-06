//================================================================================================================
// 
// 弾の処理
// Aouther:umeda souma
// 
//================================================================================================================
#include "bullet.h"
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include "enemy.h"

LPDIRECT3DTEXTURE9 CBullet::m_pTexture = nullptr;

//======================================================================
// コンストラクタ
//======================================================================
CBullet::CBullet(int nPriority) :CObject2D(nPriority), m_fSpeed(0), m_nLife(0), m_bIsEnemyCollision(false)
{
	m_nPriority = nPriority;
}

//======================================================================
// デストラクタ
//======================================================================
CBullet::~CBullet()
{
}

//======================================================================
// 初期設定
//======================================================================
HRESULT CBullet::Init()
{
	// サイズの値を設定
	float bulletsize = 30.0f;

	m_fSpeed = 10.0f;	// スピード設定
	m_nLife = MAX_LIFE;		// 寿命設定
	m_nTexSplit = { 1,1 };	// テクスチャ分割設定

	// サイズを設定
	SetSize({ bulletsize,bulletsize,bulletsize });

	CObject2D::Init();

	return S_OK;
}

//======================================================================
// 終了処理
//======================================================================
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//======================================================================
// 更新処理
//======================================================================
void CBullet::Update()
{
	D3DXVECTOR3 pos = GetPos();	// 位置情報取得

	// 全てのオブジェクトを確認する
	LookObjectALL();

	// pos.x += m_fSpeed;	// 速さ分位置をずらす
	pos.x += m_fSpeed;	// 速さ分位置をずらす
	SetPos(pos);	// 位置を更新させる

	// 弾が寿命を迎えたとき
	if (m_nLife <= 0) 
	{ 
		CExplosion::Create(pos, { 3,3 });	// 爆発を生成

		// 解放する
		Uninit();
		//Release();
		return;
	}

	CObject2D::Update();

	m_nLife--;
}

//======================================================================
// 描画処理
//======================================================================
void CBullet::Draw()
{
	CObject2D::Draw();
}

//======================================================================
// 生成処理
//======================================================================
CBullet* CBullet::Create(D3DXVECTOR3 pos)
{
	// 動的確保
	CBullet* pBullet = new CBullet;

	pBullet->SetPos(pos);	// 位置設定
	pBullet->BindTexture(m_pTexture);	// テクスチャセット
	pBullet->Init();	// 初期設定

	return pBullet;	// 弾の情報を返す
}

//======================================================================
// 全てのオブジェクト確認->当たり判定
//======================================================================
void CBullet::LookObjectALL()
{
	// オブジェクトを全て確認
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		// オブジェクトのポインタに代入
		CObject* pObj = GetOBJECT(PRIORITY_DEFAULT, nCntObj);

		if (pObj != nullptr)
		{
			// 当たり判定
			m_bIsEnemyCollision = CollisionEnemy(pObj);
		}
	}
}

//======================================================================
// 敵との当たり判定
//======================================================================
bool CBullet::CollisionEnemy(CObject* pObj)
{
	// 中身が入っているオブジェクトのタイプを確認
	CObject::TYPE type = pObj->GetType();

	// Bulletの情報を持ってくる
	D3DXVECTOR3 BulletPos = GetPos();	// pos
	D3DXVECTOR3 BulletSize = GetSize(); // size

	// サイズを半分に設定しておく
	BulletSize.x *= 0.5f;
	BulletSize.y *= 0.5f;

	// 取得したオブジェクトがエネミーだった場合
	if (type == TYPE::ENEMY)
	{
		// オブジェクトをエネミーにダウンキャスト
		CEnemy* pEnemy = (CEnemy*)pObj;

		// enemyの情報を取得
		D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
		D3DXVECTOR3 EnemySize = pEnemy->GetSize();

		// サイズを半分に設定
		EnemySize.x *= 0.5f;
		EnemySize.y *= 0.5f;

		if (EnemyPos.x-EnemySize.x<=BulletPos.x+BulletSize.x
			&&EnemyPos.x+EnemySize.x>=BulletPos.x-BulletSize.x
			&&EnemyPos.y+EnemySize.y>=BulletPos.y-BulletSize.y
			&&EnemyPos.y-EnemySize.y<=BulletPos.y+BulletSize.y
			)
		{// 範囲内で重なったときに	
			m_nLife = 0;	// 弾の寿命をなくす

			return true;
		}
	}

	return false;
}

//========================================================================================================================
// テクスチャ読み込み
//========================================================================================================================
HRESULT CBullet::TextureLoad()
{
	if (m_pTexture == nullptr)
	{
		CRenderer* Renderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ElementWind.png", &m_pTexture);
	}

	return S_OK; 
}

//========================================================================================================================
// テクスチャ破棄
//========================================================================================================================
void CBullet::TextureUnLoad()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}