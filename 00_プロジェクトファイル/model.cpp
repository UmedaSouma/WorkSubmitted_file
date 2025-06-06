//===========================================================================================================================================================
// 
// モデルの管理
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "model.h"
#include "player3D.h"
#include "manager.h"
#include "editor.h"

//===================================================================================
// コンストラクタ
//===================================================================================
CModel::CModel(int nPriority) :CObject3D(nPriority)
, m_Direction(0)
, m_ModelType(MODEL_TYPE::NORMAL)
, m_nIdx(-1)
, m_bMat(false)
, m_Diffuse{}
, m_pModelAddress{}
, m_apTexture{}
{
	m_ModelInfo = {};
}

//===================================================================================
// デストラクタ
//===================================================================================
CModel::~CModel()
{
}

//===================================================================================
// 初期設定
//===================================================================================
HRESULT CModel::Init()
{
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_pEffect = nullptr;	// 初期化
	CRenderer* pRender = CManager::GetRenderer();	// レンダラーを取得
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();	// デバイスを取得

#if 1

	// エフェクト生成
	D3DXCreateEffectFromFile(
		pDevice,	// 描画デバイス
		"test.fx",	// ファイルパス
		nullptr,	// プリプロセッサ定義
		nullptr,	// ID3DXIncludeインターフェイスの定義
		0,			// 読み込み時のオプションフラグ
		nullptr,	// ID3DXEffectPoolインターフェイス
		&m_pEffect,	// ID3DXEffectのインターフェイスのポインタ
		nullptr		// コンパイルエラー時の情報
	);

#endif // 0

	m_ModelType = (MODEL_TYPE)CEditor::GetModelType();

	UpdateMatrix();	// マトリックスの更新

	// デバッグ用の変数の初期化
	m_Debug_Radian = 50.0f;
	m_Debug_f = 0.0f;

	return S_OK;
}

//===================================================================================
// 終了処理
//===================================================================================
void CModel::Uninit()
{
	//CPlayer3D* pPlayer3D = dynamic_cast<CPlayer3D*>(this);
	//if (pPlayer3D == nullptr)
	//{
		//if (m_ModelInfo.Mesh != nullptr)
		//{
		//	m_ModelInfo.Mesh->Release();
		//	m_ModelInfo.Mesh = nullptr;
		//}
	//}
	
	//if (m_ModelInfo.BuffMat != nullptr)
	//{
	//	m_ModelInfo.BuffMat->Release();
	//	m_ModelInfo.BuffMat = nullptr;
	//}

	//if (m_pEffect != nullptr)
	//{
	//	m_pEffect = nullptr;
	//}

	Release();
}

//===================================================================================
// 更新処理
//===================================================================================
void CModel::Update()
{
	// オブジェクト3Dの更新
	CObject3D::Update();	// ここでマトリックスの設定をしている

	FixRot();	// 角度の修正
}

//===================================================================================
// 描画処理
//===================================================================================
void CModel::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータのポインタ
	CInputKeyBoard*keyboad = CManager::GetKeyboard();	// キーボードの取得

	// デバッグ用の操作
	if (keyboad->GetPress(DIK_UPARROW))
	{
		m_Debug_Radian++;
	}
	else if (keyboad->GetPress(DIK_DOWNARROW))
	{
		m_Debug_Radian--;
	}

#if 1

	//// ビュー変換・射影変換
	//D3DXMATRIX View, Proj;
	//D3DXMatrixPerspectiveFovLH(&Proj, D3DXToRadian(m_Debug_Radian), SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 10000.0f);

