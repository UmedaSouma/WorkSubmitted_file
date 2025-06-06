#include "renderer.h"
#include "object.h"
#include "scene.h"
#include "manager.h"
#include "player3D.h"
#include "game.h"
#include "search.h"
#include "sword.h"
#include "sound.h"

#include "imgui_impl_dx9.h"


CText* CRenderer::m_pText = nullptr;

//===========================================================================================================
// �R���X�g���N�^
//===========================================================================================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_pFont = nullptr;
}

//===========================================================================================================
// �f�X�g���N�^
//===========================================================================================================
CRenderer::~CRenderer()
{

}

//===========================================================================================================
// �����ݒ�
//===========================================================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;	// �f�B�X�v���C���[�h
	//D3DPRESENT_PARAMETERS m_d3dpp;	// �v���[���e�[�V�����p�����[�^

	m_pText = new CText;

	m_pText->Init();

	m_ResizeWidth = 0;
	m_ResizeHeight = 0;


	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));							// �p�����[�^�̃[���N���A

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʂ̃T�C�Y(��)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʂ̃T�C�Y(����)
	m_d3dpp.BackBufferFormat = d3ddm.Format;						// 
	m_d3dpp.BackBufferCount = 1;									// 
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// 
	m_d3dpp.EnableAutoDepthStencil = TRUE;						// 
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// 
	m_d3dpp.Windowed = bWindow;									// 
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// 
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// 

	// 
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		// 
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	

	//
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "Terminal", &m_pFont);

	InitSound(hWnd);

	return S_OK;
}

//===========================================================================================================
// �I������
//===========================================================================================================
void CRenderer::Uninit()
{
	// 
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// 
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	if (m_pText != nullptr)
	{
		m_pText->Uninit();
		delete m_pText;
		m_pText = nullptr;
	}

	UninitSound();
}

//===========================================================================================================
// �X�V����
//===========================================================================================================
void CRenderer::Update()
{
	// �S�čX�V
	CObject::UpdateAll();
}

//===========================================================================================================
// �`�揈��
//===========================================================================================================
void CRenderer::Draw()
{
	// �w�i�Ȃǂ̐ݒ�
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		m_pD3DDevice->Clear(0, nullptr,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(100, 178, 255, 0), 1.0f, 0);
	}
	else
	{
		m_pD3DDevice->Clear(0, nullptr,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	}

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_ResizeWidth != 0 && m_ResizeHeight != 0)
	{
		//m_d3dpp.BackBufferWidth = m_ResizeWidth;
		//m_d3dpp.BackBufferHeight = m_ResizeHeight;
		//m_ResizeWidth = m_ResizeHeight = 0;

		//ImGui_ImplDX9_InvalidateDeviceObjects();
		//HRESULT hr = pDevice->Reset(&m_d3dpp);
		//if (hr == D3DERR_INVALIDCALL)
		//	IM_ASSERT(0);
		//ImGui_ImplDX9_CreateDeviceObjects();
	}

	RECT rect = { 0,0,m_ResizeWidth,m_ResizeWidth };

	// 
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		char aStr[256] = {};
		char aEnemyStr[256] = {};

		CManager::GetCamera()->SetCamera();

		// �S�ĕ`��
		CObject::DrawAll();

		// �f�o�b�O�e�L�X�g
		DebugPrint(aStr[0],aEnemyStr[0]);

		m_pText->DrawAll();

		// 
		m_pD3DDevice->EndScene();

		// �e�L�X�g�̕`��
		m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

		m_pFont->DrawText(NULL, &aEnemyStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

		// ImGui�̃����_���[
		// �`��ɕK�v
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//===========================================================================================================
// �f�o�b�O�e�L�X�g�̕`��
//===========================================================================================================
void CRenderer::DebugPrint(char& pStr, char& pStr2)
{
	CFade* pFade = CManager::GetFade();
	CScene::MODE mode = CScene::GetMode();

	if (mode == CScene::MODE_GAME || mode == CScene::MODE_EDITOR)
	{
		std::vector<CEnemy3D*>pEnemy;
		CSword* pSword = nullptr;
		
		pEnemy = CSearch::SearchMultiObject(pEnemy, CObject::ENEMY_MESSE);
		pSword = CSearch::SearchObject(pSword, CObject::SWORD);

		CPlayer3D* pPlayer = nullptr;
		D3DXVECTOR3 playerpos, playermove = {};

		pPlayer = CSearch::SearchObject(pPlayer, CObject::TYPE::PLAYER);
		if (pPlayer != nullptr)
		{
			playerpos = pPlayer->GetPos();
			playermove = pPlayer->GetMove();
		}

		int tag = 0;

		if (mode == CScene::MODE_GAME)
		{
			tag = pPlayer->GetFrontTag();
		}



#if _DEBUG

		if (CScene::GetMode() != CScene::MODE_EDITOR)
		{
			sprintf(&pStr,
				"�t�F�[�h�A���t�@�l : %f \n"
				//"enemypos %.2f , %.2f, %.2f \n"
				" playerpos %.2f , %.2f, %.2f \n"
				" playermove %.2f , %.2f ,%.2f \n "
				" fronttag %d \n"
				"swordrot %f , %f , %f \n"
				"swordtargetrot %f , %f , %f \n"
				"\nplayerlife %d"
				, pFade->GetColor().a
				, playerpos.x, playerpos.y, playerpos.z
				, playermove.x, playermove.y, playermove.z
				, tag
				, pSword->GetRot().x, pSword->GetRot().y, pSword->GetRot().z
				, pSword->GetTargetrot().x, pSword->GetTargetrot().y, pSword->GetTargetrot().z
				, pPlayer->GetLife()

			);

			if (pEnemy.size() != 0)
			{
				sprintf(&pStr2,
					"\n\n\n\n\n\n\n\n\n\nenemy_00 \n"
					"pos : %f , %f , %f \n"
					"size : %f , %f , %f \n"
					"state : %d |  \n"
					"respawn : %d \n"
					, pEnemy[0]->GetPos().x, pEnemy[0]->GetPos().y, pEnemy[0]->GetPos().z
					, pEnemy[0]->GetSize().x, pEnemy[0]->GetSize().y, pEnemy[0]->GetSize().z
					, pEnemy[0]->GetState()
					, pEnemy[0]->GetRespawnCnt()
				);
			}
		}

		
#endif // !_DEBUG

	}
}

//===========================================================================================================
// �f�o�C�X�̎擾
//===========================================================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return m_pD3DDevice;
}