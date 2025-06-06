//===========================================================================================================================================================
// 
// 背景の管理
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "bg.h"
#include "manager.h"

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CBg::CBg(int nPriorty) :CObject(nPriorty)
{

}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CBg::~CBg()
{

}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CBg::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_OBJECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	// テクスチャの読み込み

	VERTEX_2D* pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
	m_size = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
	m_nTexSplit = { 1.0f,1.0f };

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\minecraft_skyTexture.png", &m_pVtxTexture);

	// 座標の設定
	{
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
	}

	{
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	}

	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / m_nTexSplit.x, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / m_nTexSplit.y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / m_nTexSplit.x, 1.0f / m_nTexSplit.y);
	}

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CBg::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// テクスチャの破棄
	if (m_pVtxTexture != nullptr)
	{
		m_pVtxTexture->Release();
		m_pVtxTexture = nullptr;
	}
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CBg::Update()
{

}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CBg::Draw()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pVtxTexture);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CBg* CBg::Create()
{
	CBg* pBG = new CBg;

	pBG->Init();

	return pBG;
}