#include "explosion.h"

LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = nullptr;

CExplosion::CExplosion() :m_nMaxTexSplit(0.0f)
{
}

CExplosion::~CExplosion()
{
}

HRESULT CExplosion::Init()
{
	float ExplosionSize = 80.0f;

	SetSize({ ExplosionSize,ExplosionSize,0.0f });

	CObject2D::Init();


	return S_OK;
}

void CExplosion::Uninit()
{
	CObject2D::Uninit();
}

void CExplosion::Update()
{
	D3DXVECTOR2 AnimeCounter = GetAnimeCounter();

	float nMaxTexSplit = AnimeCounter.x + AnimeCounter.y + 2;

	CObject2D::Update();

	if (nMaxTexSplit == m_nMaxTexSplit)
	{
		Uninit();
	}
}

void CExplosion::Draw()
{
	CObject2D::Draw();
}

CExplosion *CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR2 TexSplit)
{
	CExplosion* pExplosion = new CExplosion;

	pExplosion->SetPos(pos);
	pExplosion->SetTexSplit(TexSplit);
	pExplosion->SetAnimeInterval(2);
	pExplosion->BindTexture(m_pTexture);

	pExplosion->m_nMaxTexSplit = TexSplit.x + TexSplit.y;

	pExplosion->Init();

	return pExplosion;
}

HRESULT CExplosion::TextureLoad()
{
	if (m_pTexture == nullptr)
	{
		CRenderer* Renderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sea_explosion.png", &m_pTexture);
	}
	return S_OK;
}

void CExplosion::TextureUnLoad()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}