#endif // m_Debug_IsShader


	

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &GetMatrix());

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (m_ModelInfo.BuffMat != nullptr
		&& m_ModelInfo.Mesh != nullptr
		&& m_ModelInfo.NumMat != 0)
	{
		m_Debug_f += 0.025f;

		// マテリアルデータのポインタの取得
		pMat = (D3DXMATERIAL*)m_ModelInfo.BuffMat->GetBufferPointer();
		CModeldata* pModeldata = CManager::GetModeldata();	// Modeldata のポインタを持ってくる

		// エフェクト内のワールドビュー射影変換行列を設定
		//D3DXMATRIX mat = GetMatrix();
		////D3DXMatrixLookAtLH(&View, &D3DXVECTOR3(30 * sin(f), 20, -30 * cos(f)), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));

		//D3DXVECTOR3 v1(CManager::GetCamera()->GetPos());
		//D3DXVECTOR3 v2(GetPos());
		//D3DXVECTOR3 v3(0, 1, 0);
		//D3DXMatrixLookAtLH(&View, &v1, &v2, &v3);

		//D3DXMatrixIdentity(&mat);
		//mat = mat * View * Proj;

		//m_pEffect->SetMatrix("matWorldViewProj", &mat);

		DemoUpdateSheder();

		if (pModeldata)
		{
			for (int nCntMat = 0; nCntMat < (int)m_ModelInfo.NumMat; nCntMat++)
			{
				

				if (m_bMat)
				{// ダメージを受けたとき
					pMat[nCntMat].MatD3D.Diffuse = { 1.0f,0.0f,0.0f,0.5f };	// 赤くする
				}
				else
				{// そのほかの状態の時

					//if (CScene::GetMode() != CScene::MODE_EDITOR)
					{
						pMat[nCntMat].MatD3D.Diffuse = *pModeldata->GetMatData(m_pModelAddress, nCntMat);	// 通常色にする
					}
				}


				if (m_pEffect != nullptr)
				{
					m_pEffect->SetTechnique("BasicTec");
					UINT U_numpass;
					m_pEffect->Begin(&U_numpass, 0);

					// 色をそのまま映し出す(影なし)
					D3DXCOLOR _LocalColor = (D3DXCOLOR)pMat[nCntMat].MatD3D.Diffuse;
					D3DXVECTOR4 LocalColor = (D3DXVECTOR4)_LocalColor;
					m_pEffect->SetVector("ScreenColor", &LocalColor);

					for (unsigned int i = 0; i < U_numpass; i++)
					{
						m_pEffect->BeginPass(i);

						// モデルパーツの描画
						m_ModelInfo.Mesh->DrawSubset(nCntMat);

						m_pEffect->EndPass();
					}

					if (GetType() == CObject::BASE)
					{// エディターのベースオブジェクトの時だけ少し透かす
						pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;
					}

					

					
				}
				else
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// テクスチャの設定
					pDevice->SetTexture(0, nullptr);

					// モデルパーツの描画
					m_ModelInfo.Mesh->DrawSubset(nCntMat);
				}

				

				//D3DXVECTOR4 color = { pMat[nCntMat].MatD3D.Diffuse.r,pMat[nCntMat].MatD3D.Diffuse.g,pMat[nCntMat].MatD3D.Diffuse.b,pMat[nCntMat].MatD3D.Diffuse.a };

				//m_pEffect->SetVector("OutLineColor", &color);

				

				if (m_pEffect != nullptr)
				{
					m_pEffect->End();
				}
			}
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================================================================================================
// 角度を修正する
//===========================================================================================================
void CModel::FixRot()
{
	if (GetRot().x >= D3DX_PI*2 ||  GetRot().x <= -D3DX_PI*2)
	{
		SetRot({ 0.0f,GetRot().y,GetRot().z });
	}
	if (GetRot().y >= D3DX_PI * 2 || GetRot().y <= -D3DX_PI * 2)
	{
		SetRot({ GetRot().x,0.0f,GetRot().z });
	}
	if (GetRot().z >= D3DX_PI * 2 ||  GetRot().z <= -D3DX_PI * 2)
	{
		SetRot({ GetRot().x,GetRot().y,0.0f });
	}
}

//===================================================================================
// 生成処理
//===================================================================================
CModel* CModel::Create(D3DXVECTOR3 pos,TYPE type, D3DXVECTOR3 scale, D3DXVECTOR3 rot)
{
	CModel* pModel = new CModel;

	pModel->SetPos(pos);
	pModel->SetType(TYPE::MODEL);

	pModel->Init();

	return pModel;
}

//===================================================================================
// モデルの情報をセットする
//===================================================================================
void CModel::BindModel(CModeldata::ModelData *modelinfo)
{
	m_ModelInfo.Mesh = modelinfo->Mesh;
	m_ModelInfo.BuffMat = modelinfo->BuffMat;
	m_ModelInfo.NumMat = modelinfo->NumMat;
}

//===========================================================================================================
// 向きを設定
//===========================================================================================================
void CModel::SetDirection(DIRECTION dire)
{
	switch (dire)
	{
	case DIRECTION_LEFT:
		m_Direction = -1;
		SetRot({
			0.0f
			,D3DX_PI * 0.5f
			,0.0f
			});
		break;

	case DIRECTION_RIGHT:
		m_Direction = 1;
		SetRot({
			0.0f
			,D3DX_PI * -0.5f
			,0.0f
			});
		break;

	default:
		break;
	}
}

//===========================================================================================================
// 向きを取得
//===========================================================================================================
int CModel::GetDirection()
{
	return m_Direction;
}

//===========================================================================================================
// ModelFileの設定
//===========================================================================================================
void CModel::SetModelFile()
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		m_ModelFile[i].name = "a";
	}
}

//===========================================================================================================
// モデルの見た目設定
//===========================================================================================================
void CModel::SetModel(const char* address)
{
	CModeldata* pModeldata = nullptr;
	int nIdx = 0;

	SetModelAddress(address);		// アドレスを保存しておく
	pModeldata = CManager::GetModeldata();
	nIdx = pModeldata->Regist(address);
	BindModel(CManager::GetModeldata()->GetAddress(nIdx));
}

//===========================================================================================================
// シェーダーのマトリックス計算
//===========================================================================================================
void CModel::DemoUpdateSheder()
{
	if (m_pEffect == nullptr)
		return;

	D3DXMATRIX World, View, w_Proj, Proj;
	World = GetMatrix();
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &Proj);

	D3DXMatrixMultiply(&w_Proj, &World, &View);
	D3DXMatrixMultiply(&w_Proj, &w_Proj, &Proj);

	//シェーダーに行列を設定
	m_pEffect->SetMatrix("matWVP", &w_Proj);
	
	//m_pEffect->SetMatrix("World", &World);

}

//===========================================================================================================
// モデルの設定見た目
//===========================================================================================================
void CModel::SetModel(int type)
{
	char* address = nullptr;

	switch (type)
	{
	case GRASS:
		// モデルの設定
		SetModel("data\\model\\block_grass_000.x");
		break;

	case STONE:
		// モデルの設定
		SetModel("data\\model\\block_stone_000.x");
		break;

	case START:
		// モデルの設定
		SetModel("data\\model\\gimmick_start.x");
		break;

	case GOAL:
		// モデルの設定
		SetModel("data\\model\\gimmick_goal.x");
		break;

	case SLIME:
		SetModel("data\\model\\enemy_slime_000.x");
		break;

	case GOLEM:
		SetModel("data\\model\\enemy_golem000.x");
		break;

	default:
		assert(0);
		break;
	}

	if (address != nullptr)
	{
		SetModel(address);
	}
}
