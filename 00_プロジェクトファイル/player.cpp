#include "player.h"
#include "input.h"
#include "bullet.h"
#include "manager.h"
#include "block.h"

int CPlayer::m_NumAllPlayer = 0;
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = nullptr;

CPlayer::CPlayer()
{
	m_NumAllPlayer++;
}

CPlayer::~CPlayer()
{
	m_NumAllPlayer--;
}

HRESULT CPlayer::Init()
{
	CObject2D::Init();
	//CObject3D::Init();

	m_Move = {};
	m_bMoveSwitch = false;
	m_fSpeed = 2.0f;

	return S_OK;
}

void CPlayer::Uninit()
{
	CObject2D::Uninit();
	//CObject3D::Uninit();
}

void CPlayer::Update()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	
	rot = InputRotPlayer(rot);

	pos.x += m_Move.x;

	SetPos(pos);

	pos = LookObjectSIDE();

	pos.y += m_Move.y;

	SetPos(pos);

	pos = LookObjectUPDOWN();

	SetRot(rot);

	Jump();

	InputPosPlayer(pos);

	//SetPos(pos);
	m_Move.x += (0.0f - m_Move.x) * 0.2f;

	CObject2D::Update();
	//CObject3D::Update();

	m_oldPos = pos;

}

void CPlayer::Draw()
{
	CObject2D::Draw();
	//CObject3D::Draw();
}

D3DXVECTOR3 CPlayer::InputPosPlayer(D3DXVECTOR3 pos)
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();

	if (keyboard->GetPress(DIK_A))
	{
		m_Move.x += sinf(-D3DX_PI * 0.5f) * m_fSpeed;
		m_Move.y += cosf(-D3DX_PI * 0.5f) * m_fSpeed;
	}
	if (keyboard->GetPress(DIK_D))
	{
		m_Move.x -= sinf(-D3DX_PI * 0.5f) * m_fSpeed;
		m_Move.y -= cosf(-D3DX_PI * 0.5f) * m_fSpeed;
	}
	if (keyboard->GetPress(DIK_W))
	{
		
	}
	if (keyboard->GetPress(DIK_S))
	{
		
	}

	
	if (keyboard->GetTrigger(DIK_F))
	{
		CBullet::Create(pos);
	}

	return m_Move;
}

D3DXVECTOR3 CPlayer::InputRotPlayer(D3DXVECTOR3 rot)
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();

	float localrot = m_TargetRot.z - rot.z;

	localrot *= 0.05f;

	rot.z += localrot;

	if (keyboard->GetPress(DIK_E))
	{
		rot.z += 0.05f;
	}

	return rot;
}

void CPlayer::Jump()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();

	if (keyboard->GetTrigger(DIK_SPACE))
	{
		m_Move.y -= 20.1f;
	}

	m_Move.y += 1.0f;
}

CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CPlayer* pPlayer = nullptr;
	pPlayer = new CPlayer;

	pPlayer->SetPos(pos);
	pPlayer->SetSize(size);
	//pPlayer->SetTex(tex);
	//pPlayer->m_nTexSplit = TexSplit;
	//pPlayer->m_nAnimeInterval = Interval;
	pPlayer->BindTexture(m_pTexture);

	pPlayer->Init();
	

	//pPlayer->Init();

	return pPlayer;
}

HRESULT CPlayer::TextureLoad()
{
	if (m_pTexture == nullptr)
	{
		CRenderer* Renderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player_human_walk001.png", &m_pTexture);
	}
	return S_OK;
}

void CPlayer::TextureUnLoad()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

D3DXVECTOR3 CPlayer::GetOldPos()
{
	return m_oldPos;
}

void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_Move = move;
}

D3DXVECTOR3 CPlayer::LookObjectUPDOWN()
{
	D3DXVECTOR3 pos = {};

	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		CObject* pObj = GetOBJECT(PRIORITY_DEFAULT, nCntObj);

		if (pObj != nullptr)
		{
			pos = UpDownCollision(pObj);
		}
	}

	return pos;
}

D3DXVECTOR3 CPlayer::LookObjectSIDE()
{
	D3DXVECTOR3 pos = {};

	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		CObject* pObj = GetOBJECT(PRIORITY_DEFAULT, nCntObj);

		if (pObj != nullptr)
		{
			pos = SideCollision(pObj);
		}
	}

	return pos;
}

D3DXVECTOR3 CPlayer::UpDownCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 PlayerPos = GetPos();
	D3DXVECTOR3 PlayerSize = GetSize();

	PlayerSize.x *= 0.5f;
	PlayerSize.y *= 0.5f;

	if (type == TYPE::BLOCK)
	{
		CBlock* pBlock = (CBlock*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		BlockSize.y *= 0.5f;

		if (BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x
			&& m_oldPos.y + PlayerSize.y <= BlockPos.y - BlockSize.y
			)
		{// ã‚Éæ‚é
			m_Move.y = 0;

			SetPos({ PlayerPos.x,PlayerPos.y=(BlockPos.y - BlockSize.y - PlayerSize.y),PlayerPos.z });
		}

		if (BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y
			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x
			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x
			&& m_oldPos.y - PlayerSize.y >= BlockPos.y + BlockSize.y
			)
		{
			m_Move.y = 0;

			SetPos({ PlayerPos.x,PlayerPos.y = (BlockPos.y + BlockSize.y + PlayerSize.y),PlayerPos.z });
		}
	}

	return PlayerPos;
}

D3DXVECTOR3 CPlayer::SideCollision(CObject* pObj)
{
	CObject::TYPE type = pObj->GetType();

	D3DXVECTOR3 PlayerPos = GetPos();
	D3DXVECTOR3 PlayerSize = GetSize();

	PlayerSize.x *= 0.5f;
	PlayerSize.y *= 0.5f;

	if (type == TYPE::BLOCK)
	{
		CBlock* pBlock = (CBlock*)pObj;

		D3DXVECTOR3 BlockPos = pBlock->GetPos();
		D3DXVECTOR3 BlockSize = pBlock->GetSize();

		BlockSize.x *= 0.5f;
		BlockSize.y *= 0.5f;

		if (BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y
			&& m_oldPos.x - PlayerSize.x >= BlockPos.x + BlockSize.x
			)
		{
			m_Move.x = 0;

			SetPos({ PlayerPos.x=(BlockPos.x + BlockSize.x + PlayerSize.x),PlayerPos.y,PlayerPos.z });
		}

		else if (BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x
			&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y
			&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y
			&& m_oldPos.x + PlayerSize.x <= BlockPos.x - BlockSize.x
			)
		{
			m_Move.x = 0;

			SetPos({ PlayerPos.x=(BlockPos.x - BlockSize.x - PlayerSize.x),PlayerPos.y,PlayerPos.z });
		}
	}

	return PlayerPos;
}